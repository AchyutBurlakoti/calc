#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// maximum number of any particular tokens in any expression
#define MAX_TKN 20
#define MAX_EXPR 30

typedef enum TOKENS
{
    IDENTIFIER,
    VALUE,
    OPERATOR,
    DOT,
    PARANTHESIS 
} TOKENS;

typedef enum STATE
{
    START,
    INT,
    FLOAT,
    ID,
    OP,
    PARAN,
    ERR,
    BAD
} STATE;

STATE transition_table[6][5] = {

    { ID, INT, OP, ERR, PARAN },
    { ERR, INT, ERR, FLOAT, ERR },
    { ERR, FLOAT, ERR, ERR, ERR },
    { ID, ERR, ERR, ERR, ERR },
    { ERR, ERR, ERR, ERR, ERR },
    { ERR, ERR, ERR, ERR, ERR },

};

STATE state;

typedef struct Table{
    TOKENS tkn;
    char var[10];
} Table;

Table tbl[MAX_TKN];
int tbl_ptr = 0;

int MAXSIZE = 8;       
STATE stack[8];     
int top = -1;            

int isempty() {

   if(top == -1)
      return 1;
   else
      return 0;
}
   
int isfull() {

   if(top == MAXSIZE)
      return 1;
   else
      return 0;
}

int peek() {
   return stack[top];
}

STATE pop() {
   STATE data;
	
   if(!isempty()) {
      data = stack[top];
      top = top - 1;   
      return data;
   } else {
      printf("Could not retrieve data, Stack is empty.\n");
   }
}

void push(STATE data) {

   if(!isfull()) {
      top = top + 1;   
      stack[top] = data;
   } else {
      printf("Could not insert data, Stack is full.\n");
   }
}

void clear()
{
    top = 0;
}

void get_input(char* inp)
{
    printf(">>> ");
    scanf(" %s", inp);
}

char new_expr[MAX_EXPR];

char* remove_space(char* expr)
{
    int i = 0;
    int j = 0;
    while(expr[i] != '\0')
    {
        if(expr[i] != ' ')
        {
            new_expr[j] = expr[i];
            j++;
        }
        i++;
    }
    return new_expr;
}

TOKENS CharCat(char c)
{
    if(c == '+' | c == '-' | c == '/' | c == '*' | c == '=')
    {
        return OPERATOR;
    }

    else if(c == '.')
    {
        return DOT;
    }

    else if((c > 96 & c < 123) | (c > 64 & c < 91))
    {
        return IDENTIFIER;
    }

    else if(c > 47 & c < 58)
    {
        return VALUE;
    }

    else if(c == '(' | c == ')')
    {
        return PARANTHESIS;
    }
}

STATE delta(STATE state, TOKENS cat)
{
    return transition_table[state][cat];
}

char lexeme[MAX_EXPR];

void scanner(char* _expr)
{
    char* expr  = remove_space(_expr);
    STATE state = START;

    TOKENS previous_tkn;

    clear();
    push(BAD);

    int expr_ptr    = 0;
    int lexeme_ptr  = 0;

    while(expr[expr_ptr] != '\0')
    {
        state = START;

        while(state != ERR)
        {
            TOKENS cat;

            char c              = expr[expr_ptr];
            previous_tkn        = cat;
            lexeme[lexeme_ptr]  = c;

            expr_ptr++;
            lexeme_ptr++;

            // if state <-- Accepting state
            if (state == ID | state == INT | state == FLOAT | state == OP)
            {
                clear();
            }

            push(state);

            cat     = CharCat(c);
            state   = delta(state, cat);
        }

        lexeme[lexeme_ptr-1] = '\0';

        while((state != ID & state != INT & state != FLOAT & state != OP & state != PARAN) & state != BAD)
        {
            state = pop();
        }

        if(state == ID | state == INT | state == FLOAT | state == OP | state == PARAN)
        {
            tbl[tbl_ptr].tkn = previous_tkn;
            int i            = 0;

            while(lexeme[i] != '\0')
            {
                tbl[tbl_ptr].var[i] = lexeme[i];
                i++;
            }

            tbl_ptr++;
        }
        
        memset(lexeme, 0, MAX_EXPR);
        lexeme_ptr = 0;
        expr_ptr--;
    }
}