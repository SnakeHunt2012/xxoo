#include "../xxoo.h"

/* grammar_table::语法表（结构体）
 * - struct grammar_symbol *symbol::初始符号：语法符号结构体指针
 * - struct generator *first::第一个产生式：产生式结构体指针
 * - struct generator *last::最后一个产生式：产生式结构体指针
 * - unsigned int len::产生式总数：整数 */
typedef struct grammar_table grammar_table;
struct grammar_table {
    struct grammar_symbol *symbol;
    struct generator *first;
    struct generator *last;
    unsigned int len;
};

/* generator::产生式（结构体）
 * - struct generator *next::下一产生式：产生式结构体指针
 * - struct grammar_symbol *left::左产生式：语法符号结构体指针
 * - struct generator_symbol *first::第一个右产生式：产生式符号结构体指针
 * - struct generator_symbol *last::最后一个右产生式：产生式符号结构体指针
 * - unsigned int len::右产生式总数：整数 */
typedef struct generator generator;
struct generator {
    struct generator *next;
    struct grammar_symbol *left;
    struct right_generator *first;
    struct right_generator *last;
    unsigned int len;
};

/* right_generator::右产生式（结构体）
 * - struct right_generator *next::下一右产生式：右产生式结构体指针
 * - struct generator_symbol *first::第一个产生式符号：产生式符号结构体指针
 * - struct generator_symbol *last::最后一个产生式符号：产生式符号结构体指针
 * - unsigned int len::产生式符号总数：整数 */
typedef struct right_generator right_generator; 
struct right_generator {
    struct right_generator *next;
    struct generator_symbol *first;
    struct generator_symbol *last;
    unsigned int len;
};


/* generator_symbol::产生式符号（结构体）
 * - struct generator_symbol *next::下一产生式符号：产生式符号结构体指针
 * - struct grammar_symbol *symbol::语法符号：语法符号结构体指针 */
typedef struct generator_symbol generator_symbol;
struct generator_symbol {
    struct generator_symbol *next;
    struct grammar_symbol *symbol;
};

/* grammar_symbol_table::语法符号表（结构体）
 * - struct grammar_symbol *first::第一个语法符号：语法符号结构体指针
 * - struct grammar_symbol *last::最后一个语法符号：语法符号结构体指针
 * - unsigned int len::语法符号总数：整数 */
typedef struct grammar_symbol_table grammar_symbol_table;
struct grammar_symbol_table {
    struct grammar_symbol *first;
    struct grammar_symbol *last;
    unsigned int len;
};

/* grammar_symbol::语法符号（结构体）
 * - struct grammar_symbol *next::下一语法符号：语法符号结构体指针
 * - unsigned int id::符号id：整数（按照语法符号表顺序分配）
 * - unsigned int type::语法符号类型：整数（宏定义，终结附/非终结符）
 * - char *value::语法符号内容：字符串指针 */
typedef struct grammar_symbol grammar_symbol;
struct grammar_symbol {
    struct grammar_symbol *next;
    unsigned int id;
    unsigned int type;
    char *value;
};

char parse_code[MAX];
unsigned int parse_code_len;
grammar_table *grammars;
grammar_symbol_table *grammar_symbols;

void parse(FILE *);
unsigned int parse_read(FILE *, char *);
grammar_table *parse_scan(const char *);
int skip_blank(const char *, register int);

int grammar_define_read(const char *, register int, grammar_table *, grammar_symbol_table *);
int grammar_generator_read(const char *, register int, grammar_table *, grammar_symbol_table *);
int right_generator_read(const char *, register int, generator *, grammar_symbol_table *);
int generator_symbol_read(const char *, register int, right_generator *ptr, grammar_symbol_table *);

int grammar_define_install(const char *, register int, grammar_symbol_table *);
void generator_install(generator *, grammar_table *);
void left_generator_install(generator *, grammar_symbol *);
void right_generator_install(right_generator *, generator *);
grammar_symbol *grammar_symbol_install(char *, unsigned int, grammar_symbol_table *);
void generator_symbol_install(right_generator *, grammar_symbol *);

int symbol_recognise(const char *, register int);
char *code_strcpy(const char *, register int, register int);
grammar_symbol *symbol_find(const char *, register int, register int, grammar_symbol_table *);
int symbol_match(const char *, register int, register int, const char *);

grammar_table *grammar_table_create();
grammar_symbol_table *grammar_symbol_table_create();
grammar_symbol *grammar_symbol_create();
generator *grammar_generator_create();
right_generator *right_generator_create();
generator_symbol *generator_symbol_create();


void debug_grammar(void); /* undefined */
void debug_generator(void); /* undefined */
void debug_left_generator(void); /* undefined */
void debug_right_generators(void); /* undefined */
void debug_generator_symbols(void); /* undefined */

