#!/bin/sh

if [[ $UID != 0 ]]; then
    echo "Please run this script with sudo:"
    echo "sudo $0 $*"
    exit 1
fi

# Create tap device
ip tuntap add dev tap0 mode tap

# Create bridge
ip link add br0 type bridge
ip link set dev tap0 master br0

# Start interfaces
ip link set dev br0 up
ip link set dev tap0 up

# Assign ip to bridge
ip address add dev br0 10.0.2.15/24
