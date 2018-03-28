use YAML 'DumpFile';
use POSIX 'strftime';

local $YAML::UseBlock = 1;

exit 1 unless -d 'posts';

my @posts = <posts/*.yaml>;
my $file  = 'posts/' . ( @posts + 1 ) . '.yaml';

my $fields =
{
    title => '',
    date  => strftime( '%d %B %Y', localtime() ),
    text  => "\n\n",
};

DumpFile( $file, $fields );

system( $ENV{EDITOR}, $file ) == 0
    or die "Error launching $ENV{EDITOR}: $!\n";
