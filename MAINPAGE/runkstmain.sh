#!/bin/bash
 COUNT=$(ps -ef |grep KSTMAIN | grep -v "grep" |wc -l)
echo $COUNT
if [ $COUNT -eq 0 ]; then 
./KSTMAIN &
else 
echo is RUN 
fi
