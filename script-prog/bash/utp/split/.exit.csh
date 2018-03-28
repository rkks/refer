# Sample .exit.csh file for use with the C shell 'cd' alias

# .enter file may have put old umask in shell variable - reset it:
if ($?prevumask) then
        umask $prevumask
        echo ".exit.csh: setting umask to $prevumask"
        unset prevumask
endif

# Reminder to come back here if need to:
echo "If you didn't check in the RCS files, type 'cd $cwd'."

# Set generic prompt (setprompt alias comes from .cshrc file):
setprompt
