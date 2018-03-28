define parent
  echo "parent has two parameters: $1, $2"
  $(call child,$1)
endef

define child
  echo "child has one parameter: $1"
  echo "but child can also see parent's second parameter: $2!"
endef

scoping_issue:
	@$(call parent,one,two)
