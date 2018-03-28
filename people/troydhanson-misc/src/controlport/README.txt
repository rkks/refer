Control Port
============

Administrative control port for a running application
------------------------------------------------------
The code in controlport.c implements a control port which can be added to an
application. When initialized, it creates a UNIX domain listener which is 
serviced by a command dispatcher. It gives a running application an interactive  
administrative console. However, the commands that are provided by the control
port are entirely up to you. Typically you would have commands that reflect or
change the application state.

This document is a stub. For now please see sample applications:

 * app1.c
 * app2.c
 * app3.c (app3 must be run as root since it uses chroot)

To connect to the running application, use `cpcli`, the control port client.

Note that, in a real application, the control port should be serviced by a 
dedicated thread. The application's "real work" would be done in other
threads. These sample applications are single threaded so they demonstrate
a control port application that does nothing else.


// vim: set tw=80 wm=2 aw syntax=asciidoc: 
