#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cnr/logging.h>
#include "bluez/bluetooth.h"
#include "bluez/l2cap.h"
#include "gattclient.h"

bdaddr_t any_bdaddr = {0, 0, 0, 0, 0, 0};
#define BDADDR_ANY (&any_bdaddr)

static void shutdown(int no) {
  (void)no;
  exit(0);
}

int main(int argc, char **argv) {
  signal(SIGINT, shutdown);

  if (argc != 2) {
    printf("Usage: %s <Bluetooth MAC address>\n", argv[0]);
    exit(1);
  }

  bdaddr_t src_addr, dst_addr;
  uint16_t mtu = 512;
  if (str2ba(argv[1], &dst_addr) < 0) {
    cLOG_ERROR << "Invalid remote address";
    return EXIT_FAILURE;
  }
  bacpy(&src_addr, BDADDR_ANY);

  while (true) {
    mainloop_init();
    dm::BleClient client(&src_addr, &dst_addr, mtu);
    if (!client.connectionEstablished()) {
      continue;
    }

    mainloop_run();
  }

  return 0;
}
