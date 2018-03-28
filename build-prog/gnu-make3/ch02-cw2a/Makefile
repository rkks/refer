count_words: count_words.o counter.o lexer.o -lfl
	gcc $^ -o $@

count_words.o: count_words.c
	gcc -c $<

counter.o: counter.c
	gcc -c $<

lexer.o: lexer.c
	gcc -c $<

lexer.c: lexer.l
	flex -t $< > $@
