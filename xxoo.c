#include <stdio.h>
#include <stdlib.h>

#define MAX 1024 * 1024

main(int argc, char *argv[])
{
    FILE *fp;
    char *prog = argv[0];
    char ori_code[MAX];
    char code[MAX];

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
	    /* debug */
	    printf("%s", code);
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
