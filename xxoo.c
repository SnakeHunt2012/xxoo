#include <stdio.h>
#include <stdlib.h>
#include "lex/lex.h"
#include "parse/parse.h"

main(int argc, char *argv[])
{
    FILE *fp;
    char *prog = argv[0];

    int ori_code_length;
    int clean_code_length;
    int tokens_length;

    if (argc == 1)
	fprintf(stderr, "%s: no file input", prog);
    else {
	/* lex */
	if ((fp = fopen(*++argv, "r")) == NULL) {
	    fprintf(stderr, "%s: can't open %s\n", prog, *argv);
	    exit(1);
	} else {
	    ori_code_length = code_load(fp, ori_code);
	    fclose(fp);
	    /*
	      fprintf(stdout, "code length: %d\ncode: %s\n the last word is: %c\n", 
	      code_length, ori_code, ori_code[code_length - 1]);
	    */
	    clean_code_length = comment_remove(ori_code, buffer_one);
	    /* 
	       fprintf(stdout, "code length: %d\ncode: %s\n the last word is: %c\n",
	       clean_code_length, buffer_one, buffer_one[clean_code_length - 2]);
	    */
	    clean_code_length = blank_remove(buffer_one, buffer_two);
	    /*
	      fprintf(stdout, "code       length: %d\ncode: %s\n the last word is: %c\n",
	      clean_code_length, buffer_two, buffer_two[clean_code_length - 2]);
	    */
	    code_scan(buffer_two, &tokens, &symbols);
	}
	/* parse */
	if ((fp = fopen(*++argv, "r")) == NULL) {
	    fprintf(stderr, "%s: can't open %s\n", prog, *argv);
	    exit(1);
	} else {
	    parse(fp);
	}
    }
    if (ferror(stdout)) {
	fprintf(stderr, "%s: error writing stdout\n", prog);
	exit(2);
    }
    exit(0);
}
