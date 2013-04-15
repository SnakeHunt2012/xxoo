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
    /* skip blank characters before grammar define */
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

    /* skip blank characters before grammar rules */
    while (code[i] == ' ' |
	   code[i] == '\t' |
	   code[i] == '\n' |
	   code[i] == '\r')
	++i;
    /* debug
    fprintf(stderr, "parse_scan: current character %c, next is %c\n", code[i], code[i + 1]); */

    /* begin with %{ */
    if (code[i] == '%' && code[i + 1] == '{') {
	i += 2;
	i = skip_blank(code, i);
    }
    else
	fprintf(stderr, "error parse_scan: not the start of rules section\n");
    /* debug
    fprintf(stderr, "parse_scan: current character is %c\n", code[i], code[i + 1]); */

    /* read grammar generator rules */
    while (code[i] == '<')
	i = grammar_generator_read(code, i, grammars, grammar_symbols);

    /* debug: display all rules in grammars */
    /* debug: display all rules in grammars */

    /* end with %} */
    if (code[i] == '%' && code[i + 1] == '}') {
	i += 2;
	i = skip_blank(code, i);
    }
    else
	fprintf(stderr, "error parse_scan: not the end of rules section\n");
    /* debug begin: print all rules
    fprintf(stderr, "parse_scan: number of rules: %d\n", grammars->len);
    for (ptr = grammars->first; ptr != NULL; ptr = ptr->next)
        fprintf(stderr, "parse_scan: ptr->left: %s\n", ptr->left->value); */
}

int grammar_define_read(const char *code, register int i, grammar_table *grammars, grammar_symbol_table *grammar_symbols)
{
    grammar_symbol *ptr;
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

    /* debug
    for (ptr = grammar_symbols->first; ptr != NULL; ptr = ptr->next)
         fprintf(stderr, "grammar_define_read: current symbol is %s, type is %d\n", ptr->value, ptr->type); */
    return i + 2;
}

int grammar_generator_read(const char *code, register int i, grammar_table *grammars, grammar_symbol_table *grammar_symbols)
{
    generator *ptr;

    /***********************************/
    /* read and install left generator */
    /***********************************/

    /* check first character */
    if (code[i] != '<')
	if (code[i] == '"')
	    fprintf(stderr, "grammar_generator_read: the left generator is not a un-terminal grammar symbol\n");
	else
	    fprintf(stderr, "grammar_generator_read: the left generator is even not a grammar symbol\n");

    /* create grammar generator */
    ptr = grammar_generator_create();

    /* install left generator */
    left_generator_install(ptr, symbol_find(code, i, symbol_recognise(code, i) - 1, grammar_symbols));

    /* skip this generstor symbol */
    i = symbol_recognise(code, i);

    /* skip blank */
    i = skip_blank(code, i);

    /***********************************/
    /* read and install right generator*/
    /***********************************/

    /* install right generators */
    /* debug: skip all right generator */
    /* 正常应该是read_right_generator()的循环 */
    while (code[i] != ';')
	i++;
    /* debug: skip all right generator */

    /* check the last character of a generator and skip it */
    if (code[i] == ';')
	i++;
    else
	fprintf(stderr, "error grammar_generator_read: invalid end of a generator, the last character is %c", code[i]);

    /* install the generator to grammars */
    generator_install(ptr, grammars); /* undefined */

    /* skip blank */
    i = skip_blank(code, i);
    return i;
}

int grammar_define_install(const char *code, register int i, grammar_symbol_table *grammar_symbols)
{
    register int j;
    grammar_symbol *ptr;

    j = i;

    /* debug
    fprintf(stderr, "grammar_define_install: current character is %c\n", code[i]); */

    /* debug
    j = symbol_recognise(code, i);
    grammar_symbol_install(code_strcpy(code, i, j - 1), 
			   (unsigned int) (code[j + 1] - '0'), 
			   grammar_symbols); */
    j = symbol_recognise(code, i);
    grammar_symbol_install(code_strcpy(code, i, j - 1), 
			   (unsigned int) (code[j + 1] - '0'), 
			   grammar_symbols);
    /* debug
    fprintf(stderr, "grammar_define_install: current symbol is %s, type is %d\n",
            grammar_symbols->last->value, grammar_symbols->last->type); */

    /* skip the type code */
    j += 2;

    while (code[j] == ' ' |
	   code[j] == '\t' |
	   code[j] == '\n' |
	   code[j] == '\r')
	j++;
    return j;
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

    len = j - i + 1;
    str = (char *) malloc(len * sizeof(char));
    for (j = 0; j < len; ++j)
	*(str + j) = code[i++];
    *(str + j) = '\0';
    return str;
}

grammar_table *grammar_table_create()
{
    grammar_table *grammars = 
	(grammar_table *) malloc(sizeof(grammar_table));
    grammars->symbol = NULL;
    grammars->first = NULL;
    grammars->last = NULL;
    grammars->len = 0;
    return grammars;
}

grammar_symbol_table *grammar_symbol_table_create()
{
    grammar_symbol_table *grammar_symbols = 
	(grammar_symbol_table *) malloc(sizeof(grammar_symbol_table));
    grammar_symbols->first = NULL;
    grammar_symbols->last = NULL;
    grammar_symbols->len = 0;
    return grammar_symbols;
}

grammar_symbol *grammar_symbol_create()
{
    grammar_symbol *ptr = 
	(grammar_symbol *) malloc(sizeof(*ptr));
    ptr->next = NULL;
    ptr->id = 0;
    ptr->type = 0;
    ptr->value = NULL;
    return ptr;
}

generator *grammar_generator_create()
{
    generator *ptr = 
	(generator *) malloc(sizeof(*ptr));
    ptr->next = NULL;
    ptr->left = NULL;
    ptr->first = NULL;
    ptr->last = NULL;
    return ptr;
}

grammar_symbol *grammar_symbol_install(char *str, unsigned int type, grammar_symbol_table *list)
{
    grammar_symbol *ptr;

    ptr = grammar_symbol_create();
    ptr->next = NULL;
    ptr->id = list->len;
    ptr->type = type;
    ptr->value = str;
    if (!list->len)
	/* set the first */
	list->first = ptr;
    else
	/* set the tail */
	list->last->next = ptr;
    /* set the last */
    list->last = ptr;
    /* set len */
    list->len += 1;
    return ptr;
}

int skip_blank(const char *code, register int i)
{
    /* skip blank characters */
    while (code[i] == ' ' |
	   code[i] == '\t' |
	   code[i] == '\n' |
	   code[i] == '\r')
	i++;
    return i;
}

int symbol_match(const char *code, register int i, register int j, const char *str)
{
    register int k;

    k = 0;
    while (i <= j)
	if (str[k++] != code[i++])
	    return 0;
    if (str[k] != '\0')
	return 0;
    return 1;
}

grammar_symbol *symbol_find(const char *code, register int i, register int j, grammar_symbol_table *grammar_symbols)
{
    register grammar_symbol *ptr;

    for (ptr = grammar_symbols->first; ptr != NULL; ptr = ptr->next)
	if (symbol_match(code, i, j, ptr->value))
	    break;
    return ptr;
}

void left_generator_install(generator *gen, grammar_symbol *sym)
{
    if (sym == NULL)
	fprintf(stderr, "error left_generator_install: symbol invalid, it's NULL.\n");
    else
	gen->left = sym;
}

void generator_install(generator *ptr, grammar_table *grammars)
{
    if (grammars->len)
	grammars->last->next = ptr;
    else
	grammars->first = ptr;
    grammars->last = ptr;
    grammars->len++;
}
