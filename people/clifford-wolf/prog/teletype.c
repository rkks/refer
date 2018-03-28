/*
 * Copyright (C) 2008 Clifford Wolf
 *
 * This program is based on Ivan Tikhonov's tinyserial project.
 * http://tinyserial.sourceforge.net
 *
 * ----------------------------------------------------------------------
 *
 * Usage: teletype device [speed] [escape-letter] [command(s)]
 *
 * Default speed is 9600 Baud.
 * Default escape letter is 'A'.
 *   
 * Control Key Kombos:
 *
 *   Ctrl-A A       Send a Ctrl-A over the line
 *   Ctrl-A B       Send a break over the line
 *   Ctrl-A 0       Send a a single zero byte over the line
 *   Ctrl-A H       Print help message (command overview)
 *   Ctrl-A R       Toggle RTS (Ready To Send) signal
 *   Ctrl-A D       Toggle DTR (Data Terminal Ready) signal
 *   Ctrl-A T       Toggle timestamp-mode and reset timer
 *   Ctrl-A L       Set slow send mode (N chars/sec, 0=unlimited)
 *   Ctrl-A N       Create network link to remote TCP/IP port
 *   Ctrl-A E       Toggle local terminal echo
 *   Ctrl-A C       Run a chat script (or any other command such as 'sx')
 *   Ctrl-A W       Wait one second (this is usefull for scripts)
 *   Ctrl-A U       Set baud rate (use with care!)
 *   Ctrl-A S       Print line status
 *   Ctrl-A \       Exit without serial reset
 *   Ctrl-A X       Exit with serial reset
 *
 *   Ctrl-A . [x]   Change escape key from Ctrl-A to Ctrl-[x]
 *
 * Commands are in the form of '-<Key><Argument>'. E.g.:
 * teletype /dev/tts 9600 -b -l10 -c'chat -f myscript.txt' -l0 -u115200 -c'sx datafile.bin' -x
 *
 * ----------------------------------------------------------------------
 *
 * Ivan Tikhonov, http://www.brokestream.com, kefeer@brokestream.com
 * Patches by Jim Kou (jimkou@malico.com.tw) and Henry Nestler
 *
 * Copyright (C) 2007 Ivan Tikhonov
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * Ivan Tikhonov, kefeer@brokestream.com
 *
 */

#define _GNU_SOURCE

#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>

typedef struct { char *name; int flag; } speed_spec;

speed_spec speeds[] =
{
	{"1200", B1200},
	{"2400", B2400},
	{"4800", B4800},
	{"9600", B9600},
	{"19200", B19200},
	{"38400", B38400},
	{"57600", B57600},
	{"115200", B115200},
	{NULL, 0}
};

struct termios newtio;

char escape_letter = 'A';

int timestamp_mode = 0;
struct timeval timestamp_start;

int chat_ptmx = -1;
int chat_autoexit = 0;

int skip_serial_reset = 0;
int slow_send = 0;
int local_echo = 0;
int comfd;

int net_fd;
struct sockaddr_in net_peer;
int net_active;

void print_status(int fd)
{
	unsigned int arg;
	ioctl(fd, TIOCMGET, &arg);
	fprintf(stderr, "[STATUS]:");
	if (arg & TIOCM_RTS) fprintf(stderr, " RTS");
	if (arg & TIOCM_CTS) fprintf(stderr, " CTS");
	if (arg & TIOCM_DSR) fprintf(stderr, " DSR");
	if (arg & TIOCM_CAR) fprintf(stderr, " DCD");
	if (arg & TIOCM_DTR) fprintf(stderr, " DTR");
	if (arg & TIOCM_RNG) fprintf(stderr, " RI");
	fprintf(stderr, "\r\n");
}

void print_help(int command_line_usage)
{
	fprintf(stderr, "\r\n");
	fprintf(stderr, "---------------------------------------------------------\r\n");
	fprintf(stderr, "\r\n");
	if (command_line_usage) {
		fprintf(stderr, "Usage: teletype device [speed] [escape-letter] [command(s)]\r\n");
		fprintf(stderr, "\r\n");
		fprintf(stderr, "Default speed is 9600 Baud.\r\n");
		fprintf(stderr, "Default escape letter is 'A'.\r\n");
		fprintf(stderr, "\r\n");
		fprintf(stderr, "Supported speed rates:\r\n");
		speed_spec *s;
		for (s = speeds; s->name; s++) {
			fprintf(stderr, "%s%s", s == speeds ? "  " : ", ", s->name);
		}
		fprintf(stderr, "\r\n\r\n");
	}
	fprintf(stderr, "Control Key Kombos:\r\n");
	fprintf(stderr, "\r\n");
	fprintf(stderr, "  Ctrl-%c A       Send a Ctrl-%c over the line\r\n", escape_letter, escape_letter);
	fprintf(stderr, "  Ctrl-%c B       Send a break over the line\r\n", escape_letter);
	fprintf(stderr, "  Ctrl-%c 0       Send a a single zero byte over the line\r\n", escape_letter);
	fprintf(stderr, "  Ctrl-%c H       Print help message (command overview)\r\n", escape_letter);
	fprintf(stderr, "  Ctrl-%c R       Toggle RTS (Ready To Send) signal\r\n", escape_letter);
	fprintf(stderr, "  Ctrl-%c D       Toggle DTR (Data Terminal Ready) signal\r\n", escape_letter);
	fprintf(stderr, "  Ctrl-%c T       Toggle timestamp-mode and reset timer\r\n", escape_letter);
	fprintf(stderr, "  Ctrl-%c L       Set slow send mode (N chars/sec, 0=unlimited)\r\n", escape_letter);
	fprintf(stderr, "  Ctrl-%c N       Create network link to remote TCP/IP port\r\n", escape_letter);
	fprintf(stderr, "  Ctrl-%c E       Toggle local terminal echo\r\n", escape_letter);
	fprintf(stderr, "  Ctrl-%c C       Run a chat script (or any other command such as 'sx')\r\n", escape_letter);
	fprintf(stderr, "  Ctrl-%c W       Wait one second (this is usefull for scripts)\r\n", escape_letter);
	fprintf(stderr, "  Ctrl-%c U       Set baud rate (use with care!)\r\n", escape_letter);
	fprintf(stderr, "  Ctrl-%c S       Print line status\r\n", escape_letter);
	fprintf(stderr, "  Ctrl-%c \\       Exit without serial reset\r\n", escape_letter);
	fprintf(stderr, "  Ctrl-%c X       Exit with serial reset\r\n", escape_letter);
	fprintf(stderr, "\r\n");
	fprintf(stderr, "  Ctrl-%c . [x]   Change escape key from Ctrl-%c to Ctrl-[x]\r\n", escape_letter, escape_letter);
	fprintf(stderr, "\r\n");
	if (command_line_usage) {
		fprintf(stderr, "Commands are in the form of '-<Key><Argument>'. E.g.:\r\n");
		fprintf(stderr, "teletype /dev/tts 9600 -b -l10 -c'chat -f myscript.txt' -l0 -u115200 -c'sx datafile.bin' -x\r\n");
		fprintf(stderr, "\r\n");
	}
	fprintf(stderr, "---------------------------------------------------------\r\n");
	fprintf(stderr, "\r\n");
}

void chat_close()
{
	if (chat_ptmx != -1) {
		close(chat_ptmx);
		chat_ptmx = -1;
	}
}

void chat_open(const char *command_string)
{
	chat_close();

	fprintf(stderr, "** CONTROL TURNED OVER TO CHAT SCRIPT (Ctrl-C = Abort) **\r\n");

	chat_ptmx = posix_openpt(O_RDWR|O_NOCTTY);

	grantpt(chat_ptmx);
	unlockpt(chat_ptmx);

	if (!fork())
	{
		close(0);
		close(1);

		open(ptsname(chat_ptmx), O_RDWR);
		dup(0);

		execl("/bin/sh", "sh", "-c", command_string, NULL);
		_exit(-1);
	}
}

float tv_diff(struct timeval *a, struct timeval *b)
{
	float diff = 0;
	if (a->tv_sec < b->tv_sec)
		diff = ((1000000 - a->tv_usec) + b->tv_usec) / 1000000.0 + (b->tv_sec - (a->tv_sec + 1));
	else
		diff = (b->tv_usec - a->tv_usec) / 1000000.0;
	return diff;
}

int user_input(int from, char *prompt, char *line, int n, char *argument)
{
	if (argument) {
		snprintf(line, n, "%s", argument);
		fprintf(stderr, "\r\n%s: %s\r\n", prompt, line);
		return 0;
	}

	char c;
	int i = strlen(line);

	fprintf(stderr, "\r\n%s: %s", prompt, line);

	while (1)
	{
		if (read(from, &c, 1) != 1)
			return 1;
		if (c == '\b' || c == 127) {
			if (i > 0) {
				fprintf(stderr, "\b \b");
				line[--i] = 0;
			}
		} else if (c == 21 /* NAK */) {
			while (i > 0) {
				fprintf(stderr, "\b \b");
				line[--i] = 0;
			}
		} else if (c == '\r' || c == '\n') {
			fprintf(stderr, "\r\n");
			return 0;
		} else if (c >= ' ' && i < n-1) {
			fputc(c, stderr);
			line[i++] = c;
			line[i] = 0;
		}
	}
}

int transfer_byte(int from, int to, int is_control, int command, char *argument)
{
	char c;
	static int got_escape = 0;
	char escape_c = escape_letter - 'A' + 1;
	if (command || read(from, &c, 1) == 1)
	{
		if (command || is_control) {
			if (command || got_escape) {
				if (command)
					c = command;
				got_escape = 0;
				if (c == 'b' || c == 'B') {
					tcsendbreak(to, 0);
					return 0;
				}
				if (c == '0') {
					c = 0;
					goto send_byte;
				}
				if (c == 'h' || c == 'H') {
					print_help(0);
					return 0;
				}
				if (c == 'r' || c == 'R') {
					unsigned int arg;
					ioctl(to, TIOCMGET, &arg);
					arg ^= TIOCM_RTS;
					ioctl(to, TIOCMSET, &arg);
					return 0;
				}
				if (c == 'd' || c == 'D') {
					unsigned int arg;
					ioctl(to, TIOCMGET, &arg);
					arg ^= TIOCM_DTR;
					ioctl(to, TIOCMSET, &arg);
					return 0;
				}
				if (c == 't' || c == 'T') {
					gettimeofday(&timestamp_start, NULL);
					timestamp_mode = !timestamp_mode;
					return 0;
				}
				if (c == 'l' || c == 'L') {
					static char ratestring[4096] = "";

					if (user_input(from, "Enter rate in chars/second (0=unlimited)", ratestring, 4096, argument))
						goto read_error;

					slow_send = atoi(ratestring);

					return 0;
				}
				if (c == 'n' || c == 'N') {
					static char connstring[4096] = "";

					if (user_input(from, "Enter remote port (IP-ADDR:PORT)", connstring, 4096, argument))
						goto read_error;

					char *ips = strtok(connstring, ":");
					char *ports = strtok(NULL, "");

					if (net_active) {
						close(net_fd);
						net_active = 0;
					}

					if (ips && ports) {
						bzero(&net_peer, sizeof(net_peer));
						net_peer.sin_family = AF_INET;
						net_peer.sin_addr.s_addr = inet_addr(ips);
						net_peer.sin_port = htons(atoi(ports));
						net_fd = socket(PF_INET, SOCK_STREAM, 0);
						connect(net_fd, (struct sockaddr*)&net_peer, sizeof(net_peer));
						net_active = 1;
					}

					return 0;
				}
				if (c == 'e' || c == 'E') {
					local_echo = !local_echo;
					return 0;
				}
				if (c == 's' || c == 'S') {
					print_status(to);
					return 0;
				}
				if (c == 'c' || c == 'C') {
					static char command[4096] = "";

					if (user_input(from, "Enter chat script command", command, 4096, argument))
						goto read_error;

					if (command[0])
						chat_open(command);

					return 0;
				}
				if (c == 'w' || c == 'W') {
					sleep(1);
					return 0;
				}
				if (c == 'u' || c == 'U') {
					static char baud_rate[4096] = "";

					if (user_input(from, "Enter new baud rate", baud_rate, 4096, argument))
						goto read_error;

					speed_spec *s;
					for (s = speeds; s->name; s++) {
						if(strcmp(s->name, baud_rate) == 0) {
							newtio.c_cflag = s->flag | CS8 | CREAD;
							tcflush(to, TCIFLUSH);
							tcsetattr(to, TCSANOW, &newtio);
							break;
						}
					}

					return 0;
				}
				if (c == '\\') {
					fprintf(stderr, "\r\nBye (without reset).\r\n");
					skip_serial_reset = 1;
					return -1;
				}
				if (c == 'x' || c == 'X') {
					fprintf(stderr, "\r\nBye.\r\n");
					return -1;
				}
				if (c == '.') {
					fprintf(stderr, "\r\nType new escape letter: ");
					if (read(from, &c, 1) != 1)
						goto read_error;
					if (c >= 'a' && c <= 'z')
						c -= 'a' - 'A';
					if (c >= 'A' && c <= 'Z') {
						escape_letter = c;
						fprintf(stderr, "%c\r\nType [Ctrl-%c H] for help\r\n", escape_letter, escape_letter);
					} else {
						fprintf(stderr, "%c\r\nThe character you typed is not a letter!\r\n", c);
					}
					return 0;
				}
				if (write(to, &escape_c, 1) != 1) {
					perror("write failed");
					return -1;
				}
				if (c == 'a' || c == 'A')
					return 0;
			}
			if (c == escape_c) {
				got_escape = 1;
				return 0;
			}
		}

		if (!is_control && timestamp_mode)
		{
			static int got_nl_cr = 0;

			if (c == '\n' || c == '\r') {
				got_nl_cr = 1;
			} else
			if (got_nl_cr) {
				struct timeval tv;
				gettimeofday(&tv, NULL);
				fprintf(stdout, "<%7.3fs> ", tv_diff(&timestamp_start, &tv));
				fflush(stdout);
				got_nl_cr = 0;
			}
		}

send_byte:
		if (slow_send && to != STDOUT_FILENO) {
			usleep(1000000 / slow_send);
		}
		if (to == STDOUT_FILENO && chat_ptmx != -1) {
			write(chat_ptmx, &c, 1);
		}
		if (to == STDOUT_FILENO && net_active) {
			write(net_fd, &c, 1);
		}
		if (to == comfd && local_echo) {
			write(STDOUT_FILENO, &c, 1);
			if (c == '\r')
				write(STDOUT_FILENO, "\n", 1);
		}
		if (write(to, &c, 1) != 1) {
			perror("write failed");
			return -2;
		}
	}
	else
	{
read_error:
		if (from == chat_ptmx && chat_ptmx != -1) {
			chat_close();
			int status;
			if (wait(&status) >= 0 && WEXITSTATUS(status) != 0) {
				fprintf(stderr, "\r\n** CHAT SCRIPT RETURNED ERROR CODE %d! **\r\n", WEXITSTATUS(status));
				return -1;
			}
			fprintf(stderr, "\r\n** END OF CHAT SCRIPT **\r\n");
			return chat_autoexit;
		}
		fprintf(stderr, "nothing to read. probably port disconnected.\n");
		return -2;
	}
	return 0;
}

int main(int argc, char *argv[])
{
	struct termios oldtio;
	struct termios oldkey_stdin, oldkey_stdout, newkey;
	char *devicename = argv[1];
	int need_exit = 0;
	int speed = B9600;

	if (argc < 2)
	{
		print_help(1);
		exit(1);
	}

	comfd = open(devicename, O_RDWR | O_NOCTTY | O_NONBLOCK);
	if (comfd < 0)
	{
		perror(devicename);
		exit(-1);
	}

	while (argc > 2)
	{	
		speed_spec *s;
		for (s = speeds; s->name; s++) {
			if(strcmp(s->name, argv[2]) == 0) {
				speed = s->flag;
				break;
			}
		}
		if (s->name) {
			argc--; argv++;
			continue;
		}

		if (argv[2][0] >= 'A' && argv[2][0] <= 'Z' && !argv[2][1]) {
			escape_letter = argv[2][0];
			argc--; argv++;
			continue;
		}

		if (argv[2][0] >= 'a' && argv[2][0] <= 'z' && !argv[2][1]) {
			escape_letter = argv[2][0] - ('a' - 'A');
			argc--; argv++;
			continue;
		}

		// if ((argv[2][0] < '0' || argv[2][0] > '9') && argv[2][1]) {
		// 	chat_open(argv[2]);
		// 	chat_autoexit = 1;
		// 	argc--; argv++;
		// 	continue;
		// }

		if (argv[2][0] == '-')
			break;

		print_help(1);
		exit(1);
	}

	tcgetattr(STDIN_FILENO, &oldkey_stdin);
	tcgetattr(STDOUT_FILENO, &oldkey_stdout);
	memset(&newkey, 0, sizeof(newkey));
	newkey.c_cflag = B9600 | CRTSCTS | CS8 | CLOCAL | CREAD;
	newkey.c_iflag = IGNPAR;
	newkey.c_oflag = 0;
	newkey.c_lflag = 0;
	newkey.c_cc[VMIN]=1;
	newkey.c_cc[VTIME]=0;
	tcflush(STDIN_FILENO, TCIFLUSH);
	tcflush(STDOUT_FILENO, TCIFLUSH);
	tcsetattr(STDIN_FILENO, TCSANOW, &newkey);
	tcsetattr(STDOUT_FILENO, TCSANOW, &newkey);

	tcgetattr(comfd, &oldtio);
	newtio.c_cflag = speed | CS8 | CREAD;
	newtio.c_iflag = IGNPAR;
	newtio.c_oflag = 0;
	newtio.c_lflag = 0;
	newtio.c_cc[VMIN]=1;
	newtio.c_cc[VTIME]=0;
	tcflush(comfd, TCIFLUSH);
	tcsetattr(comfd, TCSANOW, &newtio);

	if (!chat_autoexit) {
		fprintf(stderr, "Type [Ctrl-%c H] for help\r\n", escape_letter);
		print_status(comfd);
	}

	while (!need_exit)
	{
		fd_set fds;
		int max_fd = 0;
		int ret;
		
		FD_ZERO(&fds);

		if (chat_ptmx != -1) {
			FD_SET(chat_ptmx, &fds);
			if (chat_ptmx >= max_fd)
				max_fd = chat_ptmx+1;
		}

		FD_SET(STDIN_FILENO, &fds);
		if (STDIN_FILENO >= max_fd)
			max_fd = STDIN_FILENO+1;

		FD_SET(comfd, &fds);
		if (comfd >= max_fd)
			max_fd = comfd+1;

		if (net_active) {
			FD_SET(net_fd, &fds);
			if (net_fd >= max_fd)
				max_fd = net_fd+1;
		}

		if (argc > 2 && chat_ptmx == -1) {
			struct timeval timeout = { 0, 100000 };
			ret = select(max_fd, &fds, NULL, NULL, &timeout);
		} else {
			ret = select(max_fd, &fds, NULL, NULL, NULL);
		}
		if (ret == -1) {
			perror("select");
			need_exit = -2;
		} else if (ret > 0) {
			if (FD_ISSET(comfd, &fds)) {
				need_exit = transfer_byte(comfd, STDOUT_FILENO, 0, 0, NULL);
			} else if (FD_ISSET(STDIN_FILENO, &fds)) {
				if (argc > 2) {
					unsigned char ch;
					if (read(STDIN_FILENO, &ch, 1) == 1 && ch == 3) {
						fprintf(stderr, "\r\n** COMMANDS ABORT **\r\n");
						need_exit = -3;
					}
				} else if (chat_ptmx != -1) {
					unsigned char ch;
					if (read(STDIN_FILENO, &ch, 1) == 1 && ch == 3) {
						fprintf(stderr, "\r\n** CHAT SCRIPT ABORT **\r\n");
						chat_close();
						if (chat_autoexit) {
							fprintf(stderr, "Type [Ctrl-%c H] for help\r\n", escape_letter);
							chat_autoexit = 0;
						}
					}
				} else
					need_exit = transfer_byte(STDIN_FILENO, comfd, 1, 0, NULL);
			} else if (chat_ptmx != -1 && FD_ISSET(chat_ptmx, &fds)) {
				need_exit = transfer_byte(chat_ptmx, comfd, 0, 0, NULL);
			} else if (net_active && FD_ISSET(net_fd, &fds)) {
				need_exit = transfer_byte(net_fd, comfd, 0, 0, NULL);
			}
		} else if (argc > 2 && chat_ptmx == -1) {
			if (argv[2][0] == '-' && argv[2][1]) {
				fprintf(stderr, "\r\n** COMMAND: %s **\r\n", &argv[2][1]);
				need_exit = transfer_byte(STDIN_FILENO, comfd, 1, argv[2][1], &argv[2][2]);
				argc--; argv++;
			} else {
				fprintf(stderr, "\r\n** COMMANDS SYNTAX ERROR **\r\n");
				need_exit = 1;
			}
		}
	}

	tcsetattr(STDIN_FILENO, TCSANOW, &oldkey_stdin);
	tcsetattr(STDOUT_FILENO, TCSANOW, &oldkey_stdout);
	if (!skip_serial_reset)
		tcsetattr(comfd, TCSANOW, &oldtio);
	close(comfd);

	return need_exit < 0 ? -need_exit : 0;
}

