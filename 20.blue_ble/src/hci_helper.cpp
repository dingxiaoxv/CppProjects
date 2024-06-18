#include "hci_helper.h"
#include <string.h>
#include <errno.h>
#include <cnr/logging.h>
#include <cnr/datetime.h>
#include <unistd.h>
#include <thread>
#include <sys/ioctl.h>
#include "bluez/bluetooth.h"
#include "bluez/hci.h"
#include "bluez/hci_lib.h"

using namespace dm;

HciHelper& HciHelper::instance() {
  static HciHelper helper;
  return helper;
}

HciHelper::HciHelper() : fd_(-1), handle_(0) {
  if (0 != system("hciconfig hci0 down")) {
    cLOG_ERROR << "shut down hci0 failed";
  }
  usleep(10000);
  if (0 != system("hciconfig hci0 up")) {
    cLOG_ERROR << "bring up hci0 failed";
  }
  usleep(10000);
  dev_ = hci_get_route(NULL);
  if (dev_ < 0) {
    cLOG_ERROR << "failed to get the device id, error: " << strerror(errno);
    return;
  }

  // monitor the hci event
  std::thread t([this] { monitorRoutine();});
  t.detach();

  fd_ = hci_open_dev(dev_);
  if (fd_ < 0) {
    cLOG_ERROR << "failed to open device, error: " << strerror(errno);
    return;
  }
}

HciHelper::~HciHelper() {
  if (fd_ >= 0) {
    close(fd_);
  }
}

void HciHelper::monitorRoutine() {
  int fd = hci_open_dev(dev_);
  if (fd < 0) {
    cLOG_ERROR << "failed to open device for monitor, error: " << strerror(errno);
    return;
  }
  struct hci_filter flt;
  hci_filter_clear(&flt);
  hci_filter_set_ptype(HCI_EVENT_PKT, &flt);
  hci_filter_set_event(EVT_CMD_COMPLETE, &flt);
  hci_filter_set_event(EVT_DISCONN_COMPLETE, &flt);
  hci_filter_set_event(EVT_LE_META_EVENT, &flt);

  if (setsockopt(fd, SOL_HCI, HCI_FILTER, &flt, sizeof(flt)) < 0) {
    cLOG_ERROR << "Failed to set HCI filter";
    return;
  }

  bool advSetuped = false;
  while (true) {
    uint8_t buf[HCI_MAX_EVENT_SIZE];
    int len = read(fd, buf, sizeof(buf));
    if (len <= 0) {
      cLOG_ERROR << "Failed to read from HCI socket, error: " << strerror(errno);
      sleep(3);
      exit(0);
    }

    if (buf[0] != 0x04) {
      cLOG_WARN << "not a hci event msg";
      continue;
    }

    hci_event_hdr *hdr = (hci_event_hdr *) (buf + 1);
    if (hdr->evt == EVT_CMD_COMPLETE) {
      evt_cmd_complete *event = (evt_cmd_complete *)(buf + (1 + HCI_EVENT_HDR_SIZE));
      cLOG_INFO << "cmd complete, opcode = " << cnr::MStream::HEX << event->opcode;

      switch (event->opcode) {
        case 0x2005:
        case 0x2006:
        case 0x2008:
        case 0x200A: {
          advSetuped = true;
          uint8_t status = *(buf + (1 + HCI_EVENT_HDR_SIZE + EVT_CMD_COMPLETE_SIZE));
          cLOG_INFO << ">>>>>>> LE adv setup part, status = " << status;
          if (status != 0) fatal("LE adv setup failed");
          break;
        }
        case 0x0c03:
          cLOG_INFO << "device is reseted";
          if (advSetuped) fatal("device is reseted after adv setup");
          break;
        case 0xfc1b:
          cLOG_INFO << "vendor command 0x1b";
          if (advSetuped) fatal("exeute vendor command 0x1b after adv setup");
          break;
        default: break;
      }
    } else if (hdr->evt == EVT_DISCONN_COMPLETE) {
      evt_disconn_complete *event = (evt_disconn_complete *)(buf + (1 + HCI_EVENT_HDR_SIZE));
      cLOG_INFO << "disconn complete, reason = " << cnr::MStream::HEX << event->reason;
      if (event->reason == 0x3e) {
        fatal("CONNECTION FAILED TO BE ESTABLISHED!!");
      }
    } else if (hdr->evt == EVT_LE_META_EVENT) {
      evt_le_meta_event *event = (evt_le_meta_event *)(buf + (1 + HCI_EVENT_HDR_SIZE));
      if (event->subevent == EVT_LE_CONN_COMPLETE) {
        evt_le_connection_complete *conn_complete = (evt_le_connection_complete *)(event->data);
        cLOG_INFO << "le conn complete, status = " << conn_complete->status;
        if (conn_complete->status != 0) fatal("LE connection failed");
      } else if (event->subevent == EVT_LE_CONN_UPDATE_COMPLETE) {
        evt_le_connection_update_complete *conn_update_complete = (evt_le_connection_update_complete *)(event->data);
        cLOG_INFO << "le conn update complete, status = " << conn_update_complete->status
                  << ", interval = " << conn_update_complete->interval;
      }
    }
  }
}

void HciHelper::fatal(const char* msg) {
  cLOG_FATAL << msg;
  sleep(3);
  exit(0);
}

HCI_VERSION HciHelper::getHciVersion() {
  struct hci_version ver;
  int ret = hci_read_local_version(fd_, &ver, 2000);
  if (ret < 0) {
    cLOG_ERROR << "failed to read local hci version, error: " << strerror(errno);
    return HCI_BT_UNKNOWN;
  }

  if (ver.hci_ver < HCI_BT_4_0 || ver.hci_ver > HCI_BT_5_3) {
    cLOG_ERROR << "unsupported hci version: " << ver.hci_ver;
    return HCI_BT_UNKNOWN;
  }

  return (HCI_VERSION) ver.hci_ver;
}

void HciHelper::reEnableAdvertsing() {
  struct hci_request rq;
  uint8_t status;
  rq.ogf = OGF_LE_CTL;
  rq.ocf = OCF_LE_SET_ADVERTISE_ENABLE;
  rq.clen = LE_SET_ADVERTISE_ENABLE_CP_SIZE;
  uint8_t enable = 0x00;
  rq.cparam = &enable;
  rq.rparam = &status;
  rq.rlen = 1;
  if (hci_send_req(fd_, &rq, 1000) < 0) {
    cLOG_FATAL << "failed to disable adv, error = " << strerror(errno);
    exit(0);
  }

  if (status != 0) {
    cLOG_FATAL << "disable advertisse result is " << status;
    exit(0);
  }

  enable = 0x01;
  rq.cparam = &enable;
  rq.rparam = &status;
  rq.rlen = 1;
  if (hci_send_req(fd_, &rq, 1000) < 0) {
    cLOG_FATAL << "failed to enable adv, error = " << strerror(errno);
    exit(0);
  }

  if (status != 0) {
    cLOG_FATAL << "enable advertisse result is " << status;
    exit(0);
  }
}

void HciHelper::setLeAdvertisingData(uint16_t service, const char* data, const char* mac) {
  struct hci_request rq;
  uint8_t status;
  rq.ogf = OGF_LE_CTL;

  if (mac) {
    cLOG_INFO << "try to set random address to " << mac << " for advertising";
    rq.ocf = OCF_LE_SET_RANDOM_ADDRESS;
    rq.clen = LE_SET_RANDOM_ADDRESS_CP_SIZE;
    le_set_random_address_cp randomaddr;
    str2ba(mac, &randomaddr.bdaddr);
    rq.cparam = &randomaddr;
    rq.rparam = &status;
    rq.rlen = 1;
    if (hci_send_req(fd_, &rq, 1000) < 0) {
      cLOG_FATAL << "failed to send req, type is set random addr, error = " << strerror(errno);
      exit(0);
    }

    if (status != 0) {
      cLOG_FATAL << "set random address result is " << status;
      exit(0);
    }
  }

  rq.ocf = OCF_LE_SET_ADVERTISING_PARAMETERS;
  rq.clen = LE_SET_ADVERTISING_PARAMETERS_CP_SIZE;
  le_set_advertising_parameters_cp adv_params;
  memset(&adv_params, 0, sizeof(adv_params));
  adv_params.min_interval = htobs(0x0020);
  adv_params.max_interval = htobs(0x0020);
  adv_params.advtype = 0x00; // Connectable undirected advertising
  adv_params.chan_map = 0x07; // All channels
  adv_params.own_bdaddr_type = mac ? 0x01 : 0x00; // random Device Address
  adv_params.direct_bdaddr_type = 0x00; // Public Device Address
  rq.cparam = &adv_params;
  rq.rparam = &status;
  rq.rlen = 1;
  if (hci_send_req(fd_, &rq, 1000) < 0) {
    cLOG_FATAL << "failed to send req, type is et adv params, error = " << strerror(errno);
    exit(0);
  }

  if (status != 0) {
    cLOG_FATAL << "set advertising parameters result is " << status;
    exit(0);
  }

  rq.ocf = OCF_LE_SET_ADVERTISE_ENABLE;
  rq.clen = LE_SET_ADVERTISE_ENABLE_CP_SIZE;
  uint8_t enable = 0x01;
  rq.cparam = &enable;
  rq.rparam = &status;
  rq.rlen = 1;
  if (hci_send_req(fd_, &rq, 1000) < 0) {
    cLOG_FATAL << "failed to send req, ctrl code = 0x0a, error = " << strerror(errno);
    exit(0);
  }

  if (status != 0) {
    cLOG_FATAL << "enable advertisse result is " << status;
    exit(0);
  }

  LeAdvertising adv;
  adv.service.uuid[0] = service & 0xff;
  adv.service.uuid[1] = (service >> 8) & 0xff;
  adv.serviceData.uuid[0] = service & 0xff;
  adv.serviceData.uuid[1] = (service >> 8) & 0xff;
  memcpy(adv.serviceData.data, data, std::min(sizeof(adv.serviceData.data) / sizeof(uint8_t), strlen(data)));
  // memset(adv.localName.name, 0, 22);
  // memcpy(adv.localName.name, data, 22);

  rq.ocf = OCF_LE_SET_ADVERTISING_DATA;
  rq.clen = LE_SET_ADVERTISING_DATA_CP_SIZE;
  rq.cparam = &adv;
  rq.rparam = &status;
  rq.rlen = 1;

  if (hci_send_req(fd_, &rq, 1000) < 0) {
    cLOG_FATAL << "failed to send req, error = " << strerror(errno);
    exit(0);
  }

  if (status != 0) {
    cLOG_FATAL << "set advertising data result is " << status;
    exit(0);
  }

  cLOG_INFO << "adv setup done, uptime = " << cnr::Datetime::secTicks();
}

void HciHelper::setLeAdvertisingDataExt(uint16_t service, const char* data) {
  struct hci_request rq;
  uint8_t status;
  rq.ogf = OGF_LE_CTL;
  rq.ocf = 0x36;
  rq.clen = 25;
  uint8_t buf[25] = { 
    0x01, 0x13, 0x00, 0x00, 0x08, 0x00, 0x00, 0x08,
    0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x7f, 0x01, 0x00, 0x01, 0x00,
    0x00
  };

  rq.cparam = buf;
  rq.rparam = &status;
  rq.rlen = 1;
  if (hci_send_req(fd_, &rq, 1000) < 0) {
    cLOG_ERROR << "failed to send req, ctrl code = 0x36, error = " << strerror(errno);
    return;
  }


  rq.ocf = 0x38;
  rq.clen = 4;
  uint8_t buf2[4] = { 0x01, 0x03, 0x01, 0x00 };
  rq.cparam = buf2;
  rq.rparam = &status;
  rq.rlen = 1;
  if (hci_send_req(fd_, &rq, 1000) < 0) {
    cLOG_ERROR << "failed to send req, ctrl code = 0x38, error = " << strerror(errno);
    return;
  }


  LeAdvertisingEnable enable;
  enable.enable = 0x01;
  rq.ogf = OGF_LE_CTL;
  rq.ocf = 0x39;
  rq.cparam = &enable;
  rq.clen = 6;
  rq.rparam = &status;
  rq.rlen = 1;
  if (hci_send_req(fd_, &rq, 1000) < 0) {
    cLOG_ERROR << "failed to send req when enable LE ext adv, error = " << strerror(errno);
    return;
  }

  LeAdvertisingExt adv;
  adv.serviceData.uuid[0] = service & 0xff;
  adv.serviceData.uuid[1] = (service >> 8) & 0xff;
  memcpy(adv.serviceData.data, data, 20);

  rq.ocf = 0x37;
  rq.cparam = &adv;
  rq.clen = 31;
  rq.rparam = &status;
  rq.rlen = 1;

  if (hci_send_req(fd_, &rq, 1000) < 0) {
    cLOG_ERROR << "failed to send req when set LE ext adv data, error = " << strerror(errno);
    return;
  }
}

std::string HciHelper::getMacAddress() {
  struct hci_dev_info dev_info;
  memset(&dev_info, 0, sizeof(dev_info));
  dev_info.dev_id = dev_;
  if (hci_devinfo(dev_, &dev_info) < 0) {
    cLOG_ERROR << "failed to get device info, error: " << strerror(errno);
    return "00:00:00:00:00:00";
  }

  char addr[18] = { 0 };
  ba2str(&dev_info.bdaddr, addr);
  return addr;
}

int HciHelper::getPeerCount() {
  hci_conn_info *conn_info;
  hci_conn_list_req* conn_list = (hci_conn_list_req*) malloc(10 * (sizeof(hci_conn_info)) + sizeof(hci_conn_info));
  if (!conn_list) {
    cLOG_FATAL << "failed to allocate memory for hci conn list";
    exit(0);
  }

  conn_list->dev_id = dev_;
  conn_list->conn_num = 10;
  conn_info = conn_list->conn_info;

  if (ioctl(fd_, HCIGETCONNLIST, (void *) conn_list)) {
    cLOG_FATAL << "Failed to get connection list";
    exit(0);
  }

  int num = conn_list->conn_num;
  uint16_t handle = conn_list->conn_info[0].handle;
  if (num > 0) {
    if (handle_ != handle) {
      handle_ = handle;
      cLOG_INFO << "current handle is " << handle_;
    }
  } else {
    handle_ = 0;
  }

  return num;
}

void HciHelper::leReadRemoteFeatures() {
  if (handle_ == 0) return;
  uint8_t features[128];
  if (hci_le_read_remote_features(fd_, handle_, features, 1000) < 0) {
    cLOG_ERROR << "failed to read remote features, error = " << strerror(errno);
  }
}

void HciHelper::leReadLocalFeatures() {
  struct hci_request rq;
  uint8_t status;
  rq.ogf = OGF_LE_CTL;
  rq.ocf = OCF_LE_READ_LOCAL_SUPPORTED_FEATURES;
  rq.clen = 0;
  rq.cparam = NULL;
  rq.rparam = &status;
  rq.rlen = 1;
  if (hci_send_req(fd_, &rq, 1000) < 0) {
    cLOG_ERROR << "failed to send req for getting le features, error = " << strerror(errno);
  }
}

int8_t HciHelper::getRssi() {
  int8_t rssi = -100;
  if (handle_ == 0) return rssi;
  if (hci_read_rssi(fd_, handle_, &rssi, 1000) < 0) {
    cLOG_ERROR << "failed to read rssi, error = " << strerror(errno);
  } else {
    cLOG_INFO << "current connection rssi = " << rssi;
  }
  return rssi;
}

void HciHelper::disconnect() {
  getPeerCount();
  if (handle_ == 0) return;
  if (hci_disconnect(fd_, handle_, HCI_OE_USER_ENDED_CONNECTION, 1000) < 0) {
    cLOG_ERROR << "failed to disconnect, error = " << strerror(errno);
    exit(0);
  }
  cLOG_INFO << "disconnect success, handle is " << handle_;
  handle_ = 0;
}

void HciHelper::enableLeScan() {
  struct hci_request rq;
  uint8_t status;
  rq.ogf = OGF_LE_CTL;
  rq.ocf = OCF_LE_SET_SCAN_PARAMETERS;
  rq.clen = LE_SET_SCAN_PARAMETERS_CP_SIZE;
  le_set_scan_parameters_cp scan_params;
  memset(&scan_params, 0, sizeof(scan_params));
  scan_params.type = 0x00;               // Passive scanning
  scan_params.interval = htobs(0x0020);  // 20ms
  scan_params.window = htobs(0x0020);    // 20ms
  scan_params.own_bdaddr_type = 0x00;    // Public Device Address
  scan_params.filter = 0;                // accept all

  rq.cparam = &scan_params;
  rq.rparam = &status;
  rq.rlen = 1;
  if (hci_send_req(fd_, &rq, 1000) < 0) {
    cLOG_FATAL << "failed to send req, type is set scan params, error = " << strerror(errno);
    exit(0);
  }

  rq.ocf = OCF_LE_SET_SCAN_ENABLE;
  rq.clen = LE_SET_SCAN_ENABLE_CP_SIZE;
  le_set_scan_enable_cp enable;
  memset(&enable, 0, sizeof(enable));
  enable.enable = 0x01;
  rq.cparam = &enable;
  rq.rparam = &status;
  rq.rlen = 1;
  if (hci_send_req(fd_, &rq, 1000) < 0) {
    cLOG_FATAL << "failed to send req, type is enable scan, error = " << strerror(errno);
    exit(0);
  }
}

void HciHelper::filterLeMetaEvent() {
  struct hci_filter nf;
  hci_filter_clear(&nf);
  hci_filter_set_ptype(HCI_EVENT_PKT, &nf);
  hci_filter_set_event(EVT_LE_META_EVENT, &nf);

  if (setsockopt(fd_, SOL_HCI, HCI_FILTER, &nf, sizeof(nf)) < 0) {
    cLOG_FATAL << "Could not set socket options\n";
    exit(0);
  }
}