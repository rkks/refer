package Devel::TraceUse;

use Time::HiRes qw( gettimeofday tv_interval );

BEGIN
{
    unshift @INC, \&trace_use unless grep { "$_" eq \&trace_use . '' } @INC;
}

sub trace_use
{
    my ($code, $module) = @_;
    (my $mod_name       = $module) =~ s{/}{::}g;
    $mod_name           =~ s/\.pm$//;
    my ($package, $filename, $line) = caller();
    my $elapsed         = 0;

    {
        local *INC      = [ @INC[1..$#INC] ];
        my $start_time  = [ gettimeofday() ];
        eval "package $package; require '$mod_name';";
        $elapsed        = tv_interval( $start_time );
    }
    $package            = $filename if $package eq 'main';
    push @used,
    {
        'file'   => $package,
        'line'   => $line,
        'time'   => $elapsed,
        'module' => $mod_name,
    };

    return;
}

END
{
    my $first = $used[0];
    my %seen  = ( $first->{file} => 1 );
    my $pos   = 1;

    warn "Modules used from $first->{file}:\n";

    for my $mod (@used)
    {
        my $message = '';

        if (exists $seen{$mod->{file}})
        {
            $pos = $seen{$mod->{file}};
        }
        else
        {
            $seen{$mod->{file}} = ++$pos;
        }

        my $indent = '  ' x $pos;
        $message  .= "$indent$mod->{module}, line $mod->{line}";
        $message  .= " ($mod->{time})" if $mod->{time};
        warn "$message\n";
    }
}

1;
