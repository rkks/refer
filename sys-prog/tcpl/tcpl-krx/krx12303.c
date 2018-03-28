/* Gregory Pietsch <gkp1@flash.net> */

#include <stdio.h>

char p[] =
"0/!10\"040\'050.001/011*!21\"/41\'/51./02*!32.!23/ "
"03*!33.!24\"004\\064.045\'005\\075.056.047.05";

int main(){int c,i,d;char s,n;s='0';while((c=getchar())
!=EOF){d=0;for(i=0;p[i]!='\0'&&d==0;i=i+4){if(p[i]==s&&
(p[i+1]==c||p[i+1]=='.')){if(p[i+2]=='0')putchar(c);else
if(p[i+2]=='/'){putchar('/');putchar(c);}else if(p[i+2]
==' ')putchar(' ');n=p[i+3];d=1;}}s=n;}return 0;}

