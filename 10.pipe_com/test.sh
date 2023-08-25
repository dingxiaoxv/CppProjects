#!/bin/bash

FIFO_NAME="/tmp/my_fifo"

# 写入数据到管道
signal=-70
freq=244
SSID=dreamerobot_mower
cnt=0
while [ $cnt -lt 10 ]
do
  ITEM=${cnt}
  echo "{\"type\":\"test\",\"cmd\":\"report_test_status\",\"item\":${ITEM},\"signal\":$signal,\"freq\":$freq,\"SSID\":\"$SSID\",\"result\":\"ok\"}" > "$FIFO_NAME"
  let cnt++
  sleep 1
done

