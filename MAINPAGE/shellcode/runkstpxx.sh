#!/bin/bash
 COUNT=$(ps -ef |grep  KSTPXQ|grep -v "grep" |wc -l)
if [ $COUNT -eq 0 ]; then 
cd /mnt/APP/KSTPXX
./KSTPXQ &
else 
echo is RUN 
fi
