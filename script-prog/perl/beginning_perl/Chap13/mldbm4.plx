#!/usr/bin/perl
#mldbm4.plx
use MLDBM qw(SDBM_File Storable);
use POSIX;
use warnings;
use strict;

my %mldbm;
my $mldb_file="mldbmdemo.dbm";

tie (%mldbm, 'MLDBM', $mldb_file, O_CREAT|O_RDWR, 0644);

unless (tied %mldbm) {
    print "Error opening $mldb_file: $!\n";
} else {
    # wipe out the old contents, if any
    undef %mldbm;

    $mldbm{'Created'}=localtime;

    # assign a list anonymously, directly and as a copy
    $mldbm{'AnonymousList'}=[1,2,3,4,"Five",6,7.8];
    my @list=(9,"Ten",11,12.13,14);
    $mldbm{'OriginalList'}=\@list;
    $mldbm{'CopyOfList'}=[ @list ];
    $mldbm{'NumberOfListElems'}=@list;
    $list[0]="Nine"; #does NOT modify 'OriginalList'

    # assign a hash anonymously, directly and as a copy
    $mldbm{'AnonymousHash'}={One=>'1',Two=>'2',Three=>'3'};
    my %hash=(Four=>'4',Five=>'5',Six=>'6');
    $mldbm{'OriginalHash'}=\%hash;
    $mldbm{'CopyOfHash'}={ %hash };
    $mldbm{'NumberOfHashKeys'}=keys %hash;
    $hash{Four}="IV"; #does NOT modify 'OriginalHash'

    # assign a random key and value
    $mldbm{rand()}=rand;

    # a more complex assignment
    $mldbm{'HashOfMixedValues'}={
        List1=>[1,2,3],
        List2=>[4,5,6],
        String=>"A String",
        Hash1=>{
                A=>"a",
                B=>"b",
                Hash2=>{
                    C=>"c",
                },
        },
        Number=>14.767,
        List3=>[7,8,9],
    };

    # now dump out the contents again
    foreach (sort keys %mldbm) {
        print "$_ => $mldbm{$_}\n";
        if (my $ref=ref $mldbm{$_}) {
            if ($ref eq 'HASH') {
                foreach my $key (sort keys %{ $mldbm{$_} }) {
                    print "\t$key => $mldbm{$_}{$key}\n";
                }
            } else {
                print "\t",(join ",",@{ $mldbm{$_} }),"\n";
            }
        }
    }

    untie %mldbm;
}
