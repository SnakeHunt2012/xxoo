SHELL = /bin/sh

all : xxoo

xxoo : xxoo.c lex/lex.c lex/lex.h \
	parse/parse.c parse/create.c \
	parse/install.c parse/first.c \
	parse/debug.c parse/parse.h
	cc -o xxoo xxoo.c \
	lex/lex.c parse/parse.c \
	parse/create.c parse/install.c \
	parse/first.c parse/debug.c

check : xxoo test/test.xxoo test/grammar.ooxx 
	./xxoo test/test.xxoo test/grammar.ooxx

clean :
	rm xxoo
