#define MAX 1024 * 1024

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
