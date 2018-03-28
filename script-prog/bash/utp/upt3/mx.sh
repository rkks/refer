# Look up mail exchanger for host(s)
function mx( ) {
    for host
    do
        echo "==== $host ===="
        dig "$host" mx in |
        sed -n '/^;; ANSWER SECTION:/,/^$/{
            s/^[^;].* //p
        }'
    done
}
