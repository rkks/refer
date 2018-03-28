package Module::Reloader;

use strict;
use warnings;

my %module_times;

INIT
{
    while (my ($module, $path) = each %INC)
    {
        $module_times{ $module } = -M $path;
    }
}

sub reload
{
    while (my ($module, $time) = each %module_times)
    {
        my $mod_time   = -M $INC{$module};
        next if $time == $mod_time;

        no warnings 'redefine';
        require ( delete $INC{  $module } );
        $module_times{ $module } = $mod_time;
    }
}

1;
