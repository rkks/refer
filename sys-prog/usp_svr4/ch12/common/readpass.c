#include <termios.h>
#include <signal.h>
#include <stdio.h>

int
main(void)
{
    char line[BUFSIZ];
    sigset_t sig, savesig;
    struct termios modes, savemodes;

    /*
     * Block keyboard signals.
     */
    sigemptyset(&sig);
    sigaddset(&sig, SIGINT);
    sigaddset(&sig, SIGQUIT);
    sigaddset(&sig, SIGTSTP);
    sigprocmask(SIG_BLOCK, &sig, &savesig);

    /*
     * Get current terminal attributes.
     */
    if (tcgetattr(0, &modes) < 0) {
        perror("tcgetattr");
        exit(1);
    }

    /*
     * Save a copy of them to restore later, and then
     * change the attributes to remove echo.
     */
    savemodes = modes;
    modes.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHOKE);

    /*
     * Make our changes take effect.
     */
    if (tcsetattr(0, TCSAFLUSH, &modes) < 0) {
        perror("tcsetattr");
        exit(1);
    }

    /*
     * Prompt for and read a line.
     */
    printf("Enter a line (will not echo): ");
    fgets(line, sizeof(line), stdin);
    line[strlen(line)-1] = '\0';
    putchar('\n');

    /*
     * Restore original terminal attributes.
     */
    if (tcsetattr(0, TCSAFLUSH, &savemodes) < 0) {
        perror("tcsetattr");
        exit(1);
    }

    /*
     * Restore original signal mask.
     */
    sigprocmask(SIG_SETMASK, &savesig, (sigset_t *) 0);

    /*
     * Print out what the user typed.
     */
    printf("You entered \"%s\"\n", line);
    exit(0);
}
