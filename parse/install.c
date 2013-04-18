#include "parse.h"

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

    while (code[j] == ' ' ||
	   code[j] == '\t' ||
	   code[j] == '\n' ||
	   code[j] == '\r')
	j++;
    return j;
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

void generator_symbol_install(right_generator *gen, grammar_symbol *sym)
{
    generator_symbol *ptr;

    /* check the sym pointer */
    if (sym == NULL)
	fprintf(stderr, "error generator_symbol_install: symbol invalid, it's NULL.\n");

    ptr = generator_symbol_create();
    ptr->symbol = sym;
    if (gen->len)
	gen->last->next = ptr;
    else
	gen->first = ptr;
    gen->last = ptr;
    gen->len += 1;
}

void right_generator_install(right_generator *ptr, generator *gen)
{
    if (gen->len) 
	gen->last->next = ptr;
    else
	gen->first = ptr;
    gen->last = ptr;
    gen->len += 1;
}
