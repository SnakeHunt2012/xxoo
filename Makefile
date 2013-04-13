SHELL = /bin/sh

all : xxoo

xxoo : xxoo.c lex/lex.c lex/lex.h parse/parse.c parse/parse.h
	cc -o xxoo xxoo.c lex/lex.c parse/parse.c

check : xxoo test/test.c test/grammar.ooxx
	./xxoo test/test.c test/grammar.ooxx

clean :
	rm xxoo
