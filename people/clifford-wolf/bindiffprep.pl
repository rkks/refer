#!/usr/bin/perl
#
# Copyright (C) 2007  RIEGL Research ForschungsGmbH
# Copyright (C) 2007  Clifford Wolf <clifford@clifford.at>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
# Read in bytes as hex numbers and format them by inserting newlines
# in a way so that insertions and deletions of binary data can be
# seen in a line-based diff easily.

my @lastbytes = ( 0, 0, 0, 0 );
my $bytecount = -1;

sub gotbyte($)
{
        my $val = $_[0];
        shift @lastbytes;
        push @lastbytes, $val;

        if ($bytecount == -1) {
                $bytecount++;
                print $val;
                return;
        }

        my $hash = crypt(join(@latsbytes, "-"), $val);
        $bytecount = $hash =~ /[i-n][o-z]/ ? 0 : $bytecount + 1;

        print $bytecount % 16 == 0 ? "\n" : " ";
        print $val;
}

if ($#ARGV > -1 and $ARGV[0] =~ /^-/) {
        print STDERR "Usage: od -A n -t x1 FILENAME | perl bindiffprep.pl > FILENAME.bd\n";
        exit 1;
}

while (<>) {
        gotbyte $1 while s/^\s*(\S\S)\s*//;
}

print "\n";
exit 0;

