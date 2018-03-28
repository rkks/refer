package Std::Modules;
use Filter::Macro;     # <-- The magic happens here

use strict;
use warnings;
use Carp;
use Smart::Comments;
use XML::Parser;
use File::Spec;
use IO::Prompt qw( prompt );
use File::Spec::Functions;
use File::Slurp qw( slurp );
use DateTime;
use DateTime::Duration;
use DateTime::TimeZone;
use DateTime::TimeZone::Antarctica::Mawson;
# etc.
# etc.

1;
