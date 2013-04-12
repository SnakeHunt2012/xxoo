
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* messages */
typedef char Assoc_t;
typedef char Class_t;
typedef short Index_t;
typedef short Value_t;

/* the structure of a symbol table entry */
typedef struct bucket bucket;
struct bucket {
    struct bucket *link;
    struct bucket *next;
    char *name;
    char *tag;
    Value_t value;
    Index_t index;
    Value_t prec;
    Class_t class;
    Assoc_t assoc;
};


/* storage allocation macros */
#define TMALLOC(t,n)	((t*) malloc((size_t)(n) * sizeof(t)))

/*  the undefined value  */
#define UNDEFINED (-1)

/* symbol classes */
#define UNKNOWN 0
#define TERM 1
#define NONTERM 2


/* symbol_table */
#define TABLE_SIZE 1024
bucket **symbol_table = 0;
bucket *first_symbol;
bucket *last_symbol;
int symbol_table_length = 0;

FILE *input_file;

void parse(FILE *);
void reader(FILE *);

bucket *make_bucket(const char *);
