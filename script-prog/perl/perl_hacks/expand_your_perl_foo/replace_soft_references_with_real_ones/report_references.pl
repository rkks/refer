sub create_report
{
    my %totals;
    @totals{ qw( books movies candy certificates total )} =
    \( $books_total, $movies_total, $candy_total,
       $certificates_total, $total
     );

    while (my ($category, $value)  = get_row())
    {
        ${ $totals{ $category } } += $value;
        ${ $totals{total}       } += $value;
    }
}
