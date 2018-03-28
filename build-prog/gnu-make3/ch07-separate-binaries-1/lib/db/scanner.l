%{
#include <db/scanner.h>
#include <db/playlist.h>
%}

%%
playlist	return T_PLAYLIST;
track		return T_TRACK;
\"[^\"]+\"	return yytext;
.
\n
