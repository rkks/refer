#!/usr/bin/perl -w
#
#  Tiny editor for C code with some special features
#
#  Copyright (C) 2008  Clifford Wolf <clifford@clifford.at>
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#

use English;
use strict;

my $indent_command = "./wrindent";

my @nodes;

$nodes[0] = {
	"title" => "root",
	"children" => { }
};

my $next_title;

while (<>) {
	if (/^\*\*\s*(.*\S)/) {
		$next_title = $1;
		last;
	}
}

while (defined $next_title)
{
	my $title = $next_title;

	my $code = "";
	while (<>) {
		last if /^--/;
		$code .= $_;
	}

	my $desc = "";
	$next_title = undef;
	while (<>) {
		if (/^\*\*\s*(.*\S)/) {
			$next_title = $1;
			last;
		}
		$desc .= $_;
	}

	my $p = 0;

	foreach (split / *\| */, $title) {
		if (exists $nodes[$p]{"children"}{$_}) {
			$p = $nodes[$p]{"children"}{$_};
		} else {
			my $after = $nodes[$p]{"last_child"};
			push @nodes, {
				"title" => $_,
				"children" => { },
				"has_children" => 0,
				"after" => $after,
				"parent" => $p
			};
			$nodes[$p]{"children"}{$_} = $#nodes;
			$nodes[$p]{"last_child"} = $#nodes;
			$nodes[$p]{"needs_name"} = 1;
			$nodes[$after]{"needs_name"} = 1 if defined $after;
			$p = $#nodes;
		}
	}

	open(F, ">templates.tmp") or die $!;
	print F $code;
	close F;

	open(F, "$indent_command < templates.tmp |") or die $!;
	$code = "";
	$code .= $_ while <F>;
	close F;

	unlink "templates.tmp";

	$code =~ s/^\s*//;
	$code =~ s/\s*$//;

	$desc =~ s/^\s*//;
	$desc =~ s/\s*$//;

	$nodes[$p]{"code"} = $code;
	$nodes[$p]{"desc"} = $desc;
}

sub cstr($) {
	my $str = $_[0];
	if (defined $str) {
		$str =~ s/\\/\\\\/g;
		$str =~ s/\n/\\n/g;
		$str =~ s/\t/\\t/g;
		$str =~ s/"/\\"/g;
		return "QString(\"$str\")";
	}
	return "QString::null";
}

print "/* Auto-generated by templates.pl */\n";
print "#include \"cedit.h\"\n";
print "void Editor::load_templates()\n";
print "{\n";

for (my $p=1; $p<=$#nodes; $p++) {
	printf "Template_Item *ti$p = " if $nodes[$p]{"needs_name"};
	printf "new Template_Item(%s, %s%s, this, %s, %s);\n",
			$nodes[$p]{"parent"} > 0 ? "ti".$nodes[$p]{"parent"} : "tpl_list",
			defined $nodes[$p]{"after"} ? "ti".$nodes[$p]{"after"}.", " : "",
			cstr($nodes[$p]{"title"}), cstr($nodes[$p]{"desc"}), cstr($nodes[$p]{"code"});
}

print "}\n";

