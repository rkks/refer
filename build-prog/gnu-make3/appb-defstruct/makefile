include defstruct.mk

$(call defstruct, file-info,	\
  $(call defslot, path,),	\
  $(call defslot, type,unix),	\
  $(call defslot, host,oscar))

before := $(call new, file-info)
$(call set-value, before, path,/etc/password)
$(call set-value, before, host,wasatch)

after := $(call new,file-info)
$(call set-value, after, path,/etc/shadow)
$(call set-value, after, host,wasatch)

demo:
	# before       = $(before)
	# before.path  = $(call get-value, before, path)
	# before.type  = $(call get-value, before, type)
	# before.host  = $(call get-value, before, host)
	# print before = $(call print-instance, before)
	# dump before  = $(call dump-instance, before)
	#
	# all_instances   = $(all_instances)
	# all_structs     = $(all_structs)
	# print file-info = $(call print-struct, file-info)
	# dump file-info  = $(call dump-struct, file-info)
