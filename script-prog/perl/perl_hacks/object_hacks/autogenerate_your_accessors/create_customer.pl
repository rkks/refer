my $cust = My::Customer->new();
$cust->first_name( 'John' );
$cust->last_name( 'Public' );
print $cust->full_name();
