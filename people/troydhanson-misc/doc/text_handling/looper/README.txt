looper.c            -- basic iteration over lines of stdin or the given file 
looper_readline.c   -- Uses readline in interactive mode, so more user friendly.
                       Otherwise reads the file specified with the -f option.
                       This program wraps either usage in next_line().
                       In both cases it enforces a maximum line length limit.
