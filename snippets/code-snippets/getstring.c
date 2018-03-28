#include <stdio.h>

size_t get_string(char *buf, size_t max, const char *cmdline)
{
        size_t pos=0;
        FILE *fp=popen(cmdline, "r");
        if(fp == NULL) return(-1);

        while((!feof(fp)) && (pos < (max-1)))
        {
                size_t n=fread(buf+pos, 1, max-(pos+1), fp);
                if(n <= 0) break;
                pos += n;
        }

        buf[pos++]='\0';
        pclose(fp);
        return(pos);
}


int main(void)
{
        char buf[512];
        size_t len=get_string(buf, 512, "echo asdf");

        if(len >= 0)
        {
                printf("string is %s\n", buf);
        }
}
