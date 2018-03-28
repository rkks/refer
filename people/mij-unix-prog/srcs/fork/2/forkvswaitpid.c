/*
 *  forkvswaitpid.c
 *  
 *  The parent loops forking and waiting.
 *  Notice the behaviour of both the post-fork()
 *  and post-waitpid() messages.
 *
 *
 *  Created by Mij <mij@bitchx.it> on Wed Dec 31 2003.
 *  Original source file available on http://mij.oltrelinux.com/devel/unixprg/
 */

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    int status;
    
    printf("Hi. Your OS would get a bit busy right now. Please kill pressing ctrl+C\n");
    
    while (1) {
        pid = fork();
        
        if ( pid == -1 ) {
            perror("Error on fork()");
            return 1;
        }
        
        if (pid == 0) {
             /* we're the child */
            printf("One more child born.\n");
            return 0;
        } else
             /* we're the parent and we want to purge the queue of childs exited */
            while ( waitpid(-1, &status, WNOHANG) > 0 ) printf("One more child dead.\n");
    }
    
    return 0;
}
