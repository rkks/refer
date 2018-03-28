# $(call grep-string, search-string, word-list)
define grep-string
$(strip						\
  $(foreach w, $2,				\
    $(if $(findstring $1, $w),			\
      $w)))
endef
words := count_words.c counter.c lexer.l lexer.h counter.h
find-words:
	@echo $(call grep-string,un,$(words))
