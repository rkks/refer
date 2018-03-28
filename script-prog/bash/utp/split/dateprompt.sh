# Put date and time in prompt; update every 60 seconds:
trap 'PS1=`date "+%a %D %H:%M%n"`\
$\ ' 5
while :
do
    sleep 60
    kill -5 $$
done &
promptpid=$!
