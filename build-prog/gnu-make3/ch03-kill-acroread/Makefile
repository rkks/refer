AWK  := awk
KILL := kill

# $(kill-acroread)
define kill-acroread
  @ ps -W |                                           \
  $(AWK) 'BEGIN      { FIELDWIDTHS = "9 47 100" }     \
          /AcroRd32/ {                                \
                       print "Killing " $$3;          \
                       system( "$(KILL) -f " $$1 )    \
                     }'
endef
