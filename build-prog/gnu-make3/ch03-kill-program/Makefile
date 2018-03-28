AWK        := awk
KILL       := kill
KILL_FLAGS := -f
PS         := ps
PS_FLAGS   := -W
PS_FIELDS  := "9 47 100"

# $(call kill-program,awk-pattern)
define kill-program
  @ $(PS) $(PS_FLAGS) |                                  \
  $(AWK) 'BEGIN { FIELDWIDTHS = $(PS_FIELDS) }           \
          /$1/  {                                        \
                  print "Killing " $$3;                  \
                  system( "$(KILL) $(KILL_FLAGS) " $$1 ) \
                }'
endef
