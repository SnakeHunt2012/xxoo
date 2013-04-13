#include "parse.h"

void parse(FILE *fp)
{
    extern unsigned int parse_code_len;
    extern char parse_code[];

    parse_code_len = parse_read(fp, parse_code);
    /* debug
    printf("parse code:\n%s\n", parse_code);
    */
    grammars = parse_scan(parse_code);
}


unsigned int parse_read(FILE *fp, char *code)
{
    char c;

    unsigned int i;
    for (i = 0; (c = getc(fp)) != EOF; ++i)
	code[i] = c;
    code[i] = '\0';
    return i;
}

grammar_table *parse_scan(const char *code)
{
    register int i;
    register char c;
    extern grammar_table *grammars;
    extern grammar_symbol_table *grammar_symbols;

    grammars = grammar_table_create();
    /* debug
    fprintf(stderr, "grammars->len:\t%d\n", grammars->len);
    */
    grammar_symbols = grammar_symbol_table_create();
    /* debug
    fprintf(stderr, "grammar_symbols->len:\t%d\n", grammar_symbols->len);
    */
    
    /* point to the first character */
    i = 0;
    /* skip blank characters */
    while (code[i] == ' ' |
	   code[i] == '\t' |
	   code[i] == '\n' |
	   code[i] == '\r')
	++i;
    /* read grammar symbol define */
    if (code[i] == '%' && code[i + 1] == '{') {
	i += 2;
	i = grammar_define_read(code, i, grammars, grammar_symbols);
    }
    else {
	fprintf(stderr, "error parse_scan: just to read defination but not match percent + big quote \n");
	return NULL;
    }

    /* read grammar generator rules */
    /* 还没写好，不能进行测试
    while (code[i] == ' ' |
	   code[i] == '\t' |
	   code[i] == '\n' |
	   code[i] == '\r')
	i++;
    if (code[i] == '%' && code[i + 1] == '{') {
	i += 2;
	i = grammar_generator_read(code, i, grammars, grammar_symbols);
    }
    else {
	fprintf(stderr, "error parse_scan: just to read generators but not match percent + big quote \n");
	return NULL;
    }
    */
}

grammar_table *grammar_table_create()
{
    grammar_table *grammars = 
	(grammar_table *)malloc(sizeof(grammar_table));
    grammars->symbol = NULL;
    grammars->first = NULL;
    grammars->last = NULL;
    grammars->len = 0;
    return grammars;
}

grammar_symbol_table *grammar_symbol_table_create()
{
    grammar_symbol_table *grammar_symbols = 
	(grammar_symbol_table *)malloc(sizeof(grammar_symbol_table));
    grammar_symbols->first = NULL;
    grammar_symbols->last = NULL;
    grammar_symbols->len = 0;
    return grammar_symbols;
}

int grammar_define_read(const char *code, register int i, grammar_table *grammars, grammar_symbol_table *grammar_symbols)
{
    /* debug
    fprintf(stderr, "Reading grammar defination ...\nGrammars len: %d\n", grammars->len);
    fprintf(stderr, "current character define read:%c\n", code[i]);
    return i; */

    /* skip "%{" */
    i += 2;
    
    /* skip blank characters */
    while (code[i] == ' ' |
	   code[i] == '\t' |
	   code[i] == '\n' |
	   code[i] == '\r')
	i++;
    /* debug
    fprintf(stderr, "grammar_define_read: now character is %c\n", code[i]);
    fprintf(stderr, "grammar_define_read: next character is %c\n", code[i + 1]);
    return i; */

    while (!(code[i] == '%' && code[i + 1] == '}'))
	i = grammar_define_install(code, i, grammar_symbols);
}

int grammar_generator_read(const char *code, register int i, grammar_table *grammars, grammar_symbol_table *grammar_symbols)
{
    /* debug
    fprintf(stderr, "Reading grammar generator ...\nGrammars len: %d\n", grammars->len);
    fprintf(stderr, "current character generator read:%c\n", code[i]);
    return i; */

    /* skip "%{" */
    i += 2;

    /* skip blank characters */
    while (code[i] == ' ' |
	   code[i] == '\t' |
	   code[i] == '\n' |
	   code[i] == '\r')
	i++;
    /* debug
    fprintf(stderr, "grammar_generator_read: now character is %c\n", code[i]);
    fprintf(stderr, "grammar_generator_read: next character is %c\n", code[i + 1]);
    return i; */
    while (!(code[i] == '%' && code[i + 1] == '}'))
	i = grammar_generator_install(code, i, grammar_symbols, grammars);
}

int grammar_define_install(const char *code, register int i, grammar_symbol_table *grammar_symbols)
{
    register int j;

    j = i;

    /* debug
    fprintf(stderr, "grammar_define_install: current character is %c\n", code[i]); */

    j = symbol_recognise(code, i);

    /* debug
    fprintf(stderr, "grammar_define_install: i is %d, j is %d\n", i, j);
    fprintf(stderr, "grammar_define_install: the string copyed is: %s\n", code_strcpy(code, i, j - 1)); */

    /* debug 
    fprintf(stderr, "grammar_define_install: the last character of current symbol is %c\n", code[j - 1]); */

    /* time: 2013.04.13 20:19:09
     * 刚刚写到这里，现在可以获得符号定义部分的符号获取
     * 现在需要将获取的符号安装到数据结构中，具体包括：
     * 1.获得符号属性（1 - 终结符号；0 - 非终结符号）
     * 2.创建grammar_symbol结构体，及其初始化
     * 3.将符号和符号属性添加入新的grammar_symbol结构体中
     *   3.1.将语法符号字符串添加入语法符号结构体中；
     *   3.2.将语法符号属性值添加入语法符号结构体中；
     *   3.3.将语法符号结构体的next域设置为NULL；
     *   3.4.设置语法符号结构体的id值，该值为全局grammar_symbols中len的当前值。
     * 4.将新的grammar_symbol结构体添加到grammar_symbols中
     *   4.1.将grammar_symbols->last->next（即当前grammar_symbols中最后一个语法符号的next域）设置为当前结构体的指针值；
     *   4.2.将grammar_symbols->last也设置为当前结构体的指针值；
     *   4.3.增加grammar_symbol中语法符号的总数，即grammar_symbol->len++。
     */

    //fprintf(stderr, "grammar_define_install: current symbol is %s\n",
//	    gramma_symbols->last->value);
    /* debug */
    return j;
}

int grammar_generator_install(const char *code, register int i, grammar_symbol_table *grammar_symbols, grammar_table *grammars)
{
    /* debug */
    fprintf(stderr, "grammar_generator_install: current character is %c\n", code[i]);
    /* debug */
    return i;
}

int symbol_recognise(const char *code, register int i)
{
    int is_terminal;

    /* judge the type */
    if (code[i] == '"')
	is_terminal = 1;
    else if (code[i] == '<')
	is_terminal = 0;
    else {
	fprintf(stderr, "error symbol_recognise: current character is not the first character of any symbol\n");
	return i;
    }

    /* the next character */
    i++;

    /* recognise range */
    if (is_terminal)
	while (code[i] != '"')
	    i++;
    else
	while (code[i] != '>')
	    i++;
    return i + 1;
}

char *code_strcpy(const char *code, register int i, register int j)
{
    char *str;
    int len;

    /* debug
    fprintf(stderr, "code_strcpy: i is %d, j is %d\n", i, j);
    fprintf(stderr, "code_strcpy: code[i] is %c, code[j] is %c\n", code[i], code[j]); */

    len = j - i + 2;
    str = (char *)malloc(len * sizeof(char));
    for (j = 0; j < len; ++j)
	*(str + j) = code[i++];
    *(str + j) = '\0';
    return str;
}
