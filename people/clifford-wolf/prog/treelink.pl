#!/usr/bin/perl -w
#
# treelink is Copyright (C) 2004 Clifford Wolf
# treelink is Copyright (C) 2004 LINBIT Information Technologies
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version. A copy of the GNU General
# Public License can be found at COPYING.
#

use strict;
use English;

# read config file
my $config_txt = "";
while (<>) {
	s/#.*//; # strip away comments
	$config_txt .= $_;
}

# add whitespaces before and after other seperation chars
$config_txt =~ s/([\{\}\(\)=])/ $1 /g;

# this is essentially our entire lexer logic
my @config_tok = split /\s+/, $config_txt;

# serial number for temp object files
my $out_number = 0;

# decent recursive parser for the token array
sub parse_block($);
sub parse_block($)
{
	# current ouput object filename
	my $out_file = $_[0];

	# are we in a private section in the config file
	my $private_level = 0; 

	# have we got any source objects in a private section
	my $got_privates = 0;

	# list of all non-private objects and all objects
	my @obj_public = ();
	my @obj_all = ();

	# the root of the call tree the doesn't need a '{' char
	if ( $out_file ne "" ) {
		die "parser error: $config_tok[0]" if $config_tok[0] ne "{";
		shift @config_tok;
	}

	# main loop consuming input tokens
	while ( defined $config_tok[0] and $config_tok[0] ne "}" )
	{
		next if $config_tok[0] eq "";
		if ( $config_tok[0] eq "(" ) { $private_level++; next; }
		if ( $config_tok[0] eq ")" ) { $private_level--; next; }

		# this is a new unnamed block
		if ( $config_tok[0] eq "{" ) {
			my $file = sprintf "tmpobj%05d.o", $out_number++;

			# create the temporary object file and then add
			# the new file to the parser stack as it would have
			# been apared in the config file just this way.
			parse_block($file);
			unshift @config_tok, $file;
		}

		# this is a new named block
		if ( defined $config_tok[1] and $config_tok[1] eq "=" ) {
			my $file = shift @config_tok;
			shift @config_tok;

			parse_block($file);
			unshift @config_tok, $file;
		}

		# only relocateables might do symbol-hiding
		if ( $config_tok[0] =~ /\.o$/ ) {
			if ( $private_level ) {
				$got_privates = 1;
			} else {
				push @obj_public, $config_tok[0];
			}
		}

		# this might be an object, linker script, ld option, ...
		push @obj_all, $config_tok[0];
	}
	continue
	{
		shift @config_tok;
	}

	die "parser error: unbalanced '(' .. ')'" if $private_level;

	# the root of the call tree has no ouput file
	if ( $out_file ne "" ) {
		die "parser error: unexpected EOF" if not defined $config_tok[0];
		shift @config_tok;

		# Create the *.syms file using nm
		if ( $got_privates ) {
			open(SYMS, ">$out_file.syms") || die "can't open $out_file.syms";
			foreach my $obj (@obj_public) {
				next unless -f $obj;
				open(NM, "nm $obj |");
				while (<NM>) {
					print SYMS "$1\n" if /^\S.*\s(\S+)\s*$/;
				}
				close NM;
			}
			close SYMS;
		}

		# assamble and run ld (and objcopy) command(s)
		#
		# we have support for relocateables, shared objects and
		# executeables here ..
		if ( $out_file =~ /\.o$/ )
		{
			# make the relocateable
			my $command = "ld -r -o $out_file " . join(" ", @obj_all);
			print "$command\n";
			system $command and exit 1;

			# and we might have something to hide here
			if ( $got_privates ) {
				$command = "objcopy --keep-global-symbols=$out_file.syms $out_file";
				print "$command\n";
				system $command and exit 1;
			}
		}
		elsif ( $out_file =~ /\.so$/ )
		{
			# we might have something to hide here
			if ( $got_privates )
			{
				# make the temp relocateable
				my $command = "ld -r -o tmpobj$out_file.o " . join(" ", @obj_all);
				print "$command\n";
				system $command and exit 1;

				# hide stuff
				$command = "objcopy --keep-global-symbols=$out_file.syms tmpobj$out_file.o";
				print "$command\n";
				system $command and exit 1;

				# create shared object
				$command = "cc -shared -o $out_file tmpobj$out_file.o";
				print "$command\n";
				system $command and exit 1;
			}
			else
			{
				# create shared object
				my $command = "cc -shared -o $out_file " . join(" ", @obj_all);
				print "$command\n";
				system $command and exit 1;
			}
		}
		else
		{
			# we might have something to hide here
			if ( $got_privates )
			{
				# make the temp relocateable
				my $command = "ld -r -o tmpobj$out_file.o " . join(" ", @obj_all);
				print "$command\n";
				system $command and exit 1;

				# hide stuff
				$command = "objcopy --keep-global-symbols=$out_file.syms tmpobj$out_file.o";
				print "$command\n";
				system $command and exit 1;

				# create executeable
				$command = "cc -o $out_file tmpobj$out_file.o";
				print "$command\n";
				system $command and exit 1;
			}
			else
			{
				# create executeable
				my $command = "cc -o $out_file " . join(" ", @obj_all);
				print "$command\n";
				system $command and exit 1;
			}
		}
	}
}

# let's rock!
parse_block "";

