/*
 *  procsmess.c
 *
 *  A completely meaningless example for tasks.
 *  The parent process forks, then wait() for output
 *  the exit status of who exited. The child forks
 *  and does the same with waitpid(). The grandchild
 *  finally forks and suddendly terminate.
 *
 *
 *  Created by Mij <mij@bitchx.it> on Wed Dec 31 2003.
 *  Original source file available on http://mij.oltrelinux.com/devel/unixprg/
 */

 /* for printf() and perror() */
#include <stdio.h>
 /* for fork() */
#include <sys/types.h>
#include <unistd.h>
 /* for wait*() */
#include <sys/wait.h>

int main() {
    pid_t mypid, childpid;
    int status;
    
     /* what's our pid? */
    mypid = getpid();
    printf("Hi. I'm the parent process. My pid is %d.\n", mypid);
    
     /* create the child */
    childpid = fork();
    if ( childpid == -1 ) {
        perror("Cannot proceed. fork() error");
        return 1;
    }
    
    
    if (childpid  == 0) {
         /* then we're the child process "Child 1" */
        printf("Child 1: I inherited my parent's pid as %d.\n", mypid);
        
         /* get our pid: notice that this doesn't touch the value of parent's "mypid" value */
        mypid = getpid();
        printf("Child 1: getppid() tells my parent is %d. My own pid instead is %d.\n", getppid(), mypid);
        
        
         /* forks another child */
        childpid = fork();
        if ( childpid == -1 ) {
            perror("Cannot proceed. fork() error");
            return 1;
        }
        
        if (childpid == 0) {
             /* this is the child of the first child, thus "Child 2" */
            printf("Child 2: I hinerited my parent's PID as %d.\n", mypid);
            
            mypid = getpid();
            printf("Child 2: getppid() tells my parent is %d. My own pid instead is %d.\n", getppid(), mypid);
            
            childpid = fork();
            if ( childpid == -1 ) {
                perror("Cannot proceed. fork() error");
                return 1;
            }
            
            if (childpid == 0) {
                 /* "Child 3" sleeps 30 seconds then terminates 12, hopefully before its parent "Child 2" */
                printf("Child 3: I hinerited my parent's PID as %d.\n", mypid);
                
                mypid = getpid();
                printf("Child 3: getppid() tells my parent is %d. My own pid instead is %d.\n", getppid(), mypid);
                
                sleep(30);
                return 12;
            } else   /* the parent "Child 2" suddendly returns 15 */  return 15;
        } else {
             /* this is still "Child 1", which waits for its child to exit */
            while ( waitpid(childpid, &status, WNOHANG) == 0 ) sleep(1);
            
            if ( WIFEXITED(status) ) printf("Child1: Child 2 exited with exit status %d.\n", WEXITSTATUS(status));
            else printf("Child 1: child has not terminated correctly.\n");
        }
    } else {
         /* then we're the parent process, "Parent" */
        printf("Parent: fork() went ok. My child's PID is %d\n", childpid);
         
         /* wait for the child to terminate and report about that */
        wait(&status);
        
        if ( WIFEXITED(status) ) printf("Parent: child has exited with status %d.\n", WEXITSTATUS(status));
        else printf("Parent: child has not terminated normally.\n");
    }
    
    return 0;
}
