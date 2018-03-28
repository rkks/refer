spray and sprayfile
-------------------
spray and sprayfile are scripts that run a command, or send a file, on a list
of systems. Their IP addresses appear in ip.txt (one address per line).

e.g. ./spray.sh "du -sh /tmp"
e.g. ./spray.sh "tail /var/log/syslog | grep pmtr"
e.g. ./sprayfile.sh /tmp/pmtr /tmp

Ideally SSH keys are used so that spray/sprayfile are prompt-less.
