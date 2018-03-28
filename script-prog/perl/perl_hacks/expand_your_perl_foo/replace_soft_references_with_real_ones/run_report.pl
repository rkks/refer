use strict;
use warnings;

my ($books_total, $movies_total, $candy_total, $certificates_total, $total);

create_report();
print_report();
exit();

sub print_report
{
    print <<END_REPORT;
SALES
    Books:             $books_total
    Movies:            $movies_total
    Candy;             $candy_total
    Gift Certificates: $certificates_total

TOTAL:                 $total
END_REPORT
}

sub create_report
{
    # your code here
}

sub get_row
{
    return unless defined( my $line = <DATA> );
    chomp( $line );
    return split( ':', $line );
}

__DATA__
books:10.00
movies:15.00
candy:7.50
certificates:8.00
