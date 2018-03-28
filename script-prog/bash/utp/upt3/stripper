#!/bin/sh
#
# stripper - strip executables in your ~/bin directory

skipug="! -perm -4000 ! -perm -2000"  # Skip setuid, setgid files

# find all executable files that aren't setuid or setgid
find $HOME/bin -type f \( -perm -0100 $skipug \) -print |

# get a description of each file
xargs file |

# skip shell scripts and other files that can't be stripped
# by searching for non stripped executable files
sed -n '/executable .*not stripped/s/:	.*//p' |

# give what's left to xargs, which runs strip interactively
xargs -rpl strip

