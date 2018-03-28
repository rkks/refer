#!/usr/bin/perl -w
## syscalls.pl for  in /home/nico/lang/c/strace/syscalls-includes
##
## Made by nicolas
## Login   <nico@epita.fr>
##
## Started on  Sun Dec 25 18:20:06 2005 nicolas
## Last update Tue Jan  3 14:27:46 2006 nicolas cormier
##

open(MASTER, "< syscalls.master") or die "Oups: $!";
open(GEN, "> syscalls.h") or die "Oups: $!";

<MASTER>;

print GEN "
typedef struct 
{
  char*	name;
  int	arg_nbr;
  char*	args_type[25];
  char* ret_type;
}		syscnfo_t;

typedef int strace_openmode;
typedef int strace_accessmode;
typedef int strace_ioctl;
typedef int strace_fcntl;
typedef int strace_lseek;
typedef int strace_filedesc;
typedef int strace_signal;
typedef int strace_mmapprot;
typedef int strace_mmapflags;

syscnfo_t	syscalls_info[] = { 
";

sub	correct_tab()
{
    my ($name, $nbr, $val) = @_;

    $val = " \"strace_openmode\"" if ($name eq "open" && $nbr == 2);
    $val = " \"strace_accessmode\"" if ($name eq "access" && $nbr == 2);
    $val = " \"strace_ioctl\"" if ($name eq "ioctl" && $nbr == 2);
    $val = " \"strace_fcntl\"" if ($name eq "fcntl" && $nbr == 2);
    $val = " \"strace_filedesc\"" if ($name eq "lseek" && $nbr == 1);
    $val = " \"strace_filedesc\"" if ($name eq "read" && $nbr == 1);
    $val = " \"strace_filedesc\"" if ($name eq "write" && $nbr == 1);
    $val = " \"strace_filedesc\"" if ($name eq "ioctl" && $nbr == 1);
    $val = " \"strace_filedesc\"" if ($name eq "fcntl" && $nbr == 1);
    $val = " \"strace_lseek\"" if ($name eq "lseek" && $nbr == 4);
    $val = " \"strace_filedesc\"" if ($name eq "mmap" && $nbr == 5);
    $val = " \"strace_signal\"" if ($name eq "sigaction" && $nbr == 1);
    $val = " \"strace_signal\"" if ($name eq "sigmask" && $nbr == 1);
    $val = " \"strace_signal\"" if ($name eq "siginterrupt" && $nbr == 1);
    $val = " \"strace_signal\"" if ($name eq "signal" && $nbr == 1);
    $val = " \"int\"" if ($name eq "exit" && $nbr == 1);
    $val = " \"strace_signal\"" if ($name eq "sigdelset" && $nbr == 2);
    $val = " \"strace_signal\"" if ($name eq "sigaddset" && $nbr == 2);
    $val = " \"strace_signal\"" if ($name eq "sigismember" && $nbr == 2);
    $val = " \"strace_signal\"" if ($name eq "kill" && $nbr == 2);
    $val = " \"strace_mmapprot\"" if ($name eq "mmap" && $nbr == 3);
    $val = " \"strace_mmapflags\"" if ($name eq "mmap" && $nbr == 4);
    return ($val);
}

my %type_h;
my @includes;

my @to_print;

while (<MASTER>) {
    chomp;
    while (/\\$/) {
        my $line;
        s/\\$//;
        $line = <MASTER>;
        chomp($line);
        $_ = "$_$line";
    }
    if (/^\#/) {
       push(@includes, $_);
    }
    next unless (/^([0-9]+)/);

    my $syscnbr = $1;

    unless (/MSTD|MCOMPAT|MNOSTD|MNOPROTO|STD|COMPAT/) {
	$to_print[$syscnbr] = "\t{ \"TOTO\", 0, {NULL}, \"int\"  }" . ",\n";
	next;
    }

    my $last = "";
    $last = $_ if (/mmap/);

    s/\.\.\.//g;

    /\{(.*)\}(.*)$/;
    my $proto = $1;
    my $supinfo = "";

    $supinfo = $1 if ($2 =~ /[ \t]*(.+)/);


    $proto =~ s/(^ )//;
    $proto =~ s/[\t ]+/ /g;
    $proto =~ /(.*) (.*)\((.*)\)/;
    my ($name, $arg_nbr, $args_type, $ret_type) = ($2, 42, $3, $1);
    if (!$supinfo eq "") {
	$name = $supinfo;
    }

    $name =~ s/sys_//g;
    $name =~ s/^__//;

    if (!($args_type eq "void")) {
      $args_type =~ s/[a-zA-Z_1-9]+\,/\,/g;
      $args_type =~ s/[a-zA-Z_1-9]+$//;
      $args_type =~ s/ *\, */\"\,\"/g;
      $args_type =~ s/^/\"/;
      $args_type =~ s/($| $)/\"/;
      $args_type =~ s/\,/\, /g;
    }
    else {
      $args_type = "\"void\"";
    }
    my $count = 1;
    my $t = "";
    foreach (split(',', $args_type)) {
      $_ = &correct_tab($name, $count, $_);
      $type_h{$_} = 1;
      $t .= $_.',';
      $count++;
    }
    $t =~ s/\,$//;
    $args_type = $t;
    $args_type = "{ " . $args_type . ", NULL }";
    my @tmp = split(/\,/, $args_type);


    $to_print[$syscnbr] = "\t{ \"$name\", " . (scalar(@tmp) - 1) . ", $args_type, \"$ret_type\"},\n";
}

foreach (@to_print) {
  print GEN $_;
}

print GEN "
};


";


foreach (@includes) {
  next if (/sys\/sysproto.h/);
  print GEN $_."\n";
}

print GEN "
#include <ucontext.h>
#include <sys/time.h>

typedef int acl_type_t;
typedef int semid_t;
typedef unsigned int osigset_t;

typedef struct
{
  char*	str;
  int	size;
} param_size_t;

param_size_t	params_size[] =
  {
";

foreach (keys %type_h) {
  s/\"//g;
  s/^ //g;
  print GEN "\t{\"$_\", sizeof($_)},\n";
}

print GEN "
};
";
