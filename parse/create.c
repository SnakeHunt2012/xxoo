#include "parse.h"

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

