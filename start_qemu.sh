#!/bin/sh

make

echo "Setting up network interfaces for qemu..."
sudo ./qemu-ifup.sh
echo "Done."

qemu-system-arm \
	-M versatilepb \
	-m 128M \
	-nographic \
	-kernel ninjastorms \
	-device e1000,netdev=net0 \
	-netdev tap,id=net0,ifname=tap0,script=no,downscript=no \
	-object filter-dump,id=f1,netdev=net0,file=net_dump.dat

echo "Qemu stopped. Removing network interfaces ..."
sudo ./qemu-ifdown.sh
echo "Dishes are clean again. Good bye!"
