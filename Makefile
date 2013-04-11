SHELL = /bin/sh

xxoo : xxoo.c lex/lex.c parse/parse.c
	cc -o xxoo xxoo.c lex/lex.c parse/parse.c

clean :
	rm xxoo
