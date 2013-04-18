#include "parse.h"

first_set *first_pack(grammar_symbol *symbol)
{
    first_set *ptr = first_set_create();
    first_symbol *sym = first_symbol_create();
    sym->symbol = symbol;
    ptr->first = sym;
    ptr->last = sym;
    ptr->len += 1;
    if (symbol->type == 3)
	ptr->have_null = 1;
    else
	ptr->have_null = 0;
    return ptr;
}

first_set *first_merge(first_set *from, first_set *to)
{
    first_symbol *i, *j, *ptr;

    /* set have_null */
    if (from->have_null)
	to->have_null = 1;

    /* merge */
    for (i = from->first; i != NULL; i = i->next) {
	/* find the same symbol */
	for (j = to->first; j != NULL; j = j->next)
	    if (j->symbol == i->symbol)
		break;
	/* if no same symbol, add the current first_symbol */
	if (j == NULL) {
	    ptr = first_symbol_create();
	    ptr->symbol = i->symbol;
	    if (to->len)
		to->last->next = ptr;
	    else
		to->first = ptr;
	    to->last = ptr;
	    to->len += 1;
	}
    }

    /* free the from set */
    first_set_free(from);

    return to;
}
