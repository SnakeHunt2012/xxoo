#include "parse.h"

void first_debug(grammar_symbol_table *symbols)
{
    /* debug: one by one merge
    {
	grammar_symbol *ptr;
	first_set *set = first_set_create();
	for (ptr = grammar_symbols->first; ptr != NULL; ptr = ptr->next)
	    set = first_merge(first_pack(ptr), set);
	first_set_debug(set);
    }
    */

    /* debug: merge with same symbol */
    {
	grammar_symbol *ptr;
	first_set *set1 = first_set_create();
	first_set *set2 = first_set_create();
	for (ptr = grammar_symbols->first; ptr != NULL; ptr = ptr->next)
	    set1 = first_merge(first_pack(ptr), set1);
	for (ptr = grammar_symbols->first; ptr != NULL; ptr = ptr->next)
	    set2 = first_merge(first_pack(ptr), set2);
	set1 = first_merge(set2, set1);
	set2 = NULL;
	first_set_debug(set1);
	first_set_debug(set2);
    }
    /* debug: merge with same symbol */
    
}

void first_set_debug(first_set *set)
{
    first_symbol *ptr;
    if (set)
	for (ptr = set->first; ptr != NULL; ptr = ptr->next)
	    fprintf(stderr, 
		    "first_set_debug: first_symbol->symbol->value is %s\n", 
		    ptr->symbol->value);
    else
	fprintf(stderr, "first_set_debug: set is null\n");
}
