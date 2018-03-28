#! /bin/sh
# showmatch -- mark string that matches pattern
pattern=$1; shift
nawk 'match($0,pattern) > 0 {
    s = substr($0,1,RSTART-1)
    m = substr($0,1,RLENGTH)
    gsub (/[^\b- ]/, " ", s)
    gsub (/./,       "^", m)
    printf "%s\n%s%s\n", $0, s, m
}' pattern="$pattern" $*
