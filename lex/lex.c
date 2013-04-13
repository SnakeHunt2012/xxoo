#include "lex.h"

int code_load(FILE *fp, char *code)
{
    char c;
    int i;

    for (i = 0;(c = getc(fp)) != EOF; ++i)
	code[i] = c;
    code[i] = '\0';
    return i;
}

int comment_remove(const char *ori, char *code)
{
    int i, j, state;

    i = 0;
    j = 0;
    state = 0;
    while (ori[i] != '\0')
	switch (state) {
	case 0:
	    if (ori[i] == '/') {
		state = 2;
		i++;
	    } else
		state = 1;
	    break;
	case 1: 
	    code[j++] = ori[i++];
	    if (ori[i] == '/')
		state = 2;
	    break;
	case 2:
	    if (ori[++i] == '*')
		state = 3;
	    else
		state = 1;
	    break;
	case 3:
	    if (ori[++i] == '*')
		state = 4;
	    break;
	case 4:
	    if (ori[++i] == '/')
		state = 5;
	    else
		state = 3;
	    break;
	case 5:
	    i++;
	    code[j++] = ' ';
	    state = 1;
	    break;
	}
    code[j] = '\0';
    return j;
}

int blank_remove(const char *ori, char *code)
{
    int i, j, state;

    i = 0;
    j = 0;
    state = 0;
    while (ori[i] != '\0')
	switch (state) {
	case 0:
	    if (ori[i] == ' ' ||
		ori[i] == '\t' ||
		ori[i] == '\n' ||
		ori[i] == '\r')
		state = 2;
	    else if (ori[i] == '"')
		state = 3;
	    else
		state = 1;
	    break;
	case 1:
	    code[j++] = ori[i++];
	    if (ori[i] == ' ' ||
		ori[i] == '\t' ||
		ori[i] == '\n' ||
		ori[i] == '\r')
		state = 2;
	    else if (ori[i] == '"')
		state = 3;
	    else
		state = 1;
	    break;
	case 2:
	    i++;
	    if (!(ori[i] == ' ' ||
		  ori[i] == '\t' ||
		  ori[i] == '\n' ||
		  ori[i] == '\r'))
		if (ori[i] == '"') {
		    code[j++] = ' ';
		    state = 3;
		}
		else {
		    code[j++] = ' ';
		    state = 1;
		}
	    break;
	case 3:
	    code[j++] = ori[i++];
	    if (ori[i] == '"')
		state = 1;
	}
    code[j] = '\0';
    return j;
}

int code_scan(const char *code, struct token_table *tokens, struct symbol_table *symbols)
{
    int i, begin, vote[6], len, index;

    begin = 0;
    tokens->length = 0;
    do {
	len = 0;
	index = 0;
	vote[0] = number_scan(code, begin);
	vote[1] = char_scan(code, begin);
	vote[2] = string_scan(code, begin);
	vote[3] = operator_scan(code, begin);
	vote[4] = symbol_scan(code, begin);
	vote[5] = identity_scan(code, begin);
	for (i = 0; i < 6; ++i)
	    if (vote[i]) {
		index = i;
		len = vote[i];
	    }
	/*
	fprintf(stderr, "this word is: %d\tlen: %d\tcode[%d]:%c\n\n", index, len, begin, code[begin]);
	*/
	switch (index) {
	case 0:
	    /* is number */
	    number_load(code, begin, len, tokens, symbols);
	    break;
	case 1:
	    /* is char */
	    char_load(code, begin, len, tokens, symbols);
	    break;
	case 2:
	    /* is string */
	    string_load(code, begin, len, tokens, symbols);
	    break;
	case 3:
	    /* is operator */
	    operator_load(code, begin, len, tokens, symbols);
	    break;
	case 4:
	    /* is symbol */
	    symbol_load(code, begin, len, tokens, symbols);
	    break;
	case 5:
	    /* is identity */
	    identity_load(code, begin, len, tokens, symbols);
	    break;
	}
	begin += len;
    } while (code[begin] != '\0');
    /*
	vote[0] = number_scan(code, 0);
	vote[1] = char_scan(code, 0);
	vote[2] = string_scan(code, 0);
	vote[3] = operator_scan(code, 0);
	vote[4] = symbol_scan(code, 0);
	vote[5] = identity_scan(code, 0);
	fprintf(stderr, "number:\t%d\nchar:\t%d\nstring:\t%d\noperator:\t%d\nsymbol:\t%d\nidentity:\t%d\n",
	   vote[0],
	   vote[1],
	   vote[2],
	   vote[3],
	   vote[4],
	   vote[5]);
    */
    /*
    fprintf(stderr, "tokens->length: %d\n", tokens->length);
    for (i = 0; i < tokens->length; ++i)
	if (tokens->table[i]->tag == 1)
	    fprintf(stderr, "code_scan: number - tokens->table[%d] = %f\n", i, tokens->table[i]->value);
	else if (tokens->table[i]->tag == 2)
	    fprintf(stderr, "code_scan: char - tokens->table[%d] = %c\n", i, *(tokens->table[i]->pointer));
	else if (tokens->table[i]->tag == 3)
	    fprintf(stderr, "code_scan: string - tokens->table[%d] = %s\n", i, tokens->table[i]->pointer);
	else if (tokens->table[i]->tag == 4)
	    fprintf(stderr, "code_scan: operator - tokens->table[%d] = %s\n", i, tokens->table[i]->pointer);
	else if (tokens->table[i]->tag == 5)
	    fprintf(stderr, "code_scan: symbol - tokens->table[%d] = %s\n", i, tokens->table[i]->pointer);
	else if (tokens->table[i]->tag == 6)
	    fprintf(stderr, "code_scan: identity - tokens->table[%d] = %s\n", i, tokens->table[i]->pointer);
    */
    return tokens->length;
}

int number_scan(const char *code, int begin)
{
    int front, state;

    front = begin;
    if (code[front] == '+' || code[front] == '-')
	/* is +/- */
	state = 1;
    else if (code[front] == '0' ||
	     code[front] == '1' ||
	     code[front] == '2' ||
	     code[front] == '3' ||
	     code[front] == '4' ||
	     code[front] == '5' ||
	     code[front] == '6' ||
	     code[front] == '7' ||
	     code[front] == '8' ||
	     code[front] == '9')
	/* is digit */
	state = 2;
     else
	/* pattern dosen't match */
	state = 0;
    do {
	switch (state) {
	case 0:
	    return 0;
	    break;
	case 1:
	    front++;
	    if (code[front] == '0' ||
		code[front] == '1' ||
		code[front] == '2' ||
		code[front] == '3' ||
		code[front] == '4' ||
		code[front] == '5' ||
		code[front] == '6' ||
		code[front] == '7' ||
		code[front] == '8' ||
		code[front] == '9')
		state = 2;
	    else if (code[front] == ' ' ||
		     code[front] == '\n' ||
		     code[front] == '\t' ||
		     code[front] == '\r')
		state = 1;
	    else
		state = 0;
	    break;
	case 2:
	    front++;
	    if (code[front] == '0' ||
		code[front] == '1' ||
		code[front] == '2' ||
		code[front] == '3' ||
		code[front] == '4' ||
		code[front] == '5' ||
		code[front] == '6' ||
		code[front] == '7' ||
		code[front] == '8' ||
		code[front] == '9')
		state = 2;
	    else if (code[front] == '.')
		state = 3;
	    else if (code[front] == ' ' ||
		     code[front] == '\n' ||
		     code[front] == '\t' ||
		     code[front] == '\r')
		state = 2;
	    else
		state = 0;
	    break;
	case 3:
	    front++;
	    if (code[front] == '0' ||
		code[front] == '1' ||
		code[front] == '2' ||
		code[front] == '3' ||
		code[front] == '4' ||
		code[front] == '5' ||
		code[front] == '6' ||
		code[front] == '7' ||
		code[front] == '8' ||
		code[front] == '9')
		state = 4;
	    else if (code[front] == ' ' ||
		     code[front] == '\n' ||
		     code[front] == '\t' ||
		     code[front] == '\r')
		state = 3;
	    else
		state = 0;
	    break;
	case 4:
	    front++;
	    if (code[front] == '0' ||
		code[front] == '1' ||
		code[front] == '2' ||
		code[front] == '3' ||
		code[front] == '4' ||
		code[front] == '5' ||
		code[front] == '6' ||
		code[front] == '7' ||
		code[front] == '8' ||
		code[front] == '9')
		state = 4;
	    else if (code[front] == ' ' ||
		     code[front] == '\n' ||
		     code[front] == '\t' ||
		     code[front] == '\r')
		state = 4;
	    else
		state = 0;
	    break; 
	}
    } while (code[front] != ' '  &&
	     code[front] != '\n' &&
	     code[front] != '\t' &&
	     code[front] != '\r');
    if (state == 2 || state == 4)
	return front - begin + 1;
    else
	return 0;
}

int char_scan(const char *code, int begin)
{
    int front, state;

    front = begin;
    if (code[front] == '\'')
	state = 1;
    else
	state = 0;
    do {
	switch (state) {
	case 0:
	    return 0;
	    break;
	case 1:
	    front++; 
	    if (code[front] == '\'')
		state = 3;
	    else if ((code[front] <= 126 &&
		      code[front] >= 33) ||
		     code[front] == ' ')
		state = 2;
	    else if (code[front] == ' ' ||
		     code[front] == '\t' ||
		     code[front] == '\n' ||
		     code[front] == '\r')
		state = 1;
	    else
		state = 0;
	    break;
	case 2:
	    front++;
	    if (code[front] == '\'')
		state = 3;
	    else if (code[front] == ' ' ||
		     code[front] == '\t' ||
		     code[front] == '\n' ||
		     code[front] == '\r')
		state = 2;
	    else
		state = 0;
	    break;
	case 3:
	    front++;
	    if (code[front] == ' ' ||
		code[front] == '\t' ||
		code[front] == '\n' ||
		code[front] == '\r')
		state = 3;
	    else
		state = 0;
	}
    } while (code[front] != ' ' &&
	     code[front] != '\t' &&
	     code[front] != '\n' &&
	     code[front] != '\r');
    if (state == 3)
	return front - begin + 1; 
    else
	return 0;
}

int string_scan(const char *code, int begin)
{
    int front, state, inside;

    front = begin;
    inside = 0;
    if (code[front] == '"')
	state = 1;
    else
	state = 0;
    do {
	switch (state) {
	case 0:
	    return 0;
	    break;
	case 1:
	    front++; 
	    inside = 1;
	    if (code[front] == '"')
		state = 2;
	    else
		state = 1;
	    break;
	case 2:
	    front++;
	    inside = 0;
	    if (code[front] == ' ' ||
		code[front] == '\t' ||
		code[front] == '\n' ||
		code[front] == '\r')
		state = 2;
	    else
		state = 0;
	    break;
	}
    } while ((code[front] != ' ' &&
	     code[front] != '\t' &&
	     code[front] != '\n' &&
	      code[front] != '\r') ||
	     inside == 1);
    if (state == 2)
	return front - begin + 1; 
    else
	return 0;
}

int operator_scan(const char *code, int begin)
{
    int front, state;

    front = begin;
    switch (code[front]) {
    case '~':
	state = 1;
	break;
    case '*':
	state = 2;
	break;
    case '/':
	state = 4;
	break;
    case '%':
	state = 6;
	break;
    case '=':
	state = 8;
	break;
    case '!':
	state = 10;
	break;
    case '^':
	state = 12;
	break;
    case '+':
	state = 14;
	break;
    case '-':
	state = 17;
	break;
    case '&':
	state = 20;
	break;
    case '|':
	state = 23;
	break;
    case '<':
	state = 26;
	break;
    case '>':
	state = 30;
	break;
    default:
	state = 0;
	break;
    }
    do {
	switch (state) {
	case 0:
	    return 0;
	    break;
	case 1:
	    front++;
	    if (code[front] == ' ' ||
		code[front] == '\t' ||
		code[front] == '\n' ||
		code[front] == '\r')
		state = 1; 
	    else state = 0;
	    break;
	case 2:
	    front++;
	    if (code[front] == '=')
		state = 3;
	    else if (code[front] == ' ' ||
		     code[front] == '\t' ||
		     code[front] == '\n' ||
		     code[front] == '\r')
		state = 2;
	    else
		state = 0;
	    break;
	case 3:
	    front++;
	    if (code[front] == ' ' ||
		code[front] == '\t' ||
		code[front] == '\n' ||
		code[front] == '\r')
		state = 3;
	    else state = 0;
	    break;
	case 4:
	    front++;
	    if (code[front] == '=')
		state = 5;
	    else if (code[front] == ' ' ||
		     code[front] == '\t' ||
		     code[front] == '\n' ||
		     code[front] == '\r')
		state = 4;
	    else
		state = 0;
	    break;
	case 5:
	    front++;
	    if (code[front] == ' ' ||
		code[front] == '\t' ||
		code[front] == '\n' ||
		code[front] == '\r')
		state = 5;
	    else
		state = 0;
	    break;
	case 6:
	    front++;
	    if (code[front] == '=')
		state = 7;
	    else if (code[front] == ' ' ||
		     code[front] == '\t' ||
		     code[front] == '\n' ||
		     code[front] == '\r')
		state = 6;
	    else 
		state = 0;
	    break;
	case 7:
	    front++;
	    if (code[front] == ' ' ||
		code[front] == '\t' ||
		code[front] == '\n' ||
		code[front] == '\r')
		state = 7;
	    else
		state = 0;
	    break;
	case 8:
	    front++;
	    if (code[front] == '=')
		state = 9;
	    else if (code[front] == ' ' ||
		     code[front] == '\t' ||
		     code[front] == '\n' ||
		     code[front] == '\r')
		state = 9;
	    else
		state = 0;
	    break;
	case 9:
	    front++;
	    if (code[front] == ' ' ||
		code[front] == '\t' ||
		code[front] == '\n' ||
		code[front] == '\r')
		state = 9;
	    else
		state = 0;
	    break;
	case 10:
	    front++;
	    if (code[front] == '=')
		state = 11;
	    else if (code[front] == ' ' ||
		     code[front] == '\t' ||
		     code[front] == '\n' ||
		     code[front] == '\r')
		state = 10;
	    else
		state = 0;
	    break;
	case 11:
	    front++;
	    if (code[front] == ' ' ||
		code[front] == '\t' ||
		code[front] == '\n' ||
		code[front] == '\r')
		state = 11;
	    else
		state = 0;
	    break;
	case 12:
	    front++;
	    if (code[front] == '=')
		state = 13;
	    else if (code[front] == ' ' ||
		     code[front] == '\t' ||
		     code[front] == '\n' ||
		     code[front] == '\r')
		state = 12;
	    else
		state = 0;
	    break;
	case 13:
	    front++;
	    if (code[front] == ' ' ||
		code[front] == '\t' ||
		code[front] == '\n' ||
		code[front] == '\r')
		state = 13;
	    else
		state = 0;
	    break;
	case 14:
	    front++;
	    if (code[front] == '+')
		state = 15;
	    else if (code[front] == '=')
		state = 16;
	    else if (code[front] == ' ' ||
		     code[front] == '\t' ||
		     code[front] == '\n' ||
		     code[front] == '\r')
		state = 14;
	    else
		state = 0;
	    break;
	case 15:
	    front++;
	    if (code[front] == ' ' ||
		code[front] == '\t' ||
		code[front] == '\n' ||
		code[front] == '\r')
		state = 15;
	    else 
		state = 0;
	    break;
	case 16:
	    front++;
	    if (code[front] == ' ' ||
		code[front] == '\t' ||
		code[front] == '\n' ||
		code[front] == '\r')
		state = 16;
	    else
		state = 0;
	    break;
	case 17:
	    front++;
	    if (code[front] == '-')
		state = 18;
	    else if (code[front] == '=')
		state = 19;
	    else if (code[front] == ' ' ||
		     code[front] == '\t' ||
		     code[front] == '\n' ||
		     code[front] == '\r')
		state = 17;
	    else
		state = 0;
	    break;
	case 18:
	    front++;
	    if (code[front] == ' ' ||
		code[front] == '\t' ||
		code[front] == '\n' ||
		code[front] == '\r')
		state = 18;
	    else 
		state = 0;
	    break;
	case 19:
	    front++;
	    if (code[front] == ' ' ||
		code[front] == '\t' ||
		code[front] == '\n' ||
		code[front] == '\r')
		state = 19;
	    else 
		state = 0;
	    break;
	case 20:
	    front++;
	    if (code[front] == '&')
		state = 21;
	    else if (code[front] == '=')
		state = 22;
	    else if (code[front] == ' ' ||
		     code[front] == '\t' ||
		     code[front] == '\n' ||
		     code[front] == '\r')
		state = 20;
	    else
		state = 0;
	    break;
	case 21:
	    front++;
	    if (code[front] == ' ' ||
		code[front] == '\t' ||
		code[front] == '\n' ||
		code[front] == '\r')
		state = 21;
	    else 
		state = 0;
	    break;
	case 22:
	    front++;
	    if (code[front] == ' ' ||
		code[front] == '\t' ||
		code[front] == '\n' ||
		code[front] == '\r')
		state = 22;
	    else
		state = 0;
	    break;
	case 23:
	    front++;
	    if(code[front] == '|')
		state = 24;
	    else if (code[front] == '=')
		state = 25;
	    else if (code[front] == ' ' ||
		     code[front] == '\t' ||
		     code[front] == '\n' ||
		     code[front] == '\r')
		state = 23;
	    else
		state = 0;
	    break;
	case 24:
	    front++;
	    if (code[front] == ' ' ||
		code[front] == '\t' ||
		code[front] == '\n' ||
		code[front] == '\r')
		state = 24;
	    else
		state = 0;
	    break;
	case 25:
	    front++;
	    if (code[front] == ' ' ||
		code[front] == '\t' ||
		code[front] == '\n' ||
		code[front] == '\r')
		state = 25;
	    else
		state = 0;
	    break;
	case 26:
	    front++;
	    if (code[front] == '<')
		state = 27;
	    else if (code[front] == '=')
		state = 29;
	    else if (code[front] == ' ' ||
		     code[front] == '\t' ||
		     code[front] == '\n' ||
		     code[front] == '\r')
		state = 26;
	    else
		state = 0;
	    break;
	case 27:
	    front++;
	    if (code[front] == '=')
		state = 28;
	    else if (code[front] == ' ' ||
		     code[front] == '\t' ||
		     code[front] == '\n' ||
		     code[front] == '\r')
		state = 27;
	    else
		state = 0;
	    break;
	case 28:
	    front++;
	    if (code[front] == ' ' ||
		code[front] == '\t' ||
		code[front] == '\n' ||
		code[front] == '\r')
		state = 28;
	    else
		state = 0;
	    break;
	case 29:
	    front++;
	    if (code[front] == ' ' ||
		code[front] == '\t' ||
		code[front] == '\n' ||
		code[front] == '\r')
		state = 29;
	    else
		state = 0;
	    break;
	case 30:
	    front++;
	    if (code[front] == '>')
		state = 31;
	    else if (code[front] == '=')
		state = 33;
	    else if (code[front] == ' ' ||
		     code[front] == '\t' ||
		     code[front] == '\n' ||
		     code[front] == '\r')
		state = 30;
	    else
		state = 0;
	    break;
	case 31:
	    front++;
	    if (code[front] == '=')
		state = 32;
	    else if (code[front] == ' ' ||
		     code[front] == '\t' ||
		     code[front] == '\n' ||
		     code[front] == '\r')
		state = 31;
	    else
		state = 0;
	    break;
	case 32:
	    front++;
	    if (code[front] == ' ' ||
		code[front] == '\t' ||
		code[front] == '\n' ||
		code[front] == '\r')
		state = 32;
	    else
		state = 0;
	    break;
	case 33:
	    front++;
	    if (code[front] == ' ' ||
		code[front] == '\t' ||
		code[front] == '\n' ||
		code[front] == '\r')
		state = 33;
	    else
		state = 0;
	    break;
	}
    } while (code[front] != ' '  &&
	     code[front] != '\n' &&
	     code[front] != '\t' &&
	     code[front] != '\r');
    if (state == 1 ||
	state == 2 ||
	state == 3 ||
	state == 4 ||
	state == 5 ||
	state == 6 ||
	state == 7 ||
	state == 8 ||
	state == 9 ||
	state == 11 ||
	state == 13 ||
	state == 14 ||
	state == 15 ||
	state == 16 ||
	state == 17 ||
	state == 18 ||
	state == 19 ||
	state == 20 ||
	state == 21 ||
	state == 22 ||
	state == 23 ||
	state == 24 ||
	state == 25 ||
	state == 26 ||
	state == 27 ||
	state == 28 ||
	state == 29 ||
	state == 30 ||
	state == 31 ||
	state == 32 ||
	state == 33)
	return front - begin + 1; 
    else
	return 0;
}

int symbol_scan(const char *code, int begin)
{
    int front, state;
    
    front = begin;
    switch (code[front]) {
    case '(':
	state = 1;
	break;
    case ')':
	state = 2;
	break;
    case '[':
	state = 3;
	break;
    case ']':
	state = 4;
	break;
    case '{':
	state = 5;
	break;
    case '}':
	state = 6;
	break;
    case ':':
	state = 7;
	break;
    case ';':
	state = 8;
	break;
    case ',':
	state = 9;
	break;
    case '?':
	state = 10;
	break;
    default:
	return 0;
	state = 0;
	break;
    }
    front++;
    if (code[front] == ' ' ||
	code[front] == '\t' ||
	code[front] == '\n' ||
	code[front] == '\r')
	return front - begin + 1; 
    else
	return 0;
}

int identity_scan(const char *code, int begin)
{
    int front, state;

    front = begin;
    if ((code[front] <= 90 && code[front] >= 65) ||
	(code[front] <= 122 && code[front] >= 97))
	state = 1;
    else
	state = 0;
    do {
	switch (state) {
	case 0:
	    return 0;
	    break;
	case 1:
	    front++;
	    if ((code[front] <= 90 && code[front] >= 65) ||
		(code[front] <= 122 && code[front] >= 97) ||
		(code[front] <= 57 && code[front] >= 48))
		state = 1;
	    break;
	}
    } while (code[front] != ' ' &&
	     code[front] != '\t' &&
	     code[front] != '\n' &&
	     code[front] != '\r');
    if (state == 1)
	return front - begin + 1; 
    else 
	return 0;
}


void number_load(const char *code, int begin, int len, 
		 struct token_table *tokens, 
		 struct symbol_table *symbols)
{
    int i, state, front;
    double v, step;

    if (len <= 2)
	fprintf(stderr, "number_load: len is less than 2: %d\n", len);
    v = 0;
    step = 0.1;
    state = 0;
    front = begin;
    tokens->table[tokens->length] = 
	(struct token *)malloc(sizeof(struct token));
    for (i = 0; i < len - 1; ++i)
	switch (state) {
	case 0:
	    v = v * 10 + (code[front] - 48);
	    front++;
	    if (code[front] == '.')
		state = 1;
	    break;
	case 1:
	    if (code[front] == '.') {
		front++;
		break;
	    } else {
		v += step * (code[front] - 48);
		step *= 0.1;
		front++;
		break;
	    }
	}
    tokens->table[tokens->length]->value = v;
    tokens->table[tokens->length]->tag = 1;
    tokens->table[tokens->length]->pointer = NULL;
    tokens->length++;
}

void char_load(const char *code, int begin, int len, 
	       struct token_table *tokens, 
	       struct symbol_table *symbols)
{
    if (len != 4)
	fprintf(stderr, "char_load: len is not 3: %d\n", len - 1);
    tokens->table[tokens->length] = 
	(struct token *)malloc(sizeof(struct token));
    tokens->table[tokens->length]->value = 0;
    tokens->table[tokens->length]->tag = 2;
    tokens->table[tokens->length]->pointer = 
	(char *)malloc(sizeof(char));
    *(tokens->table[tokens->length]->pointer) = code[begin + 1];
    tokens->length++;
}

void string_load(const char *code, int begin, int len,
		 struct token_table *tokens,
		 struct symbol_table *symbols)
{
    int i;

    if (len <= 3)
	fprintf(stderr, "string_load: len is less than 2: %d\n", len - 1);
    tokens->table[tokens->length] = 
	(struct token *)malloc(sizeof(struct token));
    tokens->table[tokens->length]->value = 0;
    tokens->table[tokens->length]->tag = 3;
    tokens->table[tokens->length]->pointer = 
	(char *)malloc(sizeof(char) * len);
    for (i = 0; i < len - 1; ++i)
	*(tokens->table[tokens->length]->pointer + i) = code[begin + i];
    *(tokens->table[tokens->length]->pointer + i) = '\0';
    tokens->length++;
}

void operator_load(const char *code, int begin, int len,
		   struct token_table *tokens,
		   struct symbol_table *symbols)
{
    int i;

    if (len < 2)
	fprintf(stderr, "operator_load: len is less than 1: %d\n", len - 1);
    tokens->table[tokens->length] = 
	(struct token *)malloc(sizeof(struct token));
    tokens->table[tokens->length]->tag = 4;
    tokens->table[tokens->length]->value = 0;
    tokens->table[tokens->length]->pointer = 
	(char *)malloc(sizeof(char) * len);
    for (i = 0; i < len - 1; ++i)
	*(tokens->table[tokens->length]->pointer + i) = code[begin + i];
    *(tokens->table[tokens->length]->pointer + i) = '\0';
    tokens->length++;
}

void symbol_load(const char *code, int begin, int len, 
		 struct token_table *tokens,
		 struct symbol_table *symbols)
{
    int i;

    if (len != 2)
	fprintf(stderr, "symbol_load: len is not 1: %d\n", len - 1);
    tokens->table[tokens->length] = 
	(struct token *)malloc(sizeof(struct token));
    tokens->table[tokens->length]->tag = 5;
    tokens->table[tokens->length]->value = 0;
    tokens->table[tokens->length]->pointer = 
	(char *)malloc(sizeof(char) * len);
    for (i = 0; i < len - 1; ++i)
	*(tokens->table[tokens->length]->pointer + i) = code[begin + i];
    *(tokens->table[tokens->length]->pointer + i) = '\0';
    tokens->length++;
}

void identity_load(const char *code, int begin, int len, 
		   struct token_table *tokens,
		   struct symbol_table *symbols)
{
    int i;

    if (len < 2)
	fprintf(stderr, "identity_load: len is less than 1: %d\n", len - 1);
    tokens->table[tokens->length] = 
	(struct token *)malloc(sizeof(struct token));
    tokens->table[tokens->length]->tag = 6;
    tokens->table[tokens->length]->value = 0;
    tokens->table[tokens->length]->pointer = 
	(char *)malloc(sizeof(char) * len);
    for (i = 0; i < len - 1; ++i)
	*(tokens->table[tokens->length]->pointer + i) = code[begin + i];
    *(tokens->table[tokens->length]->pointer + i) = '\0';
    tokens->length++;
}
