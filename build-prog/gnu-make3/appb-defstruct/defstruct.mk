# $(next-id) - return a unique number
next_id_counter :=
define next-id
$(words $(next_id_counter))$(eval next_id_counter += 1)
endef

# all_structs - a list of the defined structure names
all_structs :=

value_sep := XxSepxX

# $(call defstruct, struct_name, $(call defslot, slot_name, value), ...)
define defstruct
  $(eval all_structs += $1) 						\
  $(eval $1_def_slotnames :=)						\
  $(foreach v, $2 $3 $4 $5 $6 $7 $8 $9 $(10) $(11),			\
    $(if $($v_name),							\
      $(eval $1_def_slotnames          += $($v_name))			\
      $(eval $1_def_$($v_name)_default := $($v_value))))
endef

# $(call defslot,slot_name,slot_value)
define defslot
  $(eval tmp_id := $(next_id))
  $(eval $1_$(tmp_id)_name := $1)
  $(eval $1_$(tmp_id)_value := $2)
  $1_$(tmp_id)
endef

# all_instances - a list of all the instances of any structure
all_instances :=

# $(call new, struct_name)
define new
$(strip								\
  $(if $(filter $1,$(all_structs)),,				\
    $(error new on unknown struct '$(strip $1)'))		\
  $(eval instance := $1@$(next-id))				\
  $(eval all_instances += $(instance))				\
  $(foreach v, $($(strip $1)_def_slotnames),			\
    $(eval $(instance)_$v := $($(strip $1)_def_$v_default)))	\
  $(instance))
endef

# $(call delete, variable)
define delete
$(strip									 \
  $(if $(filter $($(strip $1)),$(all_instances)),,			 \
    $(error Invalid instance '$($(strip $1))'))				 \
  $(eval all_instances := $(filter-out $($(strip $1)),$(all_instances))) \
  $(foreach v, $($(strip $1)_def_slotnames),				 \
    $(eval $(instance)_$v := )))
endef

# $(call struct-name, instance_id)
define struct-name
$(firstword $(subst @, ,$($(strip $1))))
endef

# $(call check-params, instance_id, slot_name)
define check-params
  $(if $(filter $($(strip $1)),$(all_instances)),,		\
    $(error Invalid instance '$(strip $1)'))			\
  $(if $(filter $2,$($(call struct-name,$1)_def_slotnames)),,	\
    $(error Instance '$($(strip $1))' does not have slot '$(strip $2)'))
endef

# $(call get-value, instance_id, slot_name)
define get-value
$(strip				\
  $(call check-params,$1,$2) 	\
  $($($(strip $1))_$(strip $2)))
endef

# $(call set-value, instance_id, slot_name, value)
define set-value
  $(call check-params,$1,$2) \
  $(eval $($(strip $1))_$(strip $2) := $3)
endef

# $(call dump-struct, struct_name)
define dump-struct
{ $(strip $1)_def_slotnames "$($(strip $1)_def_slotnames)" 	\
  $(foreach s,							\
    $($(strip $1)_def_slotnames),$(strip 			\
    $(strip $1)_def_$s_default "$($(strip $1)_def_$s_default)")) }
endef

# $(call print-struct, struct_name)
define print-struct
{ $(foreach s,					\
    $($(strip $1)_def_slotnames),$(strip	\
    { "$s" "$($(strip $1)_def_$s_default)" })) }
endef

# $(call dump-instance, instance_id)
define dump-instance
{ $(eval tmp_name := $(call struct-name,$1)) 	\
  $(foreach s,					\
    $($(tmp_name)_def_slotnames),$(strip 	\
    { $($(strip $1))_$s "$($($(strip $1))_$s)" })) }
endef

# $(call print-instance, instance_id)
define print-instance
{ $(foreach s,						\
    $($(call struct-name,$1)_def_slotnames),"$(strip 	\
    $(call get-value,$1,$s))") }
endef
