#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1024 * 1024

/* 
 *  key: 0
 *  dight: 1
 *  string: 2
 */
struct token {
    int tag;
    int value;
    char *str;
};

struct table {
    int length;
    struct token *tokens[MAX];
};

main(int argc, char *argv[])
{
    FILE *fp;
    char *prog = argv[0];
    char ori_code[MAX];
    char code[MAX];
    struct table token_table;

    void load_code(FILE *, char *);
    void clean_code(char *, char *);
    void scan_code(char *, struct table *token_table);

    /* begin debug */
    int i;
    /* end debug */

    token_table.length = 0;
    if (argc == 1)
	fprintf(stderr, "%s: no input file\n", prog);
    else
	if ((fp = fopen(*++argv, "r")) == NULL) {
	    fprintf(stderr, "%s: can't open %s\n", prog, *argv);
	    exit(1);
	} else {
	    /* load code */
	    load_code(fp, ori_code);
	    fclose(fp);
	    /* clean code */
	    clean_code(ori_code, code);
	    /* scan code */
	    scan_code(code, &token_table);

	    /* begin debug
	    for (i = 0; i < token_table.length; ++i)
		printf("token[%d]:\n\ttag: %d\n\tvalue: %d\n\tstr:%s\n",
		       i,
		       token_table.tokens[i]->tag,
		       token_table.tokens[i]->value,
		       token_table.tokens[i]->str);
	    end debug */

	}
    if (ferror(stdout)) {
	fprintf(stderr, "%s: error writing stdout\n", prog);
	exit(2);
    }
    exit(0);
}

void load_code(FILE *ifp, char *ori_code)
{
    int c, i;

    i = 0;
    while ((c = getc(ifp)) != EOF)
	ori_code[i++] = c;
    ori_code[i] = '\0';
}

void clean_code(char *ori_code, char *code)
{
    int i, j;

    i = 0;
    j = 0;

    while (ori_code[i] != '\0') {
	/* remove comment */
	if (ori_code[i] == '/' && ori_code[i + 1] == '*') {
	    while (!(ori_code[i] == '*' && ori_code[i + 1] == '/'))
		i++;
	    code[j++] = ' ';
	    i += 2;
	}
	code[j++] = ori_code[i++];
    }
    code[j] = '\0';
}

void scan_code(char *code, struct table *token_table)
{
    int i, line, v;

    i = 0;
    line = 0;
    while (code[i]) {
	/* ignore blank character */
	while (code[i]) {
	    if (code[i] == ' ' || code[i] == '\t') {
		i++;
		continue;
	    } else if (code[i] == '\n') {
		i++;
		line++;
	    }
	    else
		break;
	}
	/* scan number */
	if (isdigit(code[i])) {
	    v = 0;
	    do {
		v = 10 * v + (int)(code[i]) - 48;
		i++;
	    } while (isdigit(code[i]));
	    printf("outer\n");
	    token_table->tokens[token_table->length] = 
		(struct token *)malloc(sizeof(struct token));
	    token_table->tokens[token_table->length]->tag = 1;
	    token_table->tokens[token_table->length]->value = v;
	    token_table->tokens[token_table->length]->str = NULL;
	    token_table->length++;
	}
	/* scan string */
	else if (isalpha(code[i])) {
	    i++;
	} else i++;
    }
}
