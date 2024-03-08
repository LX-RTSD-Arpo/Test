#!/bin/sh
ip link set br0 down
#ip link set eth1 up
#ip link set eth2 up
brctl delbr br0
