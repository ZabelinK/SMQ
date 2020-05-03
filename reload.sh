#!/bin/sh

sudo rmmod smq
make clean
make
sudo insmod smq.ko
tail -f /var/log/kern.log
