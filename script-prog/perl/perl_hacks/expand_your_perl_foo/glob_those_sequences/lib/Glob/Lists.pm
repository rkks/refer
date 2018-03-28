package Glob::Lists;

use Carp;

# Regexes to parse the extended list specifications...
my $NUM    = qr{\s* [+-]? \d+ (?:\.\d*)? \s* }xms;
my $TO     = qr{\s* \.\. \s*}xms;
my $FILTER = qr{ (?: : (.*) )? }xms;
my $ABtoZ  = qr{\A ($NUM) (,) ($NUM) ,? $TO ($NUM) $FILTER \Z}xms;
my $AZxN   = qr{\A ($NUM) $TO ($NUM) (?:x ($NUM))? $FILTER \Z}xms;

# Install a new glob() function...
no warnings 'redefine';
*CORE::GLOBAL::glob = sub
{
    my ($listspec) = @_;

    # Does the spec match any of the acceptable forms?
    croak "Bad list specification: <$listspec>"
        if $listspec !~ $ABtoZ && $listspec !~ $AZxN;

    # Extract the range of values and any filter...
    my ($from, $to, $incr, $filter) =  $2 eq ',' ? ($1, $4, $3-$1, $5)
                                    :              ($1, $2, $3,    $4);

    # Work out the implicit increment, if no explicit one...
    $incr = $from > $to ? -1 : 1 unless defined $incr;

    # Check for nonsensical increments (zero or the wrong sign)...
    my $delta = $to - $from;
    croak sprintf "Sequence <%s, %s, %s...> will never reach %s",
        $from, $from+$incr, $from+2*$incr, $to
            if $incr == 0 || $delta * $incr < 0;

    # Generate list of values (and return it, if not filter)...
    my @vals = map { $from + $incr * $_ } 0..($delta/$incr);
    return @vals unless defined $filter;

    # Apply the filter before returning the values...
    $filter =~ s/\b[A-Z]\b/\$_/g;
    return eval "grep {package ".caller."; $filter } \@vals";
};

1;
