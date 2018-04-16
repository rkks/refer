#!/bin/bash

# Create a new network namespace called 'isolated'
ip netns add isolated

# Create a new pair of veth devices, called eth0 and veth1
ip link add veth0 type veth peer name veth1

# Set up network device outside the namespace
ip address add 10.10.10.2/24 dev veth1
ip link set veth1 up
ip route add default dev veth1

# Assign the veth0 device to the 'isolated' namespace
ip link set veth0 netns isolated

# Set up the device within the 'isolated' namespace
ip netns exec isolated ip address add 10.10.10.1/24 dev veth0
ip netns exec isolated ip link set veth0 up
ip netns exec isolated ip route add default dev veth0

# Execute a ping from within the namespace
ip netns exec isolated ping -c 3 10.10.10.2

# Clean up again
ip netns delete isolated
