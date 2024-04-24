#!/bin/bash
 COUNT=$(ps -ef |grep  KSTSCAN|grep -v "grep" |wc -l)
if [ $COUNT -eq 0 ]; then 
cd /mnt/APP/KSTSCAN
./KSTSCAN &
else 
echo is RUN 
fi
