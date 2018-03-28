/*
	Mini Web Server
	AUP2, Sec. 8.04.4

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
/*[top]*/
#define HEADER\
	"HTTP/1.0 %s\r\n"\
	"Server: AUP-ws\r\n"\
	"Content-Length: %ld\r\n"

#define CONTENT_TEXT\
	"Content-Type: text/html\r\n\r\n"

#define CONTENT_JPEG\
	"Content-Type: image/jpeg\r\n\r\n"

#define HTML_NOTFOUND\
	"<!DOCTYPE html PUBLIC \"-//IETF//DTD HTML 2.0//EN\">\n"\
	"<html><head><title>Error 404</title>\n"\
	"</head><body>\n"\
	"<h2>AUP-ws server can't find document</h2>"\
	"</body></html>\r\n"
/*[send_header]*/
static void send_header(SSI *ssip, const char *msg, off_t len,
  const char *path, int fd)
{
	char buf[1000], *dot;

	snprintf(buf, sizeof(buf), HEADER, msg, (long)len);
	ec_neg1( writeall(fd, buf, strlen(buf)) )
	dot = strrchr(path, '.');
	if (dot != NULL && (strcasecmp(dot, ".jpg") == 0 ||
	  strcasecmp(dot, ".jpeg") == 0))
		ec_neg1( writeall(fd, CONTENT_JPEG, strlen(CONTENT_JPEG)) )
	else
		ec_neg1( writeall(fd, CONTENT_TEXT, strlen(CONTENT_TEXT)) )
	return;

EC_CLEANUP_BGN
	EC_FLUSH("Error sending response (nonfatal)")
EC_CLEANUP_END
}
/*[handle_request]*/
#define DEFAULT_DOC "index.html"
#define WEB_ROOT "/aup/webroot/"

static bool handle_request(SSI *ssip, char *s, int fd)
{
	char *tkn, buf[1000], path[500];
	int ntkn;
	FILE *in;
	struct stat statbuf;
	ssize_t nread;

	for (ntkn = 1; (tkn = strtok(s, " ")) != NULL; ntkn++) {
		s = NULL;
		switch (ntkn) {
		case 1:
			if (strcasecmp(tkn, "get") != 0) {
				printf("Unknown request\n");
				return false;
			}
			continue;
		case 2:
			break;
		}
		break;
	}
	snprintf(path, sizeof(path) - 1 - strlen(DEFAULT_DOC),
	  "%s%s", WEB_ROOT, tkn);
	if (stat(path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode)) {
		if (path[strlen(path) - 1] != '/')
			strcat(path, "/");
		strcat(path, DEFAULT_DOC);
	}
	if (stat(path, &statbuf) == -1 ||
	  (in = fopen(path, "rb")) == NULL) {
	  	send_header(ssip, "404 Not Found", strlen(HTML_NOTFOUND), "", fd);
		ec_neg1( writeall(fd, HTML_NOTFOUND, strlen(HTML_NOTFOUND)) )
		return false;
	}
	send_header(ssip, "200 OK", statbuf.st_size, path, fd);
	while ((nread = fread(buf, 1, sizeof(buf), in)) > 0)
		ec_neg1( writeall(fd, buf, nread) )
	ec_eof( fclose(in) )
	return true;

EC_CLEANUP_BGN
	EC_FLUSH("Error sending response (nonfatal)")
	return false;
EC_CLEANUP_END
}
/*[main]*/
#define PORT ":8000"

int main(void)
{
	SSI *ssip = NULL;
	char msg[1600];
	ssize_t nrcv;
	int fd;
	char host[100] = "//";

	ec_neg1( gethostname(&host[2], sizeof(host) - 2 - strlen(PORT)) )
	strcat(host, PORT);
	printf("Connecting to host \"%s\"\n", host);
	ec_null( ssip = ssi_open(host, true) )
	printf("\t...connected\n");
	while (true) {
		ec_neg1( fd = ssi_wait_server(ssip) )
		switch (nrcv = read(fd, msg, sizeof(msg) - 1)) {
		case -1:
			printf("Read error (nonfatal)\n");
			/* fall through */
		case 0:
			ec_false( ssi_close_fd(ssip, fd) )
			continue;
		default:
			msg[nrcv] = '\0';
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
/*[]*/
#if 0
/*[html]*/
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<html>
<head>
</head>
<body>
<h1>This is a test page.</h1>
<p><img src="picture.jpg">
</body>
</html>
/*[]*/
#endif
