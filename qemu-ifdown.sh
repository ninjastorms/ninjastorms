#!/bin/sh

if [[ $UID != 0 ]]; then
    echo "Please run this script with sudo:"
    echo "sudo $0 $*"
    exit 1
fi

# Stop bridge
ip link set dev br0 down

# Delete bridge
ip link del br0 type bridge

# Delete tap
ip tuntap del dev tap0 mode tap
