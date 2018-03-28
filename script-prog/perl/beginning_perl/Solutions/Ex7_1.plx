#!/usr/bin/perl
#ex7_1.plx
use warnings;


@one = qw(one two three four five six);
@two = qw(two four six eight ten twelve);
@three = qw(three six nine twelve fifteen eighteen);
@four = qw(four eight twelve sixteen twenty twenty-four);
@five = qw(five ten fifteen twenty twenty-five thirty);
@six = qw(six twelve eighteen twenty-four thirty thirty-six);

@mult_table=(\@one, \@two, \@three, \@four, \@five, \@six);

print "Enter a number between 1 and 6: ";
$i = <STDIN>;
print "Enter a number between 1 and 6: ";
$j = <STDIN>;
chomp ($i, $j);

print $i--, " multiplied by ", $j--," equals ", $mult_table[$i]->[$j], "\n";
