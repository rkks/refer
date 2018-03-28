#include <termios.h>
#include <signal.h>
#include <stdlib.h>
#include <ctype.h>

int
main(void)
{
    char c, lastc;
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
     * change the attributes to set character-at-a-time
     * input, turn off canonical mode, and turn off echo.
     */
    savemodes = modes;
    modes.c_cc[VMIN] = 1;
    modes.c_cc[VTIME] = 0;
    modes.c_lflag &= ~ICANON;
    modes.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHOKE);

    /*
     * Make our changes take effect.
     */
    if (tcsetattr(0, TCSAFLUSH, &modes) < 0) {
        perror("tcsetattr");
        exit(1);
    }

    /*
     * Read characters.
     */
    while (read(0, &c, 1) > 0) {
        /*
         * Turn uppercase to lowercase and lowercase
         * to uppercase.
         */
        if (isupper(c))
            c = tolower(c);
        else if (islower(c))
            c = toupper(c);

        /*
         * Since non-canonical mode disables EOF,
         * we need to handle it ourselves.
         */
        if (c == savemodes.c_cc[VEOF] && lastc == '\n')
            break;

        /*
         * Output the new character and save
         * it.
         */
        write(1, &c, 1);
        lastc = c;
    }

    /*
     * Restore the original terminal attributes.
     */
    if (tcsetattr(0, TCSAFLUSH, &savemodes) < 0) {
        perror("tcsetattr");
        exit(1);
    }

    /*
     * Restore the original signal mask.
     */
    sigprocmask(SIG_SETMASK, &savesig, (sigset_t *) 0);
    exit(0);
}
