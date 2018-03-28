/*
	More-complete Web Server
	AUP2, Sec. 8.04.4 (not in book)

	Copyright 2003 by Marc J. Rochkind. All rights reserved.
	May be copied only for purposes and under conditions described
	on the Web page www.basepath.com/aup/copyright.htm.

	The Example Files are provided "as is," without any warranty;
	without even the implied warranty of merchantability or fitness
	for a particular purpose. The author and his publisher are not
	responsible for any damages, direct or incidental, resulting
	from the use or non-use of these Example Files.

	The Example Files may contain defects, and some contain deliberate
	coding mistakes that were included for educational reasons.
	You are responsible for determining if and how the Example Files
	are to be used.

*/
#include "defs.h"
#include "ssi.h"

/* rfc2616 */

/* use basename? *********************************************************/

#define WEB_ROOT		"/aup/webroot/"
#define NOTFOUND_PAGE	"/aup/notfound.htm"

#define HEADER_OK\
	"HTTP/1.1 200 OK\r\n"\
	"Server: AUP-ws\r\n"\
	"Content-Length: %ld\r\n"

#define CONTENT_TEXT\
	"Content-Type: text/html\r\n\r\n"

#define CONTENT_JPEG\
	"Content-Type: image/jpeg\r\n\r\n"

#define HEADER_NOTFOUND\
	"HTTP/1.1 404 Not Found\r\n"\
	"Server: AUP-ws\r\n"\
	"Transfer-Encoding: chunked\r\n"\
	"Content-Type: text/html\r\n\r\n"

#define HTML_NOTFOUND\
	"<!DOCTYPE html PUBLIC \"-//IETF//DTD HTML 2.0//EN\">\n"\
	"<html><head><title>Error 404</title>\n"\
	"</head><body>\n"\
	"<h2>AUP-ws server can't find page %s</h2>"\
	"</body></html>\r\n"


static void send_header_OK(SSI *ssip, off_t len, const char *path, int fd)
{
	char buf[1000], *dot;

	snprintf(buf, sizeof(buf), HEADER_OK, (long)len);
	ec_neg1(  writeall(fd, buf, strlen(buf)) )
	/* Following is extremely oversimplified */
	dot = strrchr(path, '.');
	if (dot != NULL && (strcasecmp(dot, ".jpg") == 0 ||
	  strcasecmp(dot, ".jpeg") == 0))
		ec_neg1(  writeall(fd, CONTENT_JPEG, strlen(CONTENT_JPEG)) )
	else {
		time_t tm;
		char buf[100];

		ec_neg1(  writeall(fd, CONTENT_TEXT, strlen(CONTENT_TEXT)) )
		(void)time(&tm);
		strcpy(buf, ctime(&tm));
		ec_neg1( writeall(fd, buf, strlen(buf)) )
	}
	return;

EC_CLEANUP_BGN
	EC_FLUSH("Error sending response (nonfatal)")
EC_CLEANUP_END
}

static void send_header_notfound(SSI *ssip, const char *path, int fd)
{
	char buf[2000], buf_html[1000];

	/* Bug: Should remove meta-characters from path. */
	snprintf(buf_html, sizeof(buf_html), HTML_NOTFOUND, path);
	snprintf(buf, sizeof(buf), "%s%x\r\n%s\r\n0\r\n\r\n",
	  HEADER_NOTFOUND, strlen(buf_html), buf_html);
	ec_neg1(  writeall(fd, buf, strlen(buf)) )
	return;

EC_CLEANUP_BGN
	EC_FLUSH("Error sending response (nonfatal)")
EC_CLEANUP_END
}

static bool handle_request(SSI *ssip, const char *s, int fd)
{
	char *p, buf[1000], path[FILENAME_MAX];// wrong macro
	FILE *in;
	struct stat statbuf;
	ssize_t nread, nsent, nsenttotal = 0;

	if (strncasecmp(s, "GET ", 4) == 0) {
		s += 4;
		p = strchr(s, ' ');
		if (p != NULL)
			*p = '\0';
		strcpy(path, WEB_ROOT);
		strncat(path, s, sizeof(path));
		if (stat(path, &statbuf) == 0 &&
		  (statbuf.st_mode & S_IFDIR) == S_IFDIR) {
			if (path[strlen(path) - 1] != '/')
				strncat(path, "/", sizeof(path));
			strncat(path, "index.html", sizeof(path));
		}
		if (stat(path, &statbuf) == -1 ||
		  (in = fopen(path, "rb")) == NULL) {
			send_header_notfound(ssip, s, fd);
			return false;
		}
		send_header_OK(ssip, statbuf.st_size, path, fd);
// use sendfile or whatever the fast thingy is
		while ((nread = fread(buf, 1, sizeof(buf), in)) > 0) {
			ec_neg1(  nsent = writeall(fd, buf, nread) )
			nsenttotal += nsent;
		}
		if (statbuf.st_size != nsenttotal)
			printf("Sizes different stat = %ld; sent = %d\n",
			  (long)statbuf.st_size, nsenttotal);
		(void)fclose(in);
	}
	else
		printf("Unknown request: %.25s\n", s);
	return true;

EC_CLEANUP_BGN
	EC_FLUSH("Error sending response (nonfatal)")
	return false;
EC_CLEANUP_END
}

int main(int argc, char *argv[])
{
	SSI *ssip = NULL;
	char msg[1600];
	ssize_t nrcv;
	int i, fd, c;
	char hostbuf[100] = "//", *host = NULL;

	while ((c = getopt(argc, argv, ":h:")) != -1)
		switch(c) {
		case 'h':
			host = optarg;
			break;
		case ':':
			fprintf(stderr, "Option -%c requires an operand\n", optopt);
			/* fall through */
		default:
			fprintf(stderr, "Usage: ws [-h host]\n");
			exit(EXIT_FAILURE);
		}
	if (host == NULL) {
		ec_neg1( gethostname(&hostbuf[2], sizeof(hostbuf) - 2) ) /* leave // there */
		host = hostbuf;
	}
	if (strchr(host, ':') == NULL)
		strcat(host, ":8000");
	printf("Connecting to host \"%s\"\n", host);
	ec_null(  ssip = ssi_open(host, true) )
	printf("\t...connected\n");
	while (true) {
		ec_neg1( fd = ssi_wait_server(ssip) )
		nrcv = read(fd, msg, sizeof(msg));
		switch (nrcv) {
		case 0:
			continue;
		case -1:
			if (errno == ECONNRESET)
				fprintf(stderr, "ECONNRESET\n");
			syserr_print("smi_receive_skt_string got error (nonfatal)\n");
			continue;
		default:
			msg[nrcv] = '\0';
			printf("Got %d bytes:\n", nrcv);
			for (i = 0; i < nrcv; i++) {
				if (isgraph((int)msg[i]) || isspace((int)msg[i]))
					printf("%c", msg[i]);
				else
					printf("'\\%o'", msg[i]);
			}
			(void)handle_request(ssip, msg, fd);
		}
	}
	ec_false( ssi_close(ssip) )
	printf("Done.\n");
	exit(EXIT_SUCCESS);

EC_CLEANUP_BGN
	return EXIT_FAILURE;
EC_CLEANUP_END
}
