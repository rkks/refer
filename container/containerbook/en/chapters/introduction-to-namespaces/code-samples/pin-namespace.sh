#!/bin/bash

# Start a new bash shell in a new network namespace
./unshare bash

# Create a file we will bind mount to
touch /tmp/my_network_ns

# Mount the network namespace of the current PID ($$) to /tmp/my_network_ns
mount --bind /proc/$$/ns/net /tmp/my_network_ns

# Exit the bash process, which would normally destroy the namespace
exit
