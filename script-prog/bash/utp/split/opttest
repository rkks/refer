#! /bin/sh
# opttest - test getopt handling of options

set -- `getopt "ab:" "$@"` || {
    echo "Usage: `basename $0` [-a] [-b name] [files]" 1>&2
    exit 1
}
echo "Before loop, command line has: $*"
aflag=0  name=NONE
while :
do
    case "$1" in
    -a) aflag=1 ;;
    -b) shift; name="$1" ;;
    --) break ;;
    esac
    shift
done
shift    # Remove the trailing --
echo "aflag=$aflag / name=$name / Files are $*"
