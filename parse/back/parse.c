#
#include "parse.h"

void parse(FILE *fp)
{
    fprintf(stderr, "loading parse file ...");
    input_file = fp;

    reader(fp);
}

void reader()
{
    fprintf(stderr, "reading parse file ...");
    create_symbol_table();
}

void create_symbol_table()
{
    int i;
    bucket *bp;

    /* malloc the symbol_table array */
    symbol_table = TMALLOC(bucket *, TABLE_SIZE);

    /* clean the symbol_table array */
    for (i = 0; i < TABLE_SIZE; ++i)
	symbol_table[i] = 0;

    /* initialize the symbol_table array */
    bp = make_bucket("");
    
}

bucket *make_bucket(const char *name)
{
    bucket *bp;

    /* check name */
    if (name == NULL)
	fprintf(stderr, "error: no name\n");

    bp = TMALLO(bucket, 1);
    bp->link = NULL;
    bp->next = NULL;

    bp->name = TMALLOC(char, strlen(name) + 1);
    bp->tag = NULL;
    bp->value = UNDEFINED;
    bp->index = 0;
    bp->prec = 0;
    bp->class = UNKNOWN;
    /* here */
}
