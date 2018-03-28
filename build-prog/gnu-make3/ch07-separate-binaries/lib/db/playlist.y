%{
  #include <stdio.h>
%}

%token T_PLAYLIST T_TRACK T_STRING

%%

play_list:
	| T_PLAYLIST T_STRING '{' track_list '}'
	;

track_list:
	| track_list track
	;

track:	T_TRACK T_STRING
	;
%%
