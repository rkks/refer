/*
	More-complete Web Browser
	AUP2, Sec. 8.04.3 (not in book)

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

/*
	http_URL = "http:" "//" host [ ":" port ] [ abs_path [ "?" query ]]
*/

/* use basename? *********************************************************/

#define MAX_LINK	100
#define MAX_HREF	120
#define MAX_ANCHORS	200

static struct {
	char a_link[MAX_LINK];
	char a_href[MAX_HREF];
} anchor[MAX_ANCHORS];
static int nanchors;

static void add_href(const char *tag)
{
	const char *p, *href = NULL;
	char delim = '\0';
	int nhref;

	if (nanchors < MAX_ANCHORS) {
		anchor[nanchors].a_href[0] = '\0';
		anchor[nanchors].a_link[0] = '\0';
		for (p = tag; *p && *p != delim; p++)
			if (href == NULL && strncasecmp(p, "HREF", 4) == 0) {
				p += 4;
				while (isspace((int)*p))
					p++;
				if (*p == '=') {
					p++;
					while (isspace((int)*p))
						p++;
					if (*p == '"' || *p == '\'')
						delim = *p;
					else
						p--;
					href = p + 1;
				}
			}
		if (href != NULL) {
			nhref = p - href;
			if (nhref > MAX_HREF - 1)
				nhref = MAX_HREF - 1;
			strncpy(anchor[nanchors].a_href, href, nhref);
			anchor[nanchors].a_href[nhref] = '\0';
		}
	}
}

static void add_link(const char *link)
{
	int nlink;

	if (nanchors < MAX_ANCHORS && anchor[nanchors].a_href[0] != '\0') {
		nlink = strlen(link);
		if (nlink > MAX_LINK - 1)
			nlink = MAX_LINK - 1;
		strncpy(anchor[nanchors].a_link, link, nlink);
		anchor[nanchors].a_link[nlink] = '\0';
		nanchors++;
	}
}

static const char *go_link(int limit)
{
	int i;
	long n;
	static char buf[MAX_HREF];

	if (limit <= 0 || limit > nanchors)
		limit = nanchors;
	for (i = 0; i < limit; i++)
		printf("%d:\t%-20.20s --> %s\n", i + 1, anchor[i].a_link,
		  anchor[i].a_href);
	if (limit < nanchors)
		printf("\t(%d more links not shown)\n", nanchors - limit);
	while (true) {
		printf("Link number, URL, or Ctrl-D to quit]? ");
		if (fgets(buf, sizeof(buf), stdin) == NULL || buf[0] == 'q')
			return NULL;
		for (i = 0; buf[i]; i++)
			if (!isspace((int)buf[i]) && !isdigit((int)buf[i])) {
				i = strlen(buf);
				if (i > 0 && buf[i - 1] == '\n')
					buf[i - 1] = '\0';
				return buf;
			}
		n = strtol(buf, NULL, 10);
		if (n > 0 && n <= limit)
			return anchor[n - 1].a_href;
		printf("Invalid number\n");
	}
}

static void parse_url(const char *url, char *host, size_t max_host,
  char *path, size_t max_path)
{
	const char *slash;
	int host_len;
	bool new_host = false;

	if (strncasecmp(url, "http://", 7) == 0) {
		url += 7;
		new_host = true;
	}
	if (strncasecmp(url, "https://", 8) == 0) {
		url += 8;
		new_host = true;
	}
	if (new_host) {
		slash = strchr(url, '/');
		if (slash == NULL) {
			strcpy(path, "/");
			slash = &url[strlen(url)];
		}
		else {
			strncpy(path, slash, max_path);
			path[max_path - 1] = '\0';
		}
		host_len = slash - url;
		if (host_len > max_host - 1)
			host_len = max_host - 1;
		strncpy(host, url, host_len);
		host[host_len] = '\0';
	}
	else {
		strncpy(path, url, max_path);
		path[max_path - 1] = '\0';
	}
}

static void makepath(char *path, size_t max_path, const char *newpath)
{
	char *filename;

	if (newpath[0] == '/')
		strncpy(path, newpath, max_path);
	else {
		filename = strrchr(path, '/');
		if (filename == NULL)
			filename = path;
		else
			filename++;
		/* anything with a dot is a file name to us */
		if (strchr(filename, '.') != NULL)
			*filename = '\0';
		if (path[strlen(path) - 1] != '/')
			strncat(path, "/", max_path);
		strncat(path, newpath, max_path);
	}
}

static void display(const char *s, int len, bool showtags,
  bool want_display)
{
	static enum {DSP_NORMAL, DSP_INTAG, DSP_INENTITY} state = DSP_NORMAL;
	static int taglen, ntag = 0, col = 0, nlink = 0;
	static char tag[100], link[MAX_LINK];
	static bool inlink = false, inscript = false;
	  /* substates of DSP_NORMAL */
	int i, c;

	for (i = 0; i < len; i++) {
		c = s[i] & 0177;

		if (c == '\r') {
#if 0
			if (want_display)
				putc('@', stdout);
#endif
			continue;
		}
#if 0
		if (c == '\n') {
			if (want_display)
				putc('#', stdout);
		}
#endif
		switch (state) {
		case DSP_NORMAL:
			if (c == '<') {
				state = DSP_INTAG;
				break;
			}
			if (inscript)
				break;
			switch (c) {
			case '\n':
				if (col > 0) {
					if (want_display)
						putc('\n', stdout);
					col = 0;
				}
				break;
			case '&':
				state = DSP_INENTITY;
				break;
			default:
				if (inlink && nlink < sizeof(link) - 1)
					link[nlink++] = (char)c;
				if (col >= 80) {
					if (want_display)
						putc('\n', stdout);
					col = 0;
				}
				if (col > 0 || !isspace(c)) {
					col++;
					if (want_display)
						putc(c, stdout);
				}
			}
			break;
		case DSP_INTAG:
			if (c == '>') {
				char tagname[20];

				tag[ntag] = '\0';
				if (showtags)
					printf("<%s>\n", tag);
				ntag = 0;
				state = DSP_NORMAL;
				taglen = strcspn(tag, " \t\n");
				if (taglen > sizeof(tagname) - 1)
					taglen = sizeof(tagname) - 1;
				strncpy(tagname, tag, taglen);
				tagname[taglen] = '\0';
				if (strcasecmp(tagname, "P") == 0) {
					if (want_display)
						fputs("\n\n", stdout);
					col = 0;
				}
				else if (strcasecmp(tagname, "BR") == 0) {
					if (want_display)
						fputs("\n", stdout);
					col = 0;
				}
				else if (strcasecmp(tagname, "A") == 0) {
					inlink = true;
					nlink = 0;
					add_href(tag);
				}
				else if (inlink && strcasecmp(tagname, "/A") == 0) {
					link[nlink] = '\0';
					add_link(link);
					inlink = false;
				}
				else if (strcasecmp(tagname, "SCRIPT") == 0)
					inscript = true;
				else if (inscript && strcasecmp(tagname, "/SCRIPT") == 0)
					inscript = false;
			}
			else if ((!isspace(c) || ntag > 0) && ntag < sizeof(tag) - 1)
				tag[ntag++] = c;
			break;
		case DSP_INENTITY:
			if (c == ';') {
				state = DSP_NORMAL;
				if (want_display)
					putc('?', stdout);
			}
			break;
		default:
			fprintf(stderr, __FILE__ ":" __func__ " -- bad state\n");
			exit(EXIT_FAILURE);
		}
	}
}

#define FN_TRANSFER_ENCODING	"Transfer-Encoding"
#define FN_CONTENT_LENGTH		"Content-Length"
#define FN_LOCATION				"Location"

/*
	Assumes whole header and chunk lengths are contiguous in a single call.
*/
static bool response(char *s, int len, bool start, bool showtags,
  bool want_display, char **newloc)
{
	char *body, *fn, *fv, *colon;
	static bool ischunked;
	static int chunklen;

/*printf("\n\n****response start = %d; len = %d\n", start, len);*/
	*newloc = NULL;
	if (start) {
		ischunked = false;
		chunklen = 0;
		body = strstr(s, "\r\n\r\n");
		if (body == NULL) {
			printf("Invalid header in response:\n");
			printf("%.500s\n", s);
			return false;
		}
		*body = '\0';
		body += 4;
		if ((fn = strstr(s, FN_TRANSFER_ENCODING)) != NULL) {
			if ((colon = strchr(fn, ':')) != NULL) {
				fv = colon + 1;
				while (isspace((int)*fv))
					fv++;
				if (strncasecmp(fv, "chunked", 7) == 0)
					ischunked = true;
			}
		}
		if ((fn = strstr(s, FN_CONTENT_LENGTH)) != NULL) {
			if ((colon = strchr(fn, ':')) != NULL) {
				fv = colon + 1;
				while (isspace((int)*fv))
					fv++;
				chunklen = (int)strtol(fv, NULL, 10);
			}
		}
		if ((fn = strstr(s, FN_LOCATION)) != NULL) {
			if ((colon = strchr(fn, ':')) != NULL) {
				fv = colon + 1;
				while (isspace((int)*fv))
					fv++;
				printf("Location = \"%s\"\n", fv);
				*newloc = fv;
				return true;
			}
		}
		len -= body - s;
/*printf("header size = %d; revised len = %d\n", body - s, len);*/
		s = body;
		/*printf("chunked = %d; chunklen = %d\n", ischunked, chunklen);*/
	}
	while (len > 0) {
		int seglen;

		if (ischunked && chunklen <= 0) {
			char *end;

			chunklen = strtol(s, &end, 16);
/*printf("Got chunked len = %d from [%.5s] end = %c\n", chunklen, s, *end);*/
			if (chunklen > 0)
				chunklen += 2; /* CRLF at end of data not counted yet */
			if (strncmp(end, "\r\n", 2) == 0)
				end += 2;
			len -= end - s;
			s = end;
		}
		if (chunklen <= 0) {
/*printf("discarded len = %d\n", len);*/
			return false;
}
		seglen = chunklen;
		if (seglen > len)
			seglen = len;
#if 0
if (seglen < 40) {
char buf[50];
strncpy(buf, s, seglen);
buf[seglen] = '\0';
printf("display(%s, %d)\n", buf, seglen);
}
else
printf("display(%.15s ... %.15s, %d)\n", s, &s[seglen - 15], seglen);
#endif
		display(s, seglen, showtags, want_display);
		len -= seglen;
		s += seglen;
		chunklen -= seglen;
	}
	return chunklen > 0;
}

int main(int argc, char *argv[])
{
	SSI *ssip = NULL;
	char msg[1600], *newloc;
	const char *url = "http://www.gnu.org";
	ssize_t nrcv, nrcv_total;
	int fd, c, limit = 0;
	bool showtags = false, retry = false, want_display = true,
	  raw_display = false;
	char host[500] = "", prevhost[500] = "",
	  path[MAX_HREF] = "/", newpath[MAX_HREF];

	while ((c = getopt(argc, argv, ":arstu:")) != -1)
		switch(c) {
		case 'a':
			limit = 10;
			break;
		case 's':
			want_display = false;
			break;
		case 'r':
			raw_display = true;
			want_display = false;
			break;
		case 't':
			showtags = true;
			break;
		case 'u':
			url = optarg;
			break;
		case ':':
			fprintf(stderr, "Option -%c requires an operand\n", optopt);
			/* fall through */
		default:
			fprintf(stderr, "Usage: br [-a] [-s] [-t] [-u url]\n");
			exit(EXIT_FAILURE);
		}
	while (true) {
		bool start;

		if (newloc != NULL) {
printf("newloc = %s\n", newloc);
			url = newloc;
			newloc = NULL;
			retry = false;
		}
		parse_url(url, host, sizeof(host), newpath, sizeof(newpath));
		if (!retry)
			makepath(path, sizeof(path), newpath);
		if (strcmp(prevhost, host) != 0 || ssip == NULL) {
			snprintf(msg, sizeof(msg), "//%s%s", host,
			  strchr(host, ':') == NULL ? ":80" : "");
			printf("Connecting to host \"%s\"\n", msg);
			ssip = ssi_open(msg, false);
			ec_neg1( fd = ssi_get_server_fd(ssip) )
			if (ssip == NULL) {
				/* need to document the following fcn */
				syserr_print("Can't connect");
				if ((url = go_link(limit)) == NULL)
					break;
				continue;
			}
			strcpy(prevhost, host);
			printf("\t...connected.\n");
		}
		else
			printf("Continuing with exisiting connection\n");
		snprintf(msg, sizeof(msg), "GET %s HTTP/1.1\nHost: %s\n\n",
		  path, host);
		printf("Browsing URL \"%s\"\n", path);
		nrcv_total = 0;
		nanchors = 0;
		memset(anchor, 0, sizeof(anchor));
		ec_neg1( writeall(fd, msg, strlen(msg)) )
		while (true) {
			start = nrcv_total == 0;
			ec_neg1(  nrcv = read(fd, msg, sizeof(msg)) )
			if (nrcv == 0)
				break;
			msg[nrcv] = '\0';
			nrcv_total += nrcv;
			if (raw_display)
				printf("\n\n*** %d bytes in message ***\n%s\n"
				  "***************************\n", nrcv,
				  start ? msg : "   (message text not shown)");
			if (!response(msg, nrcv, start, showtags, want_display,
			  &newloc) || newloc != NULL)
				break;
		}
		if (newloc != NULL)
			continue;
		if (nrcv_total == 0 && !retry) {
			printf("Connection dropped by host\n");
			ec_false( ssi_close(ssip) )
			ssip = NULL;
			retry = true;
			continue;
		}
		printf("\n%d total bytes received\n", nrcv_total);
		retry = false;
		if ((url = go_link(limit)) == NULL)
			break;
	}
	if (ssip != NULL)
		ec_false( ssi_close(ssip) )
	printf("Done.\n");
	exit(EXIT_SUCCESS);

EC_CLEANUP_BGN
	return EXIT_FAILURE;
EC_CLEANUP_END
}
