#include "parse.h"

/*

// LR(1) First Set Generation Algorithm
begin
    // Initialize
    step 1.
    Add all of the nonterminals of the grammar to the nonterminals queue;

    loop:
    while queue is not empty,

    Step 2.
    Pop nonterminal X from the head of the queue;

    Step 3.
    // Compute a partial first(X) set for X
    For rules with X as a LHS symbol, 'X : epsilon' (i.e., rules for X with no RHS terminal symbols), add epsilon to first(X);

    Step 4.
    For all rules with X as LHS symbols, of the form 'X : t B ...',with terminal symbol t as the leftmost RHS symbol(where t is not epsilon), add symbol t to first(X);

    Step 5.
    For all rules with X as LHS symbol, of the form 'X : P B ...' with nonterminal symbol P as the leftmost RHS symbol, and where P is not X, add to first(X) all terminal symbols other than epsilon that are currently in first(P);
    (first(P) may still be incomplete at this point.)

    Step 6.
    For all rules with X as LHS symbol, of the form 'X : P B ...', if first(P) contains epsilon, repeat steps (3) through (6) with nonterminal B (which can be epsilon) in place of P.

    Step 7.
    If any terminals were added to first(X) in steps (3) through (6) that were not already members of first(X), or if first(X) is still empty, append nonterminal X to the tail of the queue (which causes it to undergo another iteration);
    otherwise first(X) is complete
    (and nonterminal X is no longer in the queue);

    end loop;
end.


 */

first_set *first_pack(grammar_symbol *symbol)
{
    first_set *ptr = first_set_create();
    first_symbol *sym = first_symbol_create();
    if (symbol->type == 1) {
	fprintf(stderr, "error first_pack: this symbol is not terminal symbol: %s, type is %d\n", symbol->value, symbol->type);
	free(ptr);
	free(sym);
	return NULL;
    }
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

    if (!from->len)
	return to;

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

void *first(grammar_symbol *symbol, grammar_table *grammars)
{
    first_set *set;
    first_set *ptr;
    generator *gen;
    right_generator *right;
    generator_symbol *gen_sym;

    /* if is epsilon */
    if (symbol->type == 3) {
	set = first_set_create();
	set->have_null = 1;
	symbol->first = set;
	return;
    }

    /* if is terminal */
    if (symbol->type == 0 || symbol->type == 2) {
	symbol->first = first_pack(symbol);
	return;
    }

    /* if is none-terminal */
    if (symbol->type == 1) {
	/* debug */
	fprintf(stdout, "first: current symbol is %s\n", symbol->value);
	/* debug */
	set = first_set_create();
	for (gen = grammars->first; gen != NULL; gen = gen->next)
	    if (gen->left == symbol)
		for (right = gen->first; right != NULL; right = right->next)
		    for (gen_sym = right->first; gen_sym != NULL; gen_sym = gen_sym->next) {
			ptr = first(gen_sym->symbol, grammars);
			if (ptr->have_null) {
			    set = first(ptr, set);
			    break;
			}
			set = first_merge(ptr, set);
		    }
	symbol->first = set;
    }
}


void first_drive(grammar_symbol_table *grammar_symbols, grammar_table *grammars)
{
    grammar_symbol *ptr;

    if (!grammar_symbols->first)
	fprintf(stderr, "error first_drive: no symbol in grammars, grammar->first is NULL\n");
    for (ptr = grammar_symbols->first; ptr != NULL; ptr = ptr->next)
	ptr->first = first(ptr, grammars);
}
