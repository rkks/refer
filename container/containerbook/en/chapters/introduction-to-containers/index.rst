==========================
Introduction to containers
==========================

A container is an isolated area within the kernel, which is built using the
namespaces and cgroups we have covered so far in this text. By combining
several of the namespaces and cgroups, we can build very powerful isolation
mechanisms.

While namespaces and cgroups are (hopefully) tangible concepts, the container
concept is a bit more abstract. For now, it should suffice to imagine a
combination of the namespaces and cgroups already covered, and when discussing
this combination, we will call it a 'container'.

When discussing containers from a user or administrator perspective, there are
two common types of containers, the `application containers` and the `system
containers`. `Application containers` are used to launch a specific application
within a container, such as a web server, or a database server. A `system
container` on the other hand is a container which is used to launch an entire
Linux user space, such as Debian.

We will first cover application containers. We will enable all the namespaces,
and demonstrate how this gives an application a very limited view of the host
system, and we will also show two different deployment scenarios for
applications running in containers.

---------------------------------
The container controller software
---------------------------------

The container controller software is responsible for creating the necessary
namespaces, and for creating a chroot jail for the application running in the
container.

In the example below, all the namespaces are enabled, a chroot is entered in a
path supplied on the command line, and finally an executable is run inside the
container.


.. literalinclude:: code-samples/contejner.c
    :language: c
    :linenos:

----------------------------------------------
Building and deploying software in a container
----------------------------------------------

It may be tempting to just copy some binary (/bin/bash for example) into a
sysroot and attempt to launch `contejner` on that path. Unfortunately, this
will have limited success however. It is important to remember that the root
file system of the container will be fully separate from the host file systems,
and thus, while the `bash` binary will be available within the container, the
run time library dependencies will not.

On my system, these are the run time dependencies for `/bin/bash`:

.. code-block:: bash
    :linenos:

     jonte@Thinkpad:~/containerbook$ /lib64/ld-linux-x86-64.so.2 --list /bin/bash
            linux-vdso.so.1 (0x00007ffc812ad000)
            libncurses.so.5 => /lib/x86_64-linux-gnu/libncurses.so.5 (0x00007f87f3ed8000)
            libtinfo.so.5 => /lib/x86_64-linux-gnu/libtinfo.so.5 (0x00007f87f3cae000)
            libdl.so.2 => /lib/x86_64-linux-gnu/libdl.so.2 (0x00007f87f3aa9000)
            libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f87f3705000)
            /lib64/ld-linux-x86-64.so.2 (0x00005625bb3f6000)

So, in order to run `/bin/bash` within a container, we would also need to copy
these run time dependencies into the container (and also any run time
dependencies of these libraries).

For the purposes of this text, and in order to proceed a bit faster, we will
opt for a faster demonstration, which is `static linking` - meaning that we
include all the run time dependencies in the executable we are building -
thereby removing the need of copying so many files.

Consider the following simple C program:

.. literalinclude:: code-samples/helloworld.c
    :language: c
    :linenos:

And the following command to build it:

.. code-block:: c
    :linenos:

    gcc -static helloworld -o helloworld

This will produce a helloworld binary, which is statically linked, and thus can
be run as-is within a container, as such:

.. code-block:: bash
    :linenos:

    # mkdir -p /tmp/container_root
    # cp helloworld /tmp/container_root
    # sudo contejner /tmp/container_root /helloworld
    Hello world

Building and deploying BusyBox
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

While the hello world example might be impressive enough, a complete command
line environment would be more useful to have inside the container. BusyBox
[#busybox]_ is a suite of tools which is used to provide a complete command-line
environment, typically used for embedded systems, due to its small size.
BusyBox contains implementations for many of the most crucial software
typically found in Linux systems, such as :linuxman:`ls(1)`, :linuxman:`mv(1)`,
:linuxman:`cp(1)` and :linuxman:`vi(1)`.

BusyBox is highly configurable, and the set of included tools is flexible. For
our purposes, we will use the default configuration, with some minor
modifications. The reader is invited to play around with the set of included
commands to see what is available.

First, we will download the BusyBox sources:

.. code-block:: bash

    # git clone git://git.busybox.net/busybox

Then, we will make one minor modification to the BusyBox sources. BusyBox will,
per default, try to:linuxman:`setuid(2)` and :linuxman:`setgid(2)` to a
specific user ID and group ID. We could configure BusyBox to pick the right
user and group IDs in a configuration file, but in order to simplify things, we
will simply disable this feature, which also gives us a chance to show how to
configure BusyBox.

If you have ever configured a Linux kernel, the `menuconfig` utility may be
familiar. It is also used in BusyBox:

.. code-block:: bash

    # make menuconfig

    BusyBox 1.25.0.git Configuration
    -------------------------------------------------------------------------------
    +------------------------- Busybox Configuration --------------------------+
    |  Arrow keys navigate the menu.  <Enter> selects submenus --->.           |
    |  Highlighted letters are hotkeys.  Pressing <Y> includes, <N> excludes,  |
    |  <M> modularizes features.  Press <Esc><Esc> to exit, <?> for Help, </>  |
    |  for Search.  Legend: [*] built-in  [ ] excluded  <M> module  < > module |
    | +----------------------------------------------------------------------+ |
    | |    Busybox Settings  --->                                            | |
    | |--- Applets                                                           | |
    | |    Archival Utilities  --->                                          | |
    | |    Coreutils  --->                                                   | |
    | |    Console Utilities  --->                                           | |
    | |    Debian Utilities  --->                                            | |
    | |    Editors  --->                                                     | |
    | |    Finding Utilities  --->                                           | |
    | |    Init Utilities  --->                                              | |
    | |    Login/Password Management Utilities  --->                         | |
    | |    Linux Ext2 FS Progs  --->                                         | |
    | |    Linux Module Utilities  --->                                      | |
    | |    Linux System Utilities  --->                                      | |
    | |    Miscellaneous Utilities  --->                                     | |
    | |    Networking Utilities  --->                                        | |
    | |    Print Utilities  --->                                             | |
    | |    Mail Utilities  --->                                              | |
    | |    Process Utilities  --->                                           | |
    | |    Runit Utilities  --->                                             | |
    | |    Shells  --->                                                      | |
    | |    System Logging Utilities  --->                                    | |
    | |---                                                                   | |
    | |    Load an Alternate Configuration File                              | |
    | |    Save Configuration to an Alternate File                           | |
    | |                                                                      | |
    | +----------------------------------------------------------------------+ |
    +--------------------------------------------------------------------------+
    |                     <Select>    < Exit >    < Help >                     |
    +--------------------------------------------------------------------------+

The following option needs to be disabled in order to disable the uid and gid
setting. Navigate to the option as indicated by the "Location" description
using the arrow keys and the return key on your keyboard. The shift key is used
to toggle between buttons.

.. code-block:: bash

  | Symbol: FEATURE_SUID [=n]
  | Prompt: Support for SUID/SGID handling
  |   Defined at Config.in:347
  |   Location:
  |     -> Busybox Settings
  |       -> General Configuration

Once BusyBox has been configured, it can be built using `make`, and a `busybox`
binary will be produced in the top source directory. For our purposes, we would
like to produce a statically linked binary, as we did with the `helloworld`
example. Thus, the command to run looks like follows:

.. code-block:: bash

    LDFLAGS="--static" make

BusyBox consists of only one binary, and the "sub command" to use is selected
in one of two ways:

- As a command-line parameter to the `busybox` binary, for example `./busybox ls`
- By creating a symlink to the `busybox` binary, where the symlink name is the same of the sub command to run, for example: `ln -s ./busybox /bin/ls`

Here are some examples of command invocations using BusyBox:

.. code-block:: bash

    $ ./busybox ls /sys
    block       class       devices     fs          kernel      power
    bus         dev         firmware    hypervisor  module

    $ ./busybox lsusb
    Bus 001 Device 002: ID 8087:0a2a
    Bus 001 Device 003: ID 04ca:703c
    Bus 003 Device 002: ID 8087:8001
    Bus 001 Device 001: ID 1d6b:0002
    Bus 002 Device 001: ID 1d6b:0003
    Bus 003 Device 001: ID 1d6b:0002

We will, however, opt for the symlinking approach, and create symlinks for all
the commands provided by BusyBox.

The following shell snippet will create symlinks for all commands provided by
BusyBox:

.. code-block:: bash
    :linenos:

    #!/busybox ash

    /busybox --list-full | \
    while read app; do
        /busybox mkdir -p `/busybox dirname $app`
        /busybox ln -sf "/busybox" "/$app"
    done

    mkdir -p /proc /bin
    mount -t proc none /proc

Notice that we are invoking `/bin/ash`, which is the shell provided by
BusyBox. In other words, this shell script is intended to be run within the
container.

In order to set up BusyBox in a contained environment, we need to create a
directory for use as our contained root, and copy BusyBox plus the shell script
above to the new root.

.. code-block:: bash
    :linenos:

    # mkdir /tmp/contained_root
    # cp busybox /tmp/contained_root
    # cp setup.sh /tmp/contained_root
    # sudo ./contejner /tmp/contained_root /busybox ash
    # cd /
    # ./setup.sh
    # ls
    bin       busybox   linuxrc   proc      sbin      setup.sh  usr

.. [#busybox] https://www.busybox.net/
