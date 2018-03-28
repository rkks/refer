#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MSQKEY      34856
#define MSQSIZE     32

struct mymsgbuf {
    long    mtype;
    char    mtext[MSQSIZE];
};

int
main(void)
{
    key_t key;
    int n, msqid;
    struct mymsgbuf mb;

    /*
     * Create a new message queue.  We use IPC_CREAT to create it,
     * and IPC_EXCL to make sure it does not exist already.  If
     * you get an error on this, something on your system is using
     * the same key - change MSQKEY to something else.
     */
    key = MSQKEY;
    if ((msqid = msgget(key, IPC_CREAT | IPC_EXCL | 0666)) < 0) {
        perror("msgget");
        exit(1);
    }

    /*
     * Receive messages.  Messages of type 1 are to be printed
     * on the standard output; a message of type 2 indicates that
     * we're done.
     */
    while ((n = msgrcv(msqid, &mb, MSQSIZE, 0, 0)) > 0) {
        switch (mb.mtype) {
        case 1:
            write(1, mb.mtext, n);
            break;
        case 2:
            goto out;
        }
    }

out:
    /*
     * Remove the message queue from the system.
     */
    if (msgctl(msqid, IPC_RMID, (struct msqid_ds *) 0) < 0) {
        perror("msgctl");
        exit(1);
    }

    exit(0);
}
