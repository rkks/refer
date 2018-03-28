use Number::Intervals;

my $avogadro    = 6.02214199e23;   # standard physical constant
my $atomic_mass = 55.847;          # atomic mass of iron
my $mass        = 100;             # mass in grams

my $count       = int( $mass * $avogadro/$atomic_mass );

print "Number of atoms in $mass grams of iron = $count\n";
