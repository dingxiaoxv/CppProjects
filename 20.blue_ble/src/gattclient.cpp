#include "gattclient.h"

#include <cnr/eventbase.h>
#include <cnr/socket.h>
#include <cstdio>

#include "bluez/l2cap.h"

#define PRLOG(...) \
  print_prompt(); \
  printf(__VA_ARGS__);

#define COLOR_OFF "\x1B[0m"
#define COLOR_RED "\x1B[0;91m"
#define COLOR_GREEN "\x1B[0;92m"
#define COLOR_YELLOW "\x1B[0;93m"
#define COLOR_BLUE "\x1B[0;94m"
#define COLOR_MAGENTA "\x1B[0;95m"
#define COLOR_BOLDGRAY "\x1B[1;30m"
#define COLOR_BOLDWHITE "\x1B[1;37m"

using namespace dm;

static void print_prompt(void) {
  printf(COLOR_BLUE "[GATT client]" COLOR_OFF "# ");
  fflush(stdout);
}

static void print_uuid(const bt_uuid_t *uuid) {
  char uuid_str[MAX_LEN_UUID_STR];
  bt_uuid_t uuid128;

  bt_uuid_to_uuid128(uuid, &uuid128);
  bt_uuid_to_string(&uuid128, uuid_str, sizeof(uuid_str));

  printf("%s\n", uuid_str);
}

static void print_desc(struct gatt_db_attribute *attr, void *user_data) {
  (void)user_data;
  printf("\t\t  " COLOR_MAGENTA "descr" COLOR_OFF " - handle: 0x%04x, uuid: ",
         gatt_db_attribute_get_handle(attr));
  print_uuid(gatt_db_attribute_get_type(attr));
}

static void print_chrc(struct gatt_db_attribute *attr, void *user_data) {
  (void)user_data;
  uint16_t handle, value_handle;
  uint8_t properties;
  uint16_t ext_prop;
  bt_uuid_t uuid;

  if (!gatt_db_attribute_get_char_data(attr, &handle, &value_handle, &properties, &ext_prop, &uuid))
    return;

  printf("\t  " COLOR_YELLOW "charac" COLOR_OFF " - start: 0x%04x, value: 0x%04x, "
         "props: 0x%02x, ext_props: 0x%04x, uuid: ",
         handle, value_handle, properties, ext_prop);
  print_uuid(&uuid);

  gatt_db_service_foreach_desc(attr, print_desc, NULL);
}

static void print_incl(struct gatt_db_attribute *attr, void *user_data) {
  BleClient *cli = (BleClient *)user_data;
  uint16_t handle, start, end;
  struct gatt_db_attribute *service;
  bt_uuid_t uuid;

  if (!gatt_db_attribute_get_incl_data(attr, &handle, &start, &end))
    return;

  service = gatt_db_get_attribute(cli->db(), start);
  if (!service)
    return;

  gatt_db_attribute_get_service_uuid(service, &uuid);

  printf("\t  " COLOR_GREEN "include" COLOR_OFF " - handle: "
         "0x%04x, - start: 0x%04x, end: 0x%04x,"
         "uuid: ",
         handle, start, end);
  print_uuid(&uuid);
}

static void print_service(struct gatt_db_attribute *attr, void *user_data) {
  BleClient *cli = (BleClient *)user_data;
  uint16_t start, end;
  bool primary;
  bt_uuid_t uuid;

  if (!gatt_db_attribute_get_service_data(attr, &start, &end, &primary, &uuid))
    return;

  char uuid_str[MAX_LEN_UUID_STR];
  gatt_db_attribute_get_service_uuid(attr, &uuid);
  bt_uuid_to_string(&uuid, uuid_str, sizeof(uuid_str));

  printf(COLOR_RED "service" COLOR_OFF " - start: 0x%04x, "
                   "end: 0x%04x, type: %s, uuid: ",
         start, end, primary ? "primary" : "secondary");
  print_uuid(&uuid);

  gatt_db_service_foreach_incl(attr, print_incl, cli);
  gatt_db_service_foreach_char(attr, print_chrc, NULL);
  printf("\n");
}

static void print_services(BleClient *cli) {
  gatt_db_foreach_service(cli->db(), NULL, print_service, cli);
}

static void log_service_event(struct gatt_db_attribute *attr, const char *str) {
  char uuid_str[MAX_LEN_UUID_STR];
  bt_uuid_t uuid;
  uint16_t start, end;

  gatt_db_attribute_get_service_uuid(attr, &uuid);
  bt_uuid_to_string(&uuid, uuid_str, sizeof(uuid_str));
  gatt_db_attribute_get_service_handles(attr, &start, &end);

  PRLOG("%s - UUID: %s start: 0x%04x end: 0x%04x\n", str, uuid_str, start, end);
}

static void att_disconnect_cb(int err, void *user_data) {
  (void)user_data;
  printf("Device disconnected: %s\n", strerror(err));
  mainloop_quit();
}

static void service_added_cb(struct gatt_db_attribute *attr, void *user_data) {
  (void)user_data;
  log_service_event(attr, "Service Added");
}

static void service_removed_cb(struct gatt_db_attribute *attr, void *user_data) {
  (void)user_data;
  log_service_event(attr, "Service Removed");
}

static void ready_cb(bool success, uint8_t att_ecode, void *user_data) {
  BleClient *cli = (BleClient *)user_data;

  if (!success) {
    printf("GATT discovery procedures failed - error code: 0x%02x\n", att_ecode);
    return;
  }

  printf("GATT discovery procedures complete\n");
  print_services(cli);
}

static void service_changed_cb(uint16_t start_handle, uint16_t end_handle, void *user_data) {
  BleClient *cli = (BleClient *)user_data;

  printf("Service Changed handled - start: 0x%04x end: 0x%04x\n", start_handle, end_handle);
  gatt_db_foreach_service_in_range(cli->db(), NULL, print_service, cli, start_handle, end_handle);
  print_prompt();
}

BleClient::BleClient(bdaddr_t *src, bdaddr_t *dst, uint16_t mtu) {
  struct sockaddr_l2 srcaddr, dstaddr;
  struct bt_security btsec;

  fd_ = socket(PF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);
  if (fd_ < 0) {
    perror("Failed to create L2CAP socket");
    return;
  }

  /* Set up source address */
  memset(&srcaddr, 0, sizeof(srcaddr));
  srcaddr.l2_family = AF_BLUETOOTH;
  srcaddr.l2_cid = htobs(4);
  srcaddr.l2_bdaddr_type = 0;
  bacpy(&srcaddr.l2_bdaddr, src);

  if (bind(fd_, (struct sockaddr *)&srcaddr, sizeof(srcaddr)) < 0) {
    perror("Failed to bind L2CAP socket");
    close(fd_);
    return;
  }

  /* Set the security level */
  memset(&btsec, 0, sizeof(btsec));
  btsec.level = BT_SECURITY_LOW;
  if (setsockopt(fd_, SOL_BLUETOOTH, BT_SECURITY, &btsec, sizeof(btsec)) != 0) {
    fprintf(stderr, "Failed to set L2CAP security level\n");
    close(fd_);
    return;
  }

  /* Set up destination address */
  memset(&dstaddr, 0, sizeof(dstaddr));
  dstaddr.l2_family = AF_BLUETOOTH;
  dstaddr.l2_cid = htobs(4);
  dstaddr.l2_bdaddr_type = BDADDR_LE_RANDOM;
  bacpy(&dstaddr.l2_bdaddr, dst);

  char dst_addr_str[18];
  ba2str(&dstaddr.l2_bdaddr, dst_addr_str);
  printf("Connecting to device: %s\n", dst_addr_str);

  if (connect(fd_, (struct sockaddr *)&dstaddr, sizeof(dstaddr)) < 0) {
    printf("Failed to connect: %s\n", strerror(errno));
    close(fd_);
    fd_ = -1;
    return;
  }

  att_ = bt_att_new(fd_, false);
  if (!att_) {
    printf("Failed to initialze ATT transport layer\n");
    bt_att_unref(att_);
  }

  if (!bt_att_set_close_on_unref(att_, true)) {
    printf("Failed to set up ATT transport layer\n");
    bt_att_unref(att_);
  }

  if (!bt_att_register_disconnect(att_, att_disconnect_cb, NULL, NULL)) {
    printf("Failed to set ATT disconnect handler\n");
    bt_att_unref(att_);
  }

  db_ = gatt_db_new();
  if (!db_) {
    printf("Failed to create GATT database\n");
    bt_att_unref(att_);
  }

  gatt_ = bt_gatt_client_new(db_, att_, mtu, 0);
  if (!gatt_) {
    printf("Failed to create GATT client\n");
    gatt_db_unref(db_);
    bt_att_unref(att_);
  }

  gatt_db_register(db_, service_added_cb, service_removed_cb, NULL, NULL);
  bt_gatt_client_ready_register(gatt_, ready_cb, this, NULL);
  bt_gatt_client_set_service_changed(gatt_, service_changed_cb, this, NULL);
}