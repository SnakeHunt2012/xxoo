#include "../xxoo.h"

struct symbol {
    char *token;
    unsigned int tag;
    unsigned int type;
    char * pointer;
};

struct token {
    unsigned int tag;
    double value;
    char *pointer;
};

struct symbol_table {
    struct symbol *table[MAX];
    int length;
};

struct token_table {
    struct token *table[MAX];
    int length;
};

struct token_table tokens;
struct symbol_table symbols;

char ori_code[MAX];
char buffer_one[MAX];
char buffer_two[MAX];

int code_load(FILE *, char*);
int comment_remove(const char *, char *);
int blank_remove(const char *, char *);
int code_scan(const char *, struct token_table *, struct symbol_table *);

int number_scan(const char *, int);
int char_scan(const char *, int);
int string_scan(const char *, int);
int operator_scan(const char *, int);
int symbol_scan(const char *, int);
int identity_scan(const char *, int);

void number_load(const char *, int, int, struct token_table *, struct symbol_table *);
void char_load(const char *, int, int, struct token_table *, struct symbol_table *);
void string_load(const char *, int, int, struct token_table *, struct symbol_table *);
void operator_load(const char *, int, int, struct token_table *, struct symbol_table *);
void symbol_load(const char *, int, int, struct token_table *, struct symbol_table *);
void identity_load(const char *, int, int, struct token_table *, struct symbol_table *);
