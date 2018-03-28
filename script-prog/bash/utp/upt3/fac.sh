# Demonstration function to calculate factorials
# Will probably overflow on all except the smallest inputs
function fac ( ) {
    if [ "$1" -gt 0 ]
    then echo $(($1 * `fac $(($1 - 1))`))
    else echo 1
    fi
}
