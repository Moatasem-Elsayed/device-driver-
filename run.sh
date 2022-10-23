#!/bin/bash

case $1 in 

insert)
make 
sudo insmod hello.ko
sudo  chmod 777 /dev/test_file

;;

remove)
sudo rmmod  hello
make clean
;;

test)
sudo rmmod  hello
if [ $? -ne 0 ]
then 
 echo "module not inserted yet"
 read -p "press y to continue" yes
fi
make clean
make 
if [ $? -ne 0 ]
then 
    exit -1
fi
sudo insmod hello.ko
sudo  chmod 777 /dev/test_file
# cat /dev/test_file
# dmesg
;;

esac
