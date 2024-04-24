#!/bash/bin
echo $(df -h /dev/mmcblk0p7 | awk '{print $4}') |sed 's/Avail//g'|sed 's/ //g' > ./shellcode/GetDisk_1.txt

