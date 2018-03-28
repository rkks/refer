#!/usr/bin/perl
# triangle.plx
use warnings;
use strict;
use Math::Trig;

print "Triangle solver\n";
print "Enter zero if something is unknown.\n";
print "Adjacent side (x) : ";
my $x=<>;
print "Opposite side (y) : ";
my $y=<>;
print "Hypotenuse (z) : ";
my $z=<>;
print "Angle (theta) : ";
my $theta = <>;
$theta = deg2rad($theta);

if (!$theta) {
    if    ($x and $y) { $theta = atan($y/$x) }
    elsif ($x and $z) { $theta = acos($x/$z) }
    elsif ($y and $z) { $theta = asin($y/$z) }
    else  { warn "Can't work out theta. (This'll hurt)\n"}
}
unless (0+$x) {
    if    ($y and $theta) { $x = $y/tan($theta)      }
    elsif ($z and $theta) { $x = $z*cos($theta)      }
    elsif ($z and $y)     { $x = sqrt($z**2 - $y**2) }
    else  { warn "Can't work out x.\n" }
}
unless (0+$y) {
    if    ($y and $theta) { $y = $x*tan($theta)      }
    elsif ($z and $theta) { $y = $z*sin($theta)      }
    elsif ($z and $x)     { $y = sqrt($z**2 - $x**2) }
    else  { warn "Can't work out y.\n" }
}
unless (0+$z) {
    if    ($y and $theta) { $y = $y/sin($theta)      }
    elsif ($x and $theta) { $y = $x/cos($theta)      }
    elsif ($y and $x)     { $y = sqrt($y**2 + $x**2) }
    else  { warn "Can't work out y.\n" }
}

$theta = rad2deg ($theta);
print "x: $x\ny: $y\nz: $z\ntheta: $theta\n";
