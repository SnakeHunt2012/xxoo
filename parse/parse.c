#include "parse.h"

void parse(FILE *fp)
{
    extern unsigned int parse_code_len;
    extern char parse_code[];

    parse_code_len = parse_read(fp, parse_code);
    /* debug: whether can read grammar file into code[]
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

    /* debug print all defines and rules */
    grammar_symbol *ptr1;
    generator *ptr2;
    right_generator *ptr3;
    generator_symbol *ptr4;
    /* debug print all defines and rules */

    grammars = grammar_table_create();

    /* debug: whether can create grammar_table
    fprintf(stderr, "grammars->len:\t%d\n", grammars->len);
    */

    grammar_symbols = grammar_symbol_table_create();

    /* debug: whether can create grammar_symbol_table
    fprintf(stderr, "grammar_symbols->len:\t%d\n", grammar_symbols->len);
    */
    
    /* point to the first character */
    i = 0;

    /* skip blank characters before grammar define */
    i = skip_blank(code, i);

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
    i = skip_blank(code, i);

    /* debug: whether the begin of grammar define section
    fprintf(stderr, "parse_scan: current character %c, next is %c\n", code[i], code[i + 1]); */

    /* debug begin: print all defines */
    fprintf(stderr, "parse_scan: gramamr_symbols->len is %d\n", grammar_symbols->len);
    for (ptr1 = grammar_symbols->first; ptr1 != NULL; ptr1 = ptr1->next)
	fprintf(stderr, "parse_scan: ptr1->value is %s, type is %d\n", ptr1->value, ptr1->type);
    /* debug begin: print all defines */    

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

    /* end with %} */
    if (code[i] == '%' && code[i + 1] == '}') {
	i += 2;
	i = skip_blank(code, i);
    }
    else
	fprintf(stderr, "error parse_scan: not the end of rules section\n");

    /* debug begin: print all rules */
    fprintf(stderr, "parse_scan: grammars-len is %d\n", grammars->len);
    fprintf(stderr, "******************************************\n");
    for (ptr2 = grammars->first; ptr2 != NULL; ptr2 = ptr2->next) {
        fprintf(stderr, "parse_scan: ptr2->left: %s\n", ptr2->left->value);
	for (ptr3 = ptr2->first; ptr3 != NULL; ptr3 = ptr3->next) {
	    for (ptr4 = ptr3->first; ptr4 != NULL; ptr4 = ptr4->next)
		fprintf(stderr, "%s ", ptr4->symbol->value);
	    fprintf(stderr, "\n");
	}
	fprintf(stderr, "******************************************\n");
    }
    /* debug begin: print all rules */

    /* debug */
    first_drive(grammar_symbols, grammars);
    /* debug */

    return grammars;
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
    while (code[i] == ' ' ||
	   code[i] == '\t' ||
	   code[i] == '\n' ||
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

    /* skip : */
    if (code[i] != ':')
	fprintf(stderr, "error grammar_generator_read: not the start of right generator :, the code is %c\n", code[i]);

    /***********************************/
    /* read and install right generator*/
    /***********************************/

    /* install right generators */
    while (code[i] != ';')
	i = right_generator_read(code, i, ptr, grammar_symbols);

    /* check the last character of a generator and skip it */
    if (code[i] == ';')
	i++;
    else
	fprintf(stderr, "error grammar_generator_read: invalid end of a generator, the last character is %c", code[i]);

    /* install the generator to grammars */
    generator_install(ptr, grammars);

    /* skip blank */
    i = skip_blank(code, i);
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

    len = j - i + 1;
    str = (char *) malloc(len * sizeof(char));
    for (j = 0; j < len; ++j)
	*(str + j) = code[i++];
    *(str + j) = '\0';
    return str;
}

int skip_blank(const char *code, register int i)
{
    /* skip blank characters */
    while (code[i] == ' ' ||
	   code[i] == '\t' ||
	   code[i] == '\n' ||
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

int right_generator_read(const char *code, register int i, generator *gen, grammar_symbol_table *grammar_symbols)
{
    right_generator *ptr;
    /* check whether the begin */
    if (!(code[i] == ':' ||
	 code[i] == '|'))
	fprintf(stderr, "error right_generator_read: invalid begin of a right generator: %d %d %d %d\n", code[i - 1], code[i], code[i + 1], i);
    else
	i++;

    /* create right generator */
    ptr = right_generator_create();

    /* skip blank */
    i = skip_blank(code, i);

    /* check begin of symbol */
    if (!(code[i] == '<' ||
	  code[i] == '"'))
	fprintf(stderr, "error right_generator_read: invalid begin of a generator symbol: %c\n", code[i]);

    /* read generator symbol */
    while (code[i] == '<' ||
	   code[i] == '"')
	i = generator_symbol_read(code, i, ptr, grammar_symbols);

    /* install right_generator */
    right_generator_install(ptr, gen);

    /* check end of right generator */
    if (!(code[i] == '|' ||
	 code[i] == ';'))
	fprintf(stderr, "error rigth_generator_read: invalid end of a right generator: %c\n", code[i]);

    return i;
}

int generator_symbol_read(const char *code, register int i, right_generator *ptr, grammar_symbol_table *symbols)
{
    /* check the first symbol */
    if (!(code[i] == '<' ||
	  code[i] == '"'))
	fprintf(stderr, "error generator_symbol_read: not begin of a generator symbol: %c\n", code[i]);

    /* install this symbol */
    generator_symbol_install(ptr, 
			     symbol_find(code,
					 i,
					 symbol_recognise(code, i) - 1,
					 symbols));


    /* skip current symbol */
    i = symbol_recognise(code, i);

    /* skip blank */
    i = skip_blank(code, i);

    return i;
}

