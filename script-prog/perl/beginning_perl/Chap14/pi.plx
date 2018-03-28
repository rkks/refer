#!/usr/bin/perl
# pi.plx
use Math::BigFloat;
sub atanf {
    my $x=shift;
    my $xsquared= $x*$x;
    my $result = Math::BigFloat->new("1");
    my $delta = Math::BigFloat->new("1");
    for (1..$Math::BigFloat::div_scale*2) {$delta/=10;}
    $result/=$x;
    my $divisor=1;
    my $term=$result;
    while ($term>$delta) {
        $divisor+=2; $term/=$xsquared; $result -= $term/$divisor;
        $divisor+=2; $term/=$xsquared; $result += $term/$divisor;
    }
    return $result;
}

sub pi {
    my $precision= shift;
    $Math::BigFloat::div_scale=$precision;
    my $a = atanf(5)*16-4*atanf(239);
    my $answer = $a->ffround(-$precision);
    $answer=~ s/E-$precision$//;
    $answer=~ s/^\+3/3./;
    $answer;
}
print pi(400);
