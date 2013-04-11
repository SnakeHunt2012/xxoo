SHELL = /bin/sh

all : xxoo

xxoo : xxoo.c lex/lex.c parse/parse.c
	cc -o xxoo xxoo.c lex/lex.c parse/parse.c

test : xxoo test/test.c
	./xxoo test/test.c

clean :
	rm xxoo
