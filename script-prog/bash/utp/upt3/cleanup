#! /bin/sh
#
# cleanup - find files that should be removed and clean them
# out of the file system; list results and errors in /tmp/.cleanup.
# Modify the expression for your needs:

find / \(    \( -name '#*'                 -atime +1 \)  \
        -o   \( -name ',*'                 -atime +1 \)  \
        -o   \( -name rogue.sav            -atime +7 \)  \
        -o   \(      \( -name '*.bak'                    \
                     -o -name '*.dvi'                    \
                     -o -name '*.CKP'                    \
                     -o -name '.*.bak'                   \
                     -o -name '.*.CKP' \)  -atime +3 \)  \
        -o   \( -name '.emacs_[0-9]*'      -atime +7 \)  \
        -o   \( -name core                           \)  \
        -o   \( -user guest                -atime +9 \)  \
\) -print -exec rm -f {} \; > /tmp/.cleanup 2>&1
