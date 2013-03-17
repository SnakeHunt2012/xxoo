#include <stdio.h>
#include <stdlib.h>

#define MAX 1024 * 1024

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
    /* begin test
    struct table token_table;

    token_table.length = 1;
    token_table.tokens[0] = (struct token *)malloc(sizeof(struct token));
    token_table.tokens[0]->tag = 1;
    token_table.tokens[0]->value = 10;
    token_table.tokens[0]->str = "This is just a test\n";
    end test */

    void load_code(FILE *, char *);
    void clean_code(char *, char *);

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
	    /* begin debug
	    printf("test tag:%d\ntest value:%d\ntest str:%s",
		   token_table.tokens[0]->tag,
		   token_table.tokens[0]->value,
		   token_table.tokens[0]->str
		);
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
    int i, j, in_comment;

    i = 0;
    j = 0;
    in_comment = 0;

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
