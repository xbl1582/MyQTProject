#!/bin/bash
for((i=1;i<254;i++))do
{
 ping -c1 -W1 -q 192.168.10.$i \
 && echo 192.168.10.$i >>./shellcode/reachable.txt \
 ||echo 192.168.10.$i>>./shellcode/unreachable.txt;
}&
done
wait