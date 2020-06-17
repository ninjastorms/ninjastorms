make
qemu-system-arm \
	-M versatilepb \
	-m 128M \
	-nographic \
	-kernel ninjastorms \
	-device e1000,netdev=net0 \
	-netdev user,id=net0 \
	-object filter-dump,id=f1,netdev=net0,file=net_dump.dat
