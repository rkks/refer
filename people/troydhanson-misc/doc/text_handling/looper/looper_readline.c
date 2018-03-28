#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

char *prompt = "> ";

void usage(char *prog) {
  fprintf(stderr, "usage: %s [-v] [-f file]\n", prog);
  fprintf(stderr, "          -v verbose\n");  
  fprintf(stderr, "          -f file to from\n");
  exit(-1);
}

int verbose;
char *file;
FILE *filef;
char buf[256]; // max line length

char *next_line() {
  size_t len;
  char *line,*tmp;

  if (file) line=fgets(buf,sizeof(buf), filef);
  else line=readline(prompt);  // must free it
  if (!line) goto done;

  len = strlen(line);
  
  if (file) { 
    /* fgets keeps trailing newline. null it out. if absent, line got truncated*/
    if (buf[len-1] == '\n') buf[len-1]='\0'; 
    else { fprintf(stderr, "line too long\n"); line=NULL; }
  } else {  
    /* copy the mallocd readline buffer and free it */
    tmp = line;
    if (len+1 < sizeof(buf)) {memcpy(buf, line, len+1); line=buf;}
    else { fprintf(stderr, "line too long\n"); line=NULL; }
    free(tmp); 
  }

 done:
  if (file && !line) fclose(filef);
  return line;
}
 
int main(int argc, char *argv[]) {
  char *line;
  int opt;

  while ( (opt = getopt(argc, argv, "v+f:")) != -1) {
    switch (opt) {
      case 'v': verbose++; break;
      case 'f': file = strdup(optarg); break;
      default: usage(argv[0]); break;
    }
  }
  if (optind < argc) usage(argv[0]);
  using_history();

  if (file && !(filef = fopen(file,"r"))) {
    perror("fopen error"); 
    exit(-1);
  }

  while ( (line=next_line()) != NULL) {
    printf("line is %s\n",line);
    add_history(line);
  }

  return 0;
}

