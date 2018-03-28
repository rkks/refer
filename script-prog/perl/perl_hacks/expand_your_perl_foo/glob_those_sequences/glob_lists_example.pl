use Glob::Lists;

for ( <1..100 x 7> ) {...}           # 1, 8, 15, 22,...85, 92, 99

my @even_countdown  = <10,8..0>;     # 10, 8, 6, 4, 2, 0

my @fract_countdown = <10,9.5,..0>;  # 10, 9.5, 9,...1, 0.5, 0

my @some_primes = <1..100 x 3 : /7/ && is_prime(N)>;
                                     # 7, 37, 67, 73, 79, 97
