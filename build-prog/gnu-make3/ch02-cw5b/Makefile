VPATH = src

count_words: count_words.o counter.o lexer.o -lfl
	gcc $^ -o $@

count_words.o: count_words.c include/counter.h
	gcc -c $< -o $@

counter.o: counter.c include/counter.h include/lexer.h
	gcc -c $< -o $@

lexer.o: lexer.c include/lexer.h
	gcc -c $< -o $@

lexer.c: lexer.l
	flex -t $< > $@
