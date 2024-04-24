#!/bin/bash
 COUNT=$(ps -ef |grep  WTKP|grep -v "grep" |wc -l)
if [ $COUNT -eq 0 ]; then 
cd /mnt/APP/WTKP
./WTKP &
else 
echo is RUN 
fi
