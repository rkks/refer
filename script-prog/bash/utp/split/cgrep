#!/usr/bin/perl

# Usage: cgrep [-lines] pattern [files]

$context = 3;

# They might want more or less context.

if ($ARGV[0] =~ /^-(\d+)$/) {
    $context = $1;
    shift;
}

# Get the pattern and protect the delimiter.

$pat = shift;
$pat =~ s#/#\\/#g;

# First line of input will be middle of array.
# In the eval below, it will be $ary[$context].

$_ = <>;
push(@ary,$_);

# Add blank lines before, more input after first line.

for (1 .. $context) {
    unshift(@ary,'');
    $_ = <>;
    push(@ary,$_) if $_;
}

# Now use @ary as a silo, shifting and pushing.

eval <<LOOP_END;
    while (\$ary[$context]) {
        if (\$ary[$context] =~ /$pat/) {
            print "------\n" if \$seq++;
            print \@ary,"\n";
        }
        \$_ = <> if \$_;
        shift(\@ary);
        push(\@ary,\$_); 
    }
LOOP_END
