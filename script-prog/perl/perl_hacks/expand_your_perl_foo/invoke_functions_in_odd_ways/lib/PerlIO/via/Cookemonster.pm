package Function_IO_Layer;

# A dumb base class for simple PerlIO::via::* layers.
# See PerlIO::via::dynamic for a smarter version of this.

sub PUSHED { bless {}, $_[0] } # our dumb ctor

# when reading
sub FILL
{
    my($this, $fh) = @_;
    defined(my $line = readline($fh)) or return undef;
    return $this->change($line);
}

sub WRITE
{
    my($this,$buf,$fh) = @_;
    print {$fh} $this->change($buf)  or return -1;
    return length($buf);
}

sub change { my($this,$str) = @_;  $str; } #override!

# Puts everything in allcaps.
package PerlIO::via::Scream;

use base 'Function_IO_Layer';

sub change
{
    my($this, $str) = @_;
    return uc($str);
}

# Changes "I" to "me".
package PerlIO::via::Cookiemonster;

use base 'Function_IO_Layer';

sub change
{
    my($this, $str) = @_;
    $str =~ s<\bI\b><me>g;
    return $str;
}

1;
