# Please see README for documentation. This module is copyrighted
# as per the usual perl legalese:
# Copyright (c) 1997 Austin Schutz. All rights reserved. This program is free
# software; you can redistribute it and/or modify it under the same terms as
# Perl itself.
#
# Don't blame/flame me if you bust your stuff.
# Austin Schutz -  tex@habit.com

require 5; # 4 won't cut it. 

package Expect;

use IO::Pty; # This appears to require 5.004
use IO::Stty;

#use strict 'refs';
use strict 'vars';
use strict 'subs';
use POSIX; # For setsid. 
use Fcntl; # For checking file handle settings.

# This is necessary to make routines within Expect work.

@Expect::ISA= qw(IO::Pty);

BEGIN {
  $Expect::VERSION="1.07";
  # These are defaults which may be changed per object, or set as
  # the user wishes.
# This will be unset, since the default behavior differs between 
# spawned processes and initialized filehandles.
#  $Expect::Log_Stdout=1;
  $Expect::Log_Group=1;
  $Expect::Debug=0;
  $Expect::Exp_Internal=0;
  $Expect::Manual_Stty=0;
  $Expect::Multiline_Matching=1;
}

sub version {
  my($version)=shift;
  warn "Version $version is later than $Expect::VERSION. It may not be supported" if (defined ($version) && ($version > $Expect::VERSION));

  die "Versions before 1.03 are not supported in this release" if ((defined ($version)) && ($version < 1.03));
  return $Expect::VERSION;
}

sub new { goto &spawn; }# We should be called as Expect->spawn or spawn Expect

sub spawn {
  my($tty);
  my($name_of_tty);
  my ($class)=shift;
  # Create the pty which we will use to pass process info.
  my($self) = new IO::Pty;
  bless ($self,$class);
  my($cmd) = join(' ',@_); # spawn is passed command line args.
  $name_of_tty= $self->IO::Pty::ttyname();
  die "$class: Could not assign a pty" unless $self->IO::Pty::ttyname();
  $self->IO::Pty::autoflush();
  # This is defined here since the default is different for initialized
  # handles as opposed to spawned processes.
  ${*$self}{exp_Log_Stdout}=1;
  $self->_init_vars();
  ${*$self}{exp_Pid} = fork;
  unless (defined (${*$self}{exp_Pid})) {
    warn "Cannot fork: $!";
    return undef;
  }
  unless (${*$self}{exp_Pid}) {
    # Child
    # Create a new 'session', lose controlling terminal.
    POSIX::setsid() || warn "Couldn't perform setsid. Strange behavior may result.\r\n Problem: $!\r\n";
    $tty = $self->IO::Pty::slave(); # Create slave handle.
    $name_of_tty= $tty->ttyname();
    # We have to close everything and then reopen ttyname after to get
    # a controlling terminal.
    close($self);
    close STDIN; close STDOUT; close STDERR;
    open(STDIN,"<&". $tty->fileno()) || die "Couldn't reopen ". $name_of_tty ." for reading, $!\r\n";
    open(STDOUT,">&". $tty->fileno()) || die "Couldn't reopen ". $name_of_tty ." for writing, $!\r\n";
    open(STDERR,">&". fileno(STDOUT)) || die "Couldn't redirect STDERR, $!\r\n";
    exec ($cmd);
# End Child.
  }

# Parent

  # This is sort of for code compatibility, and to make debugging a little
  # easier. By code compatibility I mean that previously the process's
  # handle was referenced by $process{Pty_Handle} instead of just $process.
  # This is almost like 'naming' the handle to the process.
  # I think this also reflects Tcl Expect-like behavior.
  ${*$self}{exp_Pty_Handle}="spawn id(".$self->fileno().")";
  if ((${*$self}{"exp_Debug"})||(${*$self}{"exp_Exp_Internal"})) {
    print STDERR "Spawned '$cmd' (${*$self}{exp_Pty_Handle})\r\n";
    print STDERR "\tPid: ${*$self}{exp_Pid}\r\n";
    print STDERR "\tTty: ".$name_of_tty."\r\n";
  }
  return $self;
}


sub exp_init {
  # take a filehandle, for use later with expect() or interconnect() .
  # All the functions are written for reading from a tty, so if the naming
  # scheme looks odd, that's why.
  my ($class)=shift;
  my($self) = shift;
  bless $self, $class;
  die "exp_init not passed a file object, stopped" unless defined($self->fileno());
  $self->autoflush();
# Define standard variables.. debug states, etc.
  $self->_init_vars();
  # Turn of logging. By default we don't want crap from a file to get spewed
  # on screen as we read it.
  ${*$self}{exp_Log_Stdout}=0;
  ${*$self}{exp_Pty_Handle}="handle id(".$self->fileno().")";
  ${*$self}{exp_Pty_Handle}="STDIN" if $self->fileno() == fileno (STDIN);
  print STDERR "Initialized ${*$self}{exp_Pty_Handle}.'\r\n" if ${*$self}{"exp_Debug"};
  return $self;
}



# We're happy OOP people. No direct access to stuff.
sub debug {
  my($self)=shift;
  return ${*$self}{"exp_Debug"} unless defined($_[0]);
  ${*$self}{"exp_Debug"} = shift;
}

sub exp_internal {
  my($self)=shift;
  return ${*$self}{"exp_Exp_Internal"} unless defined($_[0]);
  ${*$self}{"exp_Exp_Internal"} = shift;
}

sub log_stdout {
  my($self)=shift;
  return ${*$self}{"exp_Log_Stdout"} unless defined($_[0]);
  ${*$self}{"exp_Log_Stdout"} = shift;
}

sub log_group {
  my($self)=shift;
  return ${*$self}{"exp_Log_Group"} unless defined($_[0]);
  ${*$self}{"exp_Log_Group"} = shift;
}

sub pid {
  my($self)=shift;
  return (${*$self}{exp_Pid}) if defined ${*$self}{exp_Pid};
  return undef; # This would probably happen anyway.
}

sub set_seq {
  # Set an escape sequence/function combo for a read handle for interconnect.
  # Ex: $read_handle->set_seq('',\&function,\@parameters); 
  my($self)=shift;
  my($escape_sequence,$function)=(shift,shift);
  ${${*$self}{exp_Function}}{$escape_sequence}=$function;
  if((!defined($function))||($function eq 'undef')) {
    ${${*$self}{exp_Function}}{$escape_sequence}=\&_undef;
  }
  ${${*$self}{exp_Parameters}}{$escape_sequence}=shift;
# This'll be a joy to execute. :)
  if ( ${*$self}{"exp_Debug"} ) {
    print STDERR "Escape seq. '".$escape_sequence;
    print STDERR "' function for ${*$self}{exp_Pty_Handle} set to '";
    print STDERR ${${*$self}{exp_Function}}{$escape_sequence};
    print STDERR "(".join(',',@_).")'\r\n";
  }
}

sub set_group {
  my($self)=shift;
  my($write_handle);
  # Make sure we can read from the read handle
  if(! defined($_[0])) {
    if (defined (${*$self}{exp_Listen_Group})) {
      return @{${*$self}{exp_Listen_Group}};
    } else {
      # Refrain from returning an undef
      return undef;
    }
  }
  @{${*$self}{exp_Listen_Group}}=();
  if($self->_get_mode()!~'r') {
    warn "Attempting to set a handle group on ${*$self}{exp_Pty_Handle}, a non-readable handle!\r\n";
  }
  while($write_handle = shift) { 
    if ($self->_get_mode()!~'w') {
       warn "Attempting to set a non-writeable listen handle ${*$write_handle}{exp_Pty_handle} for ${*$self}{exp_Pty_Handle}!\r\n";
    }
  push (@{${*$self}{exp_Listen_Group}},$write_handle);
  }
}
  

sub manual_stty {
# Let user do own stty setting. Return the name of the tty to use.
  my($self)=shift;
  return ${*$self}{"exp_Manual_Stty"} unless defined($_[0]);
  ${*$self}{"exp_Manual_Stty"} = shift;
}

sub max_accum {
  my($self)=shift;
  return ${*$self}{"exp_Max_Accum"} unless defined($_[0]);
  ${*$self}{"exp_Max_Accum"}=shift;
  if (${*$self}{"exp_Max_Accum"} eq 'undefine') {
    undef(${*$self}{"exp_Max_Accum"});
  }
}

# I'm going to leave this here in case I might need to change something.
# Previously this was calling `stty`, in a most bastardized manner.
sub exp_stty {
  my($self)=shift;
  my($mode)=shift;
  return undef unless defined($mode);
  if (${*$self}{"exp_Debug"}) {
    print STDERR "Setting ${*$self}{exp_Pty_Handle} to tty mode '$mode'\r\n";
  }
  unless (POSIX::isatty($self)) {
    if (${*$self}{"exp_Debug"}) {
      print STDERR "${*$self}{exp_Pty_Handle} is not a tty. Not changing mode\r\n";
    }
    return ''; # No undef to avoid warnings elsewhere.
  }
  IO::Stty::stty($self,split(/\s/,$mode));
}

# If we want to clear the buffer. Otherwise Accum will grow during send_slow
# etc. and contain the remainder after matches.
sub clear_accum {
  my($self)=shift;
  my ($temp)=(${*$self}{exp_Accum});
  ${*$self}{exp_Accum}='';
# return the contents of the accumulator.
  return $temp;
}

sub expect {
  # Here's the basic rundown on how the loop algorithm works:
  # Do loop continuously (while (1) {... )
  # 1. Test Accumulator for pattern End loop if pattern found.
  # 2. select() on handle.
  # 3. End loop if nothing is found (ERR = Timeout).
  # 4. Add bits found to accumulator.
  # 5. continue loop if loop_time is undef (loop forever)
  # 6. Decrement remaining time.
  my ($self)=shift; 
  my($loop_time, @patterns) = @_;
  my($current_time,$last_loop_time);
  my(@regexp_flags,@temp_patterns); 
  my($successful_pattern,$pattern_number)=(0,1);
  my($match, $before, $after, $err);
  my($rmask, $nfound, $nread);
  my($no_regexp_match_index,$pattern);

  # Let's make a list of patterns wanting to be evaled as regexps.
  while ( $pattern = shift(@patterns)) {
    if ( $pattern eq '-re' ) {
      push (@regexp_flags,1);
      push (@temp_patterns,shift(@patterns));
      next;
    }
    push (@regexp_flags,0);
    push (@temp_patterns,$pattern);
  }
  @patterns = @temp_patterns;

  print STDERR "Beginning expect from ${*$self}{exp_Pty_Handle}.\r\nAccumulator: '".$self->_trim_length(_make_readable((${*$self}{exp_Accum})))."'\r\n" if ${*$self}{"exp_Debug"};


  print STDERR "Expect timeout time: ".(defined($loop_time) ? $loop_time : "unlimited" )." seconds.\r\n" if ${*$self}{"exp_Debug"};
  # What happens if we want to go for 0 secs? We do a quick test on what is
  # ready on the handle.
  print STDERR "expect: Pty=${*$self}{exp_Pty_Handle}, time=",time,", loop_time=".(defined($loop_time) ? $loop_time : "undef")."\r\n" if ${*$self}{"exp_Debug"};

# What are we expecting? What do you expect? :-)
  if (${*$self}{"exp_Exp_Internal"}) {
    print STDERR "Expecting ";
    print STDERR "from ${*$self}{exp_Pty_Handle}:";
    foreach $pattern (@patterns) {
      print STDERR ' -re' if $regexp_flags[$pattern_number - 1];
      print STDERR " '"._make_readable($pattern)."'";
      $pattern_number++;
    }
    print STDERR "\r\n";
  }
  $pattern_number = 1;

# Set the last loop time to now for time comparisons at the end of the loop.
$last_loop_time = time();
# Initialize accumulator.
${*$self}{exp_Accum} = '' unless defined(${*$self}{exp_Accum});
READLOOP:
  while (1) {
    # Test for a match first so we can test the current Accum w/out worrying
    # about an EOF.
    if (defined(${*$self}{"exp_Max_Accum"})) {
      ${*$self}{exp_Accum}=$self->_trim_length(${*$self}{exp_Accum},${*$self}{"exp_Max_Accum"});
    }
    $pattern_number = 1;
    # Does the user want to watch the patterns?
    if (${*$self}{"exp_Exp_Internal"}) {
# This could be huge. We should attempt to do something about this. 
# Because the output is used for debugging I'm of the opinion that showing
# smaller amounts if the total is huge should be ok.
# Thus the 'trim_length'
      print STDERR "Does '";
      print STDERR $self->_trim_length(_make_readable(${*$self}{exp_Accum}));
      print STDERR "'\r\nfrom ${*$self}{exp_Pty_Handle} match:\r\n";
    }
    for $pattern ( @patterns ) {
      if (${*$self}{"exp_Exp_Internal"}) {
        print STDERR "\tpattern $pattern_number \(";
        print STDERR '-re ' if $regexp_flags[$pattern_number - 1];
        print STDERR "'".$pattern."'\)? " ;
      }
      # Matching exactly
      if (!$regexp_flags[$pattern_number - 1]) {
        $no_regexp_match_index = index(${*$self}{exp_Accum},$pattern);
        # We matched if $no_regexp_match_index > -1
        if ($no_regexp_match_index > -1) {
          $before = substr(${*$self}{exp_Accum},0,$no_regexp_match_index);
          $match = substr(${*$self}{exp_Accum},$no_regexp_match_index,
            length($pattern));
          $after = substr(${*$self}{exp_Accum},
            $no_regexp_match_index + length($pattern)) ;
          ${*$self}{exp_Before} = $before;
          ${*$self}{exp_Match} = $match;
          ${*$self}{exp_After} = $after;
          $successful_pattern=$pattern_number;
        }
      } elsif ( ($Expect::Multiline_Matching) && 
      (${*$self}{exp_Accum} =~ /$pattern/m )) {
        # Matching regexp
        ( $match, $before, $after ) = ( $&, $`, $' );
        ${*$self}{exp_Before} = $before;
        ${*$self}{exp_Match} = $match;
        ${*$self}{exp_After} = $after;
        $successful_pattern=$pattern_number;
      } elsif (${*$self}{exp_Accum} =~ /$pattern/ ) {
        # Matching regexp
        ( $match, $before, $after ) = ( $&, $`, $' );
        ${*$self}{exp_Before} = $before;
        ${*$self}{exp_Match} = $match;
        ${*$self}{exp_After} = $after;
        $successful_pattern=$pattern_number;
      }
      if ($successful_pattern) {
        ${*$self}{exp_Accum} = $after;
        print STDERR "Yes!\r\n" if (${*$self}{"exp_Exp_Internal"});
        # The exclamation point makes it stick out. And gets me excited.
        if ((${*$self}{"exp_Exp_Internal"})||(${*$self}{"exp_Debug"})) {
          print STDERR "Matched pattern $successful_pattern ";
          print STDERR "(";
          print STDERR '-re ' if $regexp_flags[$successful_pattern - 1];
          print STDERR "\'$pattern\')!\r\n";
          print STDERR "\tBefore match string: '";
          print STDERR $self->_trim_length(_make_readable(($before)))."'\r\n";
          print STDERR "\tMatch string: '"._make_readable($match)."'\r\n";
          print STDERR "\tAfter match string: '";
          print STDERR $self->_trim_length(_make_readable(($after)))."'\r\n";
        }
        last READLOOP;
      }
      print STDERR "No.\r\n" if (${*$self}{"exp_Exp_Internal"});
      $pattern_number++; 
    }
   # End of matching section

    # Read from handle section.
    $rmask = '';
    vec($rmask,$self->fileno(),1) = 1;
    ($nfound) = select($rmask, undef, undef, $loop_time);
    # go until we don't find something (or time runs out).
    last READLOOP unless $nfound;
    print STDERR "expect: handle ${*$self}{exp_Pty_Handle} ready.\r\n" if (${*$self}{"exp_Debug"});
    # read in what we found.
    $nread = sysread($self, ${*$self}{exp_Pty_Buffer}, 2048);
    # Make errors (nread undef) show up as EOF.
    $nread = 0 unless defined ($nread);
    unless ($nread) {
      $before = $self->clear_accum();
      ${*$self}{exp_Before} = $before;
      $err = "2:EOF";
      last READLOOP;
    }
    print STDERR "expect: read $nread byte(s) from ${*$self}{exp_Pty_Handle}.\r\n" if (${*$self}{"exp_Debug"}>1);
    ${*$self}{exp_Accum} .= ${*$self}{exp_Pty_Buffer};
    $self->_print_handles(${*$self}{exp_Pty_Buffer});
    # End handle reading section.

    # Adjust remaining time. Skip this if loop_time = undef.
    next READLOOP unless defined ($loop_time);
    $current_time = time();
    $loop_time -= ($current_time - $last_loop_time);
    # Case: t < 0, t == 0 , t < 1
    $loop_time = 0 if $loop_time < 1;
    $last_loop_time = $current_time;
  }
# End READLOOP

  # Post loop. Do we have anything?
  # No pattern, no EOF. Did we time out or is the process dead?
  if ((!$successful_pattern) && (!$err)) {
    $before = ${*$self}{exp_Accum};
    ${*$self}{exp_Before} = $before;
    # is it dead?
    if (defined(${*$self}{exp_Pid})) {
      waitpid(${*$self}{exp_Pid},WNOHANG);
      if ( !kill( 0, ${*$self}{exp_Pid} ) ) {
        $before = $self->clear_accum(); # Don't bother saving Accum. It's dead.
        ${*$self}{exp_Before} = $before;
        $err = "3:Child process ${*$self}{exp_Pid} died before matching";
      }
    }
    $err = "1:TIMEOUT" unless $err;
  }

  # Tell us status
  if ( ${*$self}{"exp_Debug"} || ${*$self}{exp_Exp_Internal} ) {
    print STDERR "Returning from expect ";
    print STDERR ( $err ? 'un' : '' );
    print STDERR "successfully.";
    print STDERR ( $err ? " Error: $err." : '' )."\r\n";
    if (${*$self}{"exp_Debug"}) {
        print STDERR "Accumulator: '";
      if ($err) {
        print STDERR $self->_trim_length(_make_readable($before))."'\r\n";
      } else {
        print STDERR $self->_trim_length(_make_readable(${*$self}{exp_Accum}))."'\r\n";
      }
    }
  }

  $successful_pattern = undef if $err; # Sanity check

  # Save for later
  ${*$self}{exp_Match_Number} = $successful_pattern;
  ${*$self}{exp_Error} = $err;
  if ( wantarray ) {
    return ( $successful_pattern, $err, $match, $before, $after );
  }
  return $successful_pattern;
}

# This is new in 1.06. This makes it possible to see the results of the last
# expect()
sub exp_match_number {
  my($self) = shift;
  return ${*$self}{exp_Match_Number};
}
sub exp_error {
  my($self) = shift;
  return ${*$self}{exp_Error};
}
sub exp_before {
  my($self) = shift;
  return ${*$self}{exp_Before};
}
sub exp_match {
  my($self) = shift;
  return ${*$self}{exp_Match};
}
sub exp_after {
  my($self) = shift;
  return ${*$self}{exp_After};
}


# $process->interact([$in_handle],[$escape sequence])
# If you don't specify in_handle STDIN  will be used.
sub interact {
  my ($self)=(shift);
  my ($infile)=(shift);
  my ($escape_sequence)=shift;
  my ($in_object,$in_handle,@old_group,$return_value);
  my ($old_manual_stty_val,$old_log_stdout_val);
  my ($outfile,$out_object);
  @old_group = $self->set_group();
  # If the handle is STDIN we'll
  # $infile->fileno == 0 should be stdin.. follow stdin rules.
  no strict 'subs'; # Allow bare word 'STDIN'
  unless (defined($infile)) {
    # We need a handle object Associated with STDIN.
    $infile = new IO::File;
    $infile->IO::File::fdopen(STDIN,'r');
    $outfile = new IO::File;
    $outfile->IO::File::fdopen(STDOUT,'w');
  } elsif (fileno($infile) == fileno(STDIN)) {
    # With STDIN we want output to go to stdout.
    $outfile = new IO::File;
    $outfile->IO::File::fdopen(STDOUT,'w');
  } else {
    undef ($outfile);
  }
  # Here we assure ourselves we have an Expect object.
  $in_object = Expect->exp_init($infile);
  if (defined($outfile)) {
    # as above.. we want output to go to stdout if we're given stdin.
    $out_object = Expect->exp_init($outfile);
    $out_object->manual_stty(1);
    $self->set_group($out_object);
  } else {
    $self->set_group($in_object);
  }
  $in_object->set_group($self);
  $in_object->set_seq($escape_sequence,undef) if defined($escape_sequence);
  # interconnect normally sets stty -echo raw. Interact really sort
  # of implies we don't do that by default. If anyone wanted to they could
  # set it before calling interact, of use interconnect directly.
  $old_manual_stty_val =$self->manual_stty();
  $self->manual_stty(1);
  # I think this is right. Don't send stuff from in_obj to stdout by default.
  # in theory whatever 'self' is should echo what's going on.
  $old_log_stdout_val=$self->log_stdout();
  $self->log_stdout(0);
  $in_object->log_stdout(0);
# Allow for the setting of an optional EOF escape function.
#  $in_object->set_seq('EOF',undef);
#  $self->set_seq('EOF',undef);
  Expect::interconnect($self,$in_object);
  $self->log_stdout($old_log_stdout_val);
  $self->set_group(@old_group);
# If old_group was undef, make sure that occurs. This is a slight hack since
# it modifies the value directly.
# Normally an undef passed to set_group will return the current groups.
# It is possible that it may be of worth to make it possible to undef
# The current group without doing this.
  unless (defined (@old_group)) {
    @{${*$self}{exp_Listen_Group}} = ();
  }
  $self->manual_stty($old_manual_stty_val);
  return $return_value;
}

sub interconnect {

#  my ($handle)=(shift); call as Expect::interconnect($spawn1,$spawn2,...)
  my ($rmask,$nfound,$nread);
  my ($rout, @bits, $emask, $eout, @ebits ) = ();
  my ($escape_sequence,$escape_character_buffer,$offset);
  my (@handles)=@_;
  my ($handle,$read_handle,$write_handle);
  my ($read_mask,$temp_mask)=('','');

# Get read/write handles
  foreach $handle(@handles) {
    $temp_mask='';
    vec($temp_mask,$handle->fileno(),1) = 1;
    # Under Linux w/ 5.001 the next line comes up w/ 'Uninit var.'.
    # It appears to be impossible to make the warning go away.
    # doing something like $temp_mask='' unless defined ($temp_mask)
    # has no effect whatsoever. This may be a bug in 5.001.
    $read_mask= $read_mask | $temp_mask;
  }
  if($Expect::Debug) {
    print STDERR "Read handles:\r\n";
    foreach $handle(@handles) {
      print STDERR "\tRead handle: ";
      print STDERR "'${*$handle}{exp_Pty_Handle}'\r\n";
      print STDERR "\t\tListen Handles:";
      foreach $write_handle(@{${*$handle}{exp_Listen_Group}}) {
        print STDERR " '${*$write_handle}{exp_Pty_Handle}'";
      }
      print STDERR ".\r\n";
    }
  }

#  I think if we don't set raw/-echo here we may have trouble. We don't 
# want a bunch of echoing crap making all the handles jabber at each other.
  foreach $handle(@handles) {
    unless (${*$handle}{"exp_Manual_Stty"}) {
      # This is probably O/S specific.
      ${*$handle}{exp_Stored_Stty}=$handle->exp_stty('-g');
      print STDERR "Setting tty for ${*$handle}{exp_Pty_Handle} to 'raw -echo'.\r\n"if ${*$handle}{"exp_Debug"};
      $handle->exp_stty("raw -echo");
    }
    foreach $write_handle (@{${*$handle}{exp_Listen_Group}}) {
      unless (${*$write_handle}{"exp_Manual_Stty"}) {
        ${*$write_handle}{exp_Stored_Stty}=$write_handle->exp_stty('-g');
        print STDERR "Setting ${*$write_handle}{exp_Pty_Handle} to 'raw -echo'.\r\n"if ${*$handle}{"exp_Debug"};
        $write_handle->exp_stty("raw -echo");
      }
    }
  }

  print STDERR "Attempting interconnection\r\n" if $Expect::Debug;

# Wait until the process dies or we get EOF
# In the case of !${*$handle}{exp_Pid} it means
# the handle was exp_inited instead of spawned.
CONNECT_LOOP:
  # Go until we have a reason to stop
  while (1) {
# test each handle to see if it's still alive.
    foreach $read_handle (@handles) {
      waitpid(${*$read_handle}{exp_Pid}, WNOHANG) if (defined (${*$read_handle}{exp_Pid})&&${*$read_handle}{exp_Pid});
      if (defined(${*$read_handle}{exp_Pid})&&(${*$read_handle}{exp_Pid})&&(! kill(0,${*$read_handle}{exp_Pid}))) {
        print STDERR "Got EOF (${*$read_handle}{exp_Pty_Handle} died) reading ${*$read_handle}{exp_Pty_Handle}\r\n"if ${*$read_handle}{"exp_Debug"};
        last CONNECT_LOOP unless defined(${${*$read_handle}{exp_Function}}{"EOF"});
        last CONNECT_LOOP unless &{${${*$read_handle}{exp_Function}}{"EOF"}}(@{${${*$read_handle}{exp_Parameters}}{"EOF"}});
      }
    }

# Every second? No, go until we get something from someone.
    ($nfound) = select($rout=$read_mask, undef, $eout=$emask, undef);
    # Is there anything to share?
    next CONNECT_LOOP unless $nfound;
    # Which handles have stuff?
    @bits = split(//,unpack('b*',$rout));
    $eout= 0 unless defined ($eout);
    @ebits = split(//,unpack('b*',$eout));
#    print "Ebits: $eout\r\n";
    foreach $read_handle(@handles) {
      if ($bits[$read_handle->fileno()]) {
        $nread=sysread( $read_handle, ${*$read_handle}{exp_Pty_Buffer}, 1024 );
        # Appease perl -w
        $nread = 0 unless defined ($nread);
        print STDERR "interconnect: read $nread byte(s) from ${*$read_handle}{exp_Pty_Handle}.\r\n" if ${*$read_handle}{"exp_Debug"}>1;
        # Test for escape seq. before printing.
        # Appease perl -w
        $escape_character_buffer = '' unless defined ($escape_character_buffer);
        $escape_character_buffer .=${*$read_handle}{exp_Pty_Buffer};
        foreach $escape_sequence (keys(%{${*$read_handle}{exp_Function}})) {
        print STDERR "Tested escape sequence $escape_sequence from ${*$read_handle}{exp_Pty_Handle}"if ${*$read_handle}{"exp_Debug"}>1;
          # Make sure it doesn't grow out of bounds.
          if ((defined(${*$read_handle}{"exp_Max_Accum"})&&(length($escape_character_buffer))>${*$read_handle}{"exp_Max_Accum"})) {
            $offset = length($escape_character_buffer) - ${*$read_handle}{"exp_Max_Accum"};
            $escape_character_buffer = substr($escape_character_buffer,$offset,${*$read_handle}{"exp_Max_Accum"});
          }
          if ($escape_character_buffer =~ /($escape_sequence)/) {
            if (${*$read_handle}{"exp_Debug"}) {
              print STDERR "\r\ninterconnect got escape sequence from ${*$read_handle}{exp_Pty_Handle}.\r\n";
              # I'm going to make the esc. seq. pretty because it will 
              # probably contain unprintable characters.
              print STDERR "\tEscape Sequence: '"._trim_length(undef,_make_readable($escape_sequence))."'\r\n";
              print STDERR "\tMatched by string: '"._trim_length(undef,_make_readable($&))."'\r\n";
            }
            # Print out stuff before the escape.
            # Keep in mind that the sequence may have been split up
            # over several reads.
            # Let's get rid of it from this read. If part of it was 
            # in the last read there's not a lot we can do about it now.
            if (${*$read_handle}{exp_Pty_Buffer}=~ /($escape_sequence)/) {
              $read_handle->_print_handles($`);
            } else {
              $read_handle->_print_handles(${*$read_handle}{exp_Pty_Buffer})
            }
            # Clear the buffer so no more matches can be made and it will
            # only be printed one time.
            ${*$read_handle}{exp_Pty_Buffer}='';
            $escape_character_buffer='';
            # Do the function here. Must return non-zero to continue.
            # More cool syntax. Maybe I should turn these in to objects.
            last CONNECT_LOOP unless &{${${*$read_handle}{exp_Function}}{$escape_sequence}}(@{${${*$read_handle}{exp_Parameters}}{$escape_sequence}});
          }
        }
        $nread = 0 unless defined($nread); # Appease perl -w?
        waitpid(${*$read_handle}{exp_Pid}, WNOHANG) if (defined (${*$read_handle}{exp_Pid})&&${*$read_handle}{exp_Pid});
        if ($nread == 0) {
          print STDERR "Got EOF reading ${*$read_handle}{exp_Pty_Handle}\r\n"if ${*$read_handle}{"exp_Debug"}; 
          last CONNECT_LOOP unless defined(${${*$read_handle}{exp_Function}}{"EOF"});
          last CONNECT_LOOP unless &{${${*$read_handle}{exp_Function}}{"EOF"}}(@{${${*$read_handle}{exp_Parameters}}{"EOF"}});
        }
        last CONNECT_LOOP if ($nread < 0); # This would be an error
        $read_handle->_print_handles(${*$read_handle}{exp_Pty_Buffer});
      }
      # I'm removing this because I haven't determined what causes exceptions
      # consistently.
      if (0)#$ebits[$read_handle->fileno()])
      {
        print STDERR "Got Exception reading ${*$read_handle}{exp_Pty_Handle}\r\n"if ${*$read_handle}{"exp_Debug"};
        last CONNECT_LOOP unless defined(${${*$read_handle}{exp_Function}}{"EOF"});
        last CONNECT_LOOP unless &{${${*$read_handle}{exp_Function}}{"EOF"}}(@{${${*$read_handle}{exp_Parameters}}{"EOF"}});
      }
    }
  }
  foreach $handle(@handles) {
    unless (${*$handle}{"exp_Manual_Stty"}) {
      $handle->exp_stty(${*$handle}{exp_Stored_Stty});
    }
    foreach $write_handle (@{${*$handle}{exp_Listen_Group}}) {
      unless (${*$write_handle}{"exp_Manual_Stty"}) {
        $write_handle->exp_stty(${*$write_handle}{exp_Stored_Stty});
      }
    }
  }
}


# This is an Expect standard. It's nice for talking to modems and the like
# where from time to time they get unhappy if you send items too quickly.
sub send_slow{
  my ($self)=shift;
  my($char,@linechars,$nfound,$rmask);
  my($sleep_time)=shift;
# Flushing makes it so each character can be seen separately.
  while ($_=shift) {
    @linechars = split ('');
    foreach $char (@linechars) {
#     How slow?
      select (undef,undef,undef,$sleep_time);

      print $self $char;
      print STDERR "Printed character \'"._make_readable($char)."\' to ${*$self}{exp_Pty_Handle}.\r\n" if ${*$self}{"exp_Debug"}>1;
      # I think I can get away with this if I save it in accum
      if (${*$self}{"exp_Log_Stdout"}||${*$self}{exp_Log_Group}) {
        $rmask = "";
        vec($rmask,$self->fileno(),1)=1;
        # .01 sec granularity should work. If we miss something it will
        # probably get flushed later, maybe in an expect call.
        while (select($rmask,undef,undef,.01)) {
          sysread($self,${*$self}{exp_Pty_Buffer},1024);
          # Is this necessary to keep? Probably.. #
          # if you need to expect it later.
          ${*$self}{exp_Accum}.=${*$self}{exp_Pty_Buffer};
          ${*$self}{exp_Accum}=$self->_trim_length(${*$self}{exp_Accum},${*$self}{"exp_Max_Accum"}) if defined (${*$self}{"exp_Max_Accum"});
          $self->_print_handles(${*$self}{exp_Pty_Buffer});
          print STDERR "Received \'".$self->_trim_length(_make_readable($char))."\' from ${*$self}{exp_Pty_Handle}\r\n" if ${*$self}{"exp_Debug"}>1;
        }
      }
    }
  }
}

sub test_handles {
  # This should be called by Expect::test_handles($timeout,@objects);
  my ($rmask, $allmask, $rout, $nfound, @bits, @return_list, $handle_num);
  my ($timeout)=shift;
  my (@handle_list)=@_;
  my($handle);
  foreach $handle (@handle_list) {
    $rmask = '';
    vec($rmask,$handle->fileno(),1) = 1;
    $allmask = '' unless defined ($allmask);
    $allmask = $allmask | $rmask;
  }
  ($nfound) = select($rout=$allmask, undef, undef, $timeout);
  return undef unless $nfound;
  # Which handles have stuff?
  @bits = split(//,unpack('b*',$rout));
  foreach $handle (@handle_list) {
    # I go to great lengths to get perl -w to shut the hell up.
    if (defined($bits[$handle->fileno()])&&($bits[$handle->fileno()])) {
      $handle_num = 0 unless defined($handle_num); # Have it return a numeric.
      push(@return_list,$handle_num);
    }
    $handle_num++;
  }
  return (@return_list);
}

# Be nice close. This should emulate what an interactive shell does after a
# command finishes... sort of. We're not as patient as a shell.
sub soft_close {
  my($self) = shift;
  my($nfound,$nread,$rmask,$returned_pid);
  my($end_time,$select_time,$temp_buffer);
  my($close_status);
  # Give it 15 seconds to cough up an eof.
  print STDERR "Closing ${*$self}{exp_Pty_Handle}.\r\n" if ${*$self}{exp_Debug};
  $end_time = time() + 15;
  while ($end_time > time()) {
    $select_time = $end_time - time();
    # Sanity check.
    $select_time = 0 if $select_time < 0;
    $rmask = '';
    vec($rmask,$self->fileno(),1)=1;
    ($nfound) = select($rmask,undef,undef,$select_time);
    last unless (defined($nfound) && $nfound);
    $nread = sysread($self,$temp_buffer,8096);
    # 0 = EOF.
    unless (defined($nread) && $nread) {
      print STDERR "Got EOF from ${*$self}{exp_Pty_Handle}.\r\n" if ${*$self}{exp_Debug};
      last;
    }
    $self->_print_handles($temp_buffer);
  }
  if (($end_time <= time()) && ${*$self}{exp_Debug}) {
    print STDERR "Timed out waiting for an EOF from ${*$self}{exp_Pty_Handle}.\r\n";
  }
  if ( ($close_status=$self->close()) && ${*$self}{exp_Debug}) {
    print STDERR "${*$self}{exp_Pty_Handle} closed.\r\n";
  }
  # quit now if it isn't a process.
  return $close_status unless defined($self->pid());
  # Now give it 15 seconds to die.
  $end_time = time() + 15;
  while ($end_time > time()) {
    $returned_pid = waitpid($self->pid(),&WNOHANG);
    # Stop here if the process dies.
    if (defined($returned_pid) && $returned_pid) {
      if (${*$self}{exp_Debug}) {
        print STDERR "Pid ${*$self}{exp_Pid} of ${*$self}{exp_Pty_Handle} exited, Status: $?\r\n";
      }
      return $?;
    }
    sleep 1; # Keep loop nice.
  }
  # Send it a term if it isn't dead.
  if (${*$self}{exp_Debug}) {
    print STDERR "${*$self}{exp_Pty_Handle} not exiting, sending TERM.\r\n";
  }
  kill(15,${*$self}{exp_Pid});
  # Now to be anal retentive.. wait 15 more seconds for it to die.
  $end_time = time() + 15;
  while ($end_time > time()) {
    $returned_pid = waitpid($self->pid(),&WNOHANG);
    if (defined($returned_pid) && $returned_pid) {
      if (${*$self}{exp_Debug}) {
        print STDERR "Pid ${*$self}{exp_Pid} of ${*$self}{exp_Pty_Handle} terminated, Status: $?\r\n";
      }
      return $?;
    }
    sleep 1;
  }
  # Since this is a 'soft' close, sending it a -9 would be inappropriate.
  return undef;
}

# 'Make it go away' close.
sub hard_close {
  my($self) = shift;
  my($nfound,$nread,$rmask,$returned_pid);
  my($end_time,$select_time,$temp_buffer);
  my($close_status);
  print STDERR "Closing ${*$self}{exp_Pty_Handle}.\r\n" if ${*$self}{exp_Debug};
  # Don't wait for an EOF.
  if ( ($close_status=$self->close()) && ${*$self}{exp_Debug}) {
    print STDERR "${*$self}{exp_Pty_Handle} closed.\r\n";
  }
  # Return now if handle.
  return $close_status unless defined($self->pid());
  # Now give it 5 seconds to die. Less patience here if it won't die.
  $end_time = time() + 5;
  while ($end_time > time()) {
    $returned_pid = waitpid($self->pid(),&WNOHANG);
    # Stop here if the process dies.
    if (defined($returned_pid) && $returned_pid) {
      if (${*$self}{exp_Debug}) {
        print STDERR "Pid ${*$self}{exp_Pid} of ${*$self}{exp_Pty_Handle} exited, Status: $?\r\n";
      }
      return $?;
    }
    sleep 1; # Keep loop nice.
  }
  # Send it a term if it isn't dead.
  if (${*$self}{exp_Debug}) {
    print STDERR "${*$self}{exp_Pty_Handle} not exiting, sending TERM.\r\n";
  }
  kill(15,${*$self}{exp_Pid});
  # wait 15 more seconds for it to die.
  $end_time = time() + 15;
  while ($end_time > time()) {
    $returned_pid = waitpid($self->pid(),&WNOHANG);
    if (defined($returned_pid) && $returned_pid) {
      if (${*$self}{exp_Debug}) {
        print STDERR "Pid ${*$self}{exp_Pid} of ${*$self}{exp_Pty_Handle} terminated, Status: $?\r\n";
      }
      return $?;
    }
    sleep 1;
  }
  kill(9,${*$self}{exp_Pid});
  # wait 5 more seconds for it to die.
  $end_time = time() + 5;
  while ($end_time > time()) {
    $returned_pid = waitpid($self->pid(),&WNOHANG);
    if (defined($returned_pid) && $returned_pid) {
      if (${*$self}{exp_Debug}) {
        print STDERR "Pid ${*$self}{exp_Pid} of ${*$self}{exp_Pty_Handle} killed, Status: $?\r\n";
      }
      return $?;
    }
    sleep 1;
  }
  warn "Pid ${*$self}{exp_Pid} of ${*$self}{exp_Pty_Handle} is HUNG.\r\n";
  return undef;
}
    
# These should not be called externally.

sub _init_vars {
#  print join(' ',@_,"\n");exit;
  my($self)=shift;
# for every spawned process or filehandle.
  ${*$self}{"exp_Log_Stdout"}=$Expect::Log_Stdout if defined ($Expect::Log_Stdout);
  ${*$self}{"exp_Log_Group"}=$Expect::Log_Group;
  ${*$self}{"exp_Debug"}=$Expect::Debug;
  ${*$self}{"exp_Exp_Internal"}=$Expect::Exp_Internal;
  ${*$self}{"exp_Manual_Stty"}=$Expect::Manual_Stty;
  ${*$self}{exp_Stored_Stty}='sane';
  # sysread doesn't like my or local vars.
  ${*$self}{exp_Pty_Buffer}=''; 
}


sub _make_readable {
  $_=shift;
  $_='' unless defined ($_);
  study; # Speed things up?
  s/\\/\\\\/g; # So we can tell easily(?) what is a backslash
  s/\n/\\n/g;
  s/\r/\\r/g;
  s/\t/\\t/g;
  s/\0/\\0/g;
  s/\'/\\\'/g; # So we can tell whassa quote and whassa notta quote.
  s/\"/\\\"/g;
  # Formfeed (does anyone use formfeed?)
  s/\f/\\f/g;
  # High / low ascii
  while(/([\177-\377\001-\037])/) {
    my ($equiv)=sprintf("%lo",ord($1));
    while (length ($equiv) < 3) {
      $equiv = '0'.$equiv;
    }
    s/[\200-\377\001-\037]/\\$equiv/; # Only match first one.
  }	
  return $_;
}

sub _trim_length {
  # This is sort of a reverse truncation function
  # Mostly so we don't have to see the full output when we're using
  # Also used if Max_Accum gets set to limit the size of the accumulator
  # for matching functions. 
  # exp_internal
  my($self)=shift;
  my($string)=shift;
  my($length)=shift;

# If we're not passed a length (_trim_length is being used for debugging 
# purposes) AND debug >= 3, don't trim.
  return($string) if (defined ($self) && 
      ${*$self}{"exp_Debug"} >=3 && (!(defined($length))));
  my($indicate_truncation)='...' unless $length;
  $length = 1021 unless $length;
  return($string) unless $length < length($string);
  # We wouldn't want the accumulator to begin with '...' if max_accum is passed
  # This is because this funct. gets called internally w/ max_accum
  # and is also used to print information back to the user.
  return $indicate_truncation.substr($string,(length($string)-$length),$length);
}

sub _print_handles {
  # Given crap from 'self' and the handles self wants to print to, print to
  # them. these are indicated by the handle's 'group'
  my($self)=shift;
  my($print_this)=shift;
  my($handle);
  if (${*$self}{exp_Log_Group}) {
    foreach $handle(@{${*$self}{exp_Listen_Group}}) {
      $print_this='' unless defined ($print_this);
      # Appease perl -w
      print STDERR "Printed '".$self->_trim_length(_make_readable($print_this))."' to ${*$handle}{exp_Pty_Handle} from ${*$self}{exp_Pty_Handle}.\r\n" if (${*$handle}{"exp_Debug"}>1);
      print $handle $print_this;
    }
  }
  # If ${*$self}{exp_Pty_Handle} is STDIN this would make it echo.
  print $print_this if ${*$self}{"exp_Log_Stdout"};
  $|=1; # This should not be necessary but autoflush() doesn't always work.
}

sub _get_mode {
  my($fcntl_flags)='';
  my($handle)=shift;
# What mode are we opening with? use fcntl to find out.
  $fcntl_flags=fcntl(\*{$handle},Fcntl::F_GETFL,$fcntl_flags);
  die "fcntl returned undef during exp_init of $handle, $!\r\n" unless defined($fcntl_flags);
  if($fcntl_flags|(Fcntl::O_RDWR)) {
    return 'rw';
  } elsif ($fcntl_flags|(Fcntl::O_WRONLY)) {
    return 'w'
  } else {
  # Under Solaris (among others?) O_RDONLY is implemented as 0. so |O_RDONLY would fail.
    return 'r';
  }
}


sub _undef {
 return undef;
# Seems a little retarded but &CORE::undef fails in interconnect.
# This is used for the default escape sequence function.
# w/out the leading & it won't compile.
}

__END__
