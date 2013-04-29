#include "parse.h"

/*--------
 * create
 ---------*/
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
    ptr->first = NULL;
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

right_generator *right_generator_create()
{
    right_generator *ptr = 
	(right_generator *) malloc(sizeof(*ptr));
    ptr->next = NULL;
    ptr->first = NULL;
    ptr->last = NULL;
    ptr->len = 0;
    return ptr;
}

generator_symbol *generator_symbol_create()
{
    generator_symbol *ptr;

    ptr = (generator_symbol *) malloc(sizeof(*ptr));
    ptr->next = NULL;
    ptr->symbol = NULL;
    return ptr;
}

item *item_create()
{
    item *ptr =
	(item *) malloc(sizeof(*ptr));
    ptr->next = NULL;
    ptr->left = NULL;
    ptr->right = NULL;
    ptr->situation = NULL;
    ptr->ahead = NULL;
    return ptr;
}

item_set *item_set_create()
{
    item_set *ptr = 
	(item_set *) malloc(sizeof(*ptr));
    ptr->next = NULL;
    ptr->first = NULL;
    ptr->last = NULL;
    ptr->len = 0;
    return ptr;
}

item_set_collection *item_set_collection_create()
{
    item_set_collection *ptr = 
	(item_set_collection *) malloc(sizeof(*ptr));
    ptr->first = NULL;
    ptr->last = NULL;
    ptr->len = 0;
    return ptr;
}

first_symbol *first_symbol_create()
{
    first_symbol *ptr = 
	(first_symbol *) malloc(sizeof(*ptr));
    ptr->next = NULL;
    ptr->symbol = NULL;
    return ptr;
}

first_set *first_set_create()
{
    first_set *ptr = 
	(first_set *) malloc(sizeof(*ptr));
    ptr->first = NULL;
    ptr->last = NULL;
    ptr->len = 0;
    ptr->have_null = 0;
    return ptr;
}

/*--------
 * free
 ---------*/
void first_set_free(first_set *ptr)
{
    if (ptr->first)
	first_free(ptr->first);
    free(ptr);
}

void first_free(first_symbol *ptr)
{
    if (ptr->next)
	first_free(ptr->next);
    free(ptr);
}
