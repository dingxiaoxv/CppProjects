#include <cnr/eventbase.h>
#include <cnr/logging.h>
#include <cnr/socket.h>
#include "gattclient.h"
using namespace dm;

static void log_service_event(struct gatt_db_attribute* attr, const char* str) {
  char      uuid_str[MAX_LEN_UUID_STR];
  bt_uuid_t uuid;
  uint16_t  start, end;

  gatt_db_attribute_get_service_uuid(attr, &uuid);
  bt_uuid_to_string(&uuid, uuid_str, sizeof(uuid_str));

  gatt_db_attribute_get_service_handles(attr, &start, &end);

  CNR_INFO("%s - UUID: %s start: 0x%04x end: 0x%04x", str, uuid_str, start, end);
}

static void print_desc(struct gatt_db_attribute* attr, void* user_data) {
  CNR_INFO("\t\t descr - handle: 0x%04x, uuid: ", gatt_db_attribute_get_handle(attr));
  // print_uuid(gatt_db_attribute_get_type(attr));
}

static void print_chrc(struct gatt_db_attribute* attr, void* user_data) {
  uint16_t  handle, value_handle;
  uint8_t   properties;
  uint16_t  ext_prop;
  bt_uuid_t uuid;

  if (!gatt_db_attribute_get_char_data(attr, &handle, &value_handle, &properties, &ext_prop, &uuid))
    return;

  CNR_INFO(
      "\t  charac - start: 0x%04x, value: 0x%04x, "
      "props: 0x%02x, ext_props: 0x%04x, handle: 0x%04x",
      handle, value_handle, properties, ext_prop, gatt_db_attribute_get_handle(attr));
  // print_uuid(&uuid);

  gatt_db_service_foreach_desc(attr, print_desc, NULL);
}

static void print_service(struct gatt_db_attribute* attr, void* user_data) {
  BleClient* cli = (BleClient*)user_data;
  uint16_t   start, end;
  bool       primary;
  bt_uuid_t  uuid;

  if (!gatt_db_attribute_get_service_data(attr, &start, &end, &primary, &uuid)) return;

  char uuid_str[MAX_LEN_UUID_STR];
  gatt_db_attribute_get_service_uuid(attr, &uuid);
  bt_uuid_to_string(&uuid, uuid_str, sizeof(uuid_str));

  CNR_INFO("service - start: 0x%04x, end: 0x%04x, type: %s, uuid: %s", start, end,
           primary ? "primary" : "secondary", uuid_str);
  // print_uuid(&uuid);

  // gatt_db_service_foreach_incl(attr, print_incl, cli);
  gatt_db_service_foreach_char(attr, print_chrc, NULL);
}

static void print_services(BleClient* cli) {
  gatt_db_foreach_service(cli->db(), NULL, print_service, cli);
}

static void att_disconnect_cb(int err, void* user_data) {
  cLOG_INFO << "Device disconnected: " << strerror(err);
  mainloop_quit();
}

static void service_added_cb(struct gatt_db_attribute* attr, void* user_data) {
  log_service_event(attr, "Service Added");
}

static void service_removed_cb(struct gatt_db_attribute* attr, void* user_data) {
  log_service_event(attr, "Service Removed");
}

static void ready_cb(bool success, uint8_t att_ecode, void* user_data) {
  BleClient* cli = (BleClient*)user_data;

  if (!success) {
    CNR_ERROR("GATT discovery procedures failed - error code: 0x%02x", att_ecode);
    return;
  }

  cLOG_INFO << "GATT discovery procedures complete";
  print_services(cli);
}

static void service_changed_cb(uint16_t start_handle, uint16_t end_handle, void* user_data) {
  BleClient* cli = (BleClient*)user_data;

  CNR_INFO("Service Changed handled - start: 0x%04x end: 0x%04x", start_handle, end_handle);

  gatt_db_foreach_service_in_range(cli->db(), NULL, print_service, cli, start_handle, end_handle);
  // print_prompt();
}

BleClient::BleClient(bdaddr_t* src, bdaddr_t* dst, uint16_t mtu) {
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
  dstaddr.l2_family      = AF_BLUETOOTH;
  dstaddr.l2_cid         = htobs(4);
  dstaddr.l2_bdaddr_type = BDADDR_LE_PUBLIC;
  bacpy(&dstaddr.l2_bdaddr, dst);

  cLOG_INFO << "Connecting to device...";

  if (connect(fd_, (struct sockaddr*)&dstaddr, sizeof(dstaddr)) < 0) {
    cLOG_ERROR << "Failed to connect: " << strerror(errno);
    close(fd_);
    fd_ = -1;
    return;
  }

  att_ = bt_att_new(fd_, false);
  if (!att_) {
    cLOG_ERROR << "Failed to initialze ATT transport layer";
    bt_att_unref(att_);
  }

  if (!bt_att_set_close_on_unref(att_, true)) {
    cLOG_ERROR << "Failed to set up ATT transport layer";
    bt_att_unref(att_);
  }

  if (!bt_att_register_disconnect(att_, att_disconnect_cb, NULL, NULL)) {
    cLOG_ERROR << "Failed to set ATT disconnect handler";
    bt_att_unref(att_);
  }

  db_ = gatt_db_new();
  if (!db_) {
    cLOG_ERROR << "Failed to create GATT database";
    bt_att_unref(att_);
  }

  gatt_ = bt_gatt_client_new(db_, att_, mtu, 0);
  if (!gatt_) {
    cLOG_ERROR << "Failed to create GATT client";
    gatt_db_unref(db_);
    bt_att_unref(att_);
  }

  gatt_db_register(db_, service_added_cb, service_removed_cb, NULL, NULL);
  bt_gatt_client_ready_register(gatt_, ready_cb, this, NULL);
  bt_gatt_client_set_service_changed(gatt_, service_changed_cb, this, NULL);
}