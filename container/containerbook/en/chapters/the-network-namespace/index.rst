=================
Network namespace
=================

Network namespaces are used to separate groups of network interfaces and
network routing tables. These namespaces can be used to give some application a
limited view of the networking resources of a physical machine.

There are two tools available for working with network namespaces; the kernel
APIs and command line tools which provide an abstraction layer above the kernel
APIs.

----------------
A simple example
----------------

In this example we will create a network namespace, and connect that namespace
to the rest of the system using virtual Ethernet devices.

.. literalinclude:: code-samples/veth.sh
    :language: bash
    :linenos:

This entire example uses the `ip` tool to configure network devices within and
outside the network namespace. We start out by creating the actual namespace
itself, using `ip netns add` the `netns` sub command of `ip` tells the tool
that we are working with namespaces.

The `link` sub command is used to create a virtual Ethernet device pair. The
two endpoints are called `veth0` and `veth1`. We will put one of these devices
on either side of the namespace barrier.

Next, we configure the "external" `veth1` device with an appropriate IP
address, default route, and finally bring it online. This is very similar to
the operations you would perform on a regular Ethernet, or other network
device, in our case the device is entirely virtual however.

Now comes the interesting part, using the `ip link set veth0 netns isolated`
command, we move the veth0 device into the `isolated` network namespace. This
device is no longer reachable from outside the namespace we just created. The
connection between `veth0` and `veth1` established when creating the devices
using `ip link add` still exists however, so communication between the two
devices is still entirely possible, as we shall see next.

Using the `netns exec` sub command, we can execute arbitrary commands within
our network namespace. We now issue similar commands as those we saw for the
`veth1` device, only inside the network namespace.

It is easy to believe we can only issue `ip` related commands, but this is not
the case. To test the connectivity between veth0 and veth1 we can use the
`ping` command to send a few ICMP packets which will echo back and forth,
demonstrating we indeed have connectivity.

The final step in this example is to tear down the namespace again, leaving our
system in the same state it was in before this exercise started. Both `veth0`
and `veth1` are destroyed as a consequence of deleting the namespace. `veth0`
is destroyed since it is no longer reachable once the namespace ceases to
exist, and `veth1` is destroyed once its only endpoint is lost.

----------------------
A more complex example
----------------------

.. todo:: Here I would like to build on the last example, but connect the
    external veth1 to an external network (internet?), and I would also like to
    create some interesting routing table within the container to show that
    this table is kept separate from the rest of the system.

