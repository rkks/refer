# Sample .exit.sh file for use with the sh-type 'cd' function

# .enter.sh file may have put old umask in shell variable - reset it:
if test "${prevumask+X}" = X
then
        umask $prevumask
        echo ".exit.sh: setting umask to $prevumask"
        unset prevumask
fi

# Reminder to come back here if need to:
echo "If you didn't check in the RCS files, type 'cd `pwd`'."

# Set generic prompt (setprompt comes from .profile):
setprompt
