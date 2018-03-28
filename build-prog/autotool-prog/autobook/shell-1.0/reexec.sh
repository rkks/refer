#! /bin/sh

# Zsh is not Bourne compatible without the following:
if test -n "$ZSH_VERSION"; then
  emulate sh
  NULLCMD=:
fi

# Bash is not POSIX compliant without the following:
test -n "$BASH_VERSION" && set -o posix

SHELL="${SHELL-/bin/sh}"
if test x"$1" = x--re-executed; then
  # Functional shell was found.  Remove option and continue
  shift
elif "$SHELL" -c 'foo () { exit 0; }; foo' 2>/dev/null; then
  # The current shell works already!
  :
else
  # Try alternative shells that (sometimes) support functions
  for cmd in sh bash ash bsh ksh zsh sh5; do
    set `IFS=:; X="$PATH:/bin:/usr/bin:/usr/afsws/bin:/usr/ucb"; echo $X`
    for dir
      shell="$dir/$cmd"
      if (test -f "$shell" || test -f "$shell.exe") &&
        "$shell" -c 'foo () { exit 0; }; foo' 2>/dev/null
      then
        # Re-execute with discovered functional shell
        SHELL="$shell" exec "$shell" "$0" --re-executed ${1+"$@"}
      fi
    done
  done
  echo "Unable to locate a shell interpreter with function support" >&2
  exit 1
fi

foo () {
    echo "$SHELL: ta da!"
}

foo

exit 0