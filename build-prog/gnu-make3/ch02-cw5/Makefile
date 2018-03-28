VPATH 	 = src include
CPPFLAGS = -I include

count_words: count_words.o counter.o lexer.o -lfl
	gcc $^ -o $@

count_words.o: count_words.c counter.h
	gcc $(CPPFLAGS) -c $< -o $@

counter.o: counter.c counter.h lexer.h
	gcc $(CPPFLAGS) -c $< -o $@

lexer.o: lexer.c lexer.h
	gcc $(CPPFLAGS) -c $< -o $@

lexer.c: lexer.l
	flex -t $< > $@
