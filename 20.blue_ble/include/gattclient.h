#ifndef DM_GATTCLIENT_H
#define DM_GATTCLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "bluez/att.h"
#include "bluez/bluetooth.h"
#include "bluez/gatt-client.h"
#include "bluez/gatt-db.h"
#include "bluez/hci.h"
#include "bluez/hci_lib.h"
#include "bluez/l2cap.h"
#include "bluez/mainloop.h"
#include "bluez/timeout.h"
#include "bluez/uuid.h"

namespace dm {

class BleClient {
public:
  BleClient(bdaddr_t* src, bdaddr_t* dst, uint16_t mtu);
  ~BleClient() { close(fd_); }
  gatt_db* db() { return db_; }
  bool     connectionEstablished() { return fd_ > 0; }

private:
  int             fd_;
  gatt_db*        db_;
  bt_att*         att_;
  bt_gatt_client* gatt_;
  int             mtuSize_;
};

}


#endif