=================
The IPC namespace
=================

Inter Process Communication (IPC) is used to communicate between processes.
There are currently two different types of IPC in Linux; the POSIX message
queues (:linuxman:`mq_overview(7)`), and the System V IPC mechanisms
(:linuxman:`svipc(7)`). The POSIX message queues are used to transport data
between processes in the form of `messages`. The System V IPC mechanisms
consist of message queues (different from the POSIX ones), semaphore sets,
which are used to share a group of semaphores between processes, and shared
memory segments, which are used to access a shared memory area in multiple
processes concurrently.

In order to limit access to these IPC mechanisms, the regular UNIX permissions
are typically used (user access, group access, world access). This means a
semaphore group can be created such that only the processes of the current user
has access to the group, while other users will not have access to it.

In many cases, the regular UNIX permissions work well, but in some scenarios, a
more fine-grained access control is desired, such as when one user runs two
'untrusted' processes, where these two processes must not be able to interact
using IPC. A scenario could be running an application which assumes it will be
the only provider of some IPC, and the application must be isolated in order to
run multiple instances of this application.

As you may have guessed, this fine-grained access control for IPC is
implemented using the IPC namespace. Each IPC namespace creates a separate
group of IPC mechanisms, where each group is completely isolated from the
others. Processes participating in the same namespace have access to the same
IPC resources within the namespace.

------------------
Listing namespaces
------------------

As a first example, we will try to list the available IPC channels available in
the global namespace, and once the IPC namespace has been unshared.

In the example below, the :linuxman:`ipcs(1)` command is used to list the
available IPC channels. In the output of the first invocation, we can see that
there are several "shared memory segments" mapped. These are memory areas where
several processes share a common view of a block of memory. If one process
writes to the memory, the other process(es) will see these changes, and vice
versa. This sort of primitive is very powerful for implementing communication
between processes.

.. code-block:: bash
    :linenos:

    $ sudo ipcs
    ------ Message Queues --------
    key        msqid      owner      perms      used-bytes   messages

    ------ Shared Memory Segments --------
    key        shmid      owner      perms      bytes      nattch     status
    0x00000000 1441792    jonte      600        393216     2          dest
    0x00000000 15990826   jonte      600        50688      2          dest
    0x00000000 16023595   jonte      600        72512      2          dest
    0x00000000 16678956   jonte      600        118272     2          dest
    0x00000000 18808877   jonte      600        870240     2          dest

    ------ Semaphore Arrays --------
    key        semid      owner      perms      nsems

And now, run the same command in a new IPC namespace:

.. code-block:: bash
    :linenos:

    $ sudo unshare -i ipcs
    ------ Message Queues --------
    key        msqid      owner      perms      used-bytes   messages

    ------ Shared Memory Segments --------
    key        shmid      owner      perms      bytes      nattch     status

    ------ Semaphore Arrays --------
    key        semid      owner      perms      nsems

As you may have guessed, when entering a new IPC namespace, the IPC channels
previously available become unavailable.

.. todo:: Add an example where a message is passed through a message queue
          between two processes in the same namespace, and then show what
          happens when the same code is used in different namespaces.

.. todo:: Add an example of nested IPC namespaces
