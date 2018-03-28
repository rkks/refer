gcc -g host.c elem.c server.c main.c file.c socket.c die.c -DMASTER -o master && gcc -g host.c elem.c server.c main.c file.c socket.c die.c -DSLAVE -o slave 
