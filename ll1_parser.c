#include <stdio.h>
#include <stdlib.h>

#include "tokenizer.h"

#define terminal 0
#define non_terminal 1

typedef enum Terminal
{
    _EOF,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    LEFT_PARAN,
    RIGHT_PARAN,
    NAME,
    NUMBER
} TERMINAL; 

typedef enum nonTerminal
{
    GOAL,
    EXPR,
    EXPR_,
    TERM,
    TERM_,
    FACTOR
} NONTERMINAL;

typedef struct CFG
{
    int count;
    int* separater;     // 0 means Terminal , 1 means nonTerminal
    int* production;
} CFG;

/* Our Grammar is CFG, with recursive rightmost derivation - to make parser backtrack free */

/*                    Grammar

index           Left Side           Right Side
0                GOAL        ->       EXPR
1                EXPR        ->       TERM EXPR
2                EXPR_       ->       PLUS TERM EXPR_
3                EXPR_       ->       MINUS TERM EXPR_
4                EXPR_       ->       _EOF
5                TERM        ->       FACTOR TERM_
6                TERM_       ->       MULTIPLY FACTOR TERM_
7                TERM_       ->       DIVIDE FACTOR TERM_
8                TERM_       ->       _EOF
9                FACTOR      ->       LEFT_PARAN EXPR RIGHT_PARAN
10               FACTOR      ->       NUMBER
11               FACTOR      ->       NAME

*/

    int ll1_table[6][9] = 
    {
        {-1, -1, -1, -1, -1,  0, -1,  0,  0},
        {-1, -1, -1, -1, -1,  1, -1,  1,  1},
        { 4,  2,  3, -1, -1, -1,  4, -1, -1},
        {-1, -1, -1, -1, -1,  5, -1,  5,  5},
        { 8,  8,  8,  6,  6, -1,  8, -1, -1},
        {-1, -1, -1, -1, -1,  9, -1, 11, 10}
    };

CFG grammar[11];


// typedef struct Node
// {
//     bool op;
//     bool separater;  // 0 mean terminal ; 1 means non terminal

//     TERMINAL term;
//     NONTERMINAL non_term;

//     int value; // if any
//     char str[10]; // if any

//     struct Node* left;
//     struct Node* right;
// } NODE;

// NODE *root = (NODE*) malloc (sizeof(NODE));

typedef struct Node
{
    bool checker; // 0 mean terminal; 1 mean non terminal

    TERMINAL term;
    NONTERMINAL non_term;

    int value; // if any

    struct Node *left;
    struct Nod *right;
} NODE;

typedef struct stack
{
    int separater;  // 0 is terminal, 1 is non terminal
    TERMINAL tml;
    NONTERMINAL ntml;
} STACK;

int max_size = 25;       
STACK array[8];     
int top = -1;            

int isempty_() {

   if(top == -1)
      return 1;
   else
      return 0;
}
   
int isfull_() {

   if(top == max_size)
      return 1;
   else
      return 0;
}

STACK pop_() {
   STACK data;
	
   if(!isempty()) {
      data = array[top];
      top = top - 1;   
      return data;
   } else {
      printf("Could not retrieve data, Stack is empty.\n");
   }
}

STACK top_() {
   STACK data;
	
   if(!isempty()) {
      data = array[top];   
      return data;
   } else {
      printf("Could not retrieve data, Stack is empty.\n");
   }
}

void push_(STACK data) {

   if(!isfull()) {
      top = top + 1;   
      array[top] = data;
   } else {
      printf("Could not insert data, Stack is full.\n");
   }
}

void clear_()
{
    top = 0;
}

    TERMINAL get_token(char* c, TOKENS t)
    {
        if(!strcmp(c, "+"))
        {
            return PLUS;
        }
        else if(!strcmp(c, "-"))
        {
            return MINUS;
        }
        else if(!strcmp(c, "*"))
        {
            return MULTIPLY;
        }
        else if(!strcmp(c, "/"))
        {
            return DIVIDE;
        }
        else if(!strcmp(c, "("))
        {
            return LEFT_PARAN;
        }
        else if(!strcmp(c, ")"))
        {
            return RIGHT_PARAN;
        }
        else if(t == IDENTIFIER)
        {
            return NAME;
        }
        else if(t == VALUE)
        {
            return NUMBER;
        }
        else 
        {
            return _EOF;
        }
    }

int count = 0;

void parser_(NODE *curr)
{
    TERMINAL word = get_token(tbl[count].var, tbl[count].tkn);

    if(curr->term && curr->checker == 0)
    {
        NODE* t = (NODE*) malloc (sizeof(NODE));
        t->checker = terminal;
        t->term = curr->term;
        t->left = NULL;
        t->right = NULL;
        t->value = 1;
    }
    else
    {
        int k = (curr->checker == terminal) ? ll1_table[curr->term][word] : ll1_table[curr->non_term][word];
        int l = grammar[k].count;

        for(int i = l - 1; i > -1; i--)
        {
            if()
        }
    }
}

    void parser(CFG* grammar)
    {
        TERMINAL word = get_token(tbl[0].var, tbl[0].tkn);

        STACK s;
        s.separater = 0;
        s.tml = _EOF;
        push_(s);

        STACK start;
        start.separater = 1;
        start.ntml = GOAL;
        push_(start);

        STACK focus = top_();

        int count = 0;

        while(true)
        {
            if(focus.separater == 0 && focus.tml == _EOF && word ==_EOF)
            {
                printf("\n This is an absolute success.");
            }
            else if(focus.separater == 0 || focus.tml == _EOF)
            {
                if(focus.tml == word)
                {
                    STACK a = pop_();
                    // non terminal is identified
                    count++;
                    word = get_token(tbl[count].var, tbl[count].tkn);
                }
                else
                {
                    printf("\n Error looking for symbol at top of stack.");
                }
            }
            else
            {
                int k;
                if(focus.separater == 0)
                {
                    k = ll1_table[focus.tml][word];
                }
                else if(focus.separater == 1)
                {
                    k = ll1_table[focus.ntml][word];
                }
                int l = grammar[k].count;

                STACK temp = pop_();

                for(int i = l - 1; i > -1; i--)
                {
                    STACK s;
                    if(grammar[k].separater[i] == 0)
                    {
                        s.separater = 0;
                        s.tml = grammar[k].production[i];
                        if(s.tml != _EOF)
                        {
                            push_(s);
                        }
                    }
                    else if(grammar[k].separater[i] == 1)
                    {
                        s.separater = 1;
                        s.ntml = grammar[k].production[i];
                        push_(s);
                    }
                }
            }
            focus = top_();
        }
    }

    int main()
    {
        int count[] = {1, 2, 3, 3, 1, 2, 3, 3, 1, 3, 1, 1};

        for(int i = 0; i < 11; i++)
        {
            grammar[i].count = count[i];
            grammar[i].separater = (int*) malloc(count[i] * sizeof(int));
            grammar[i].production = (int*) malloc(count[i] * sizeof(int));
        }

        // set grammar manaully

        grammar[0].separater[0] = non_terminal;
        grammar[0].production[0] = EXPR;

        grammar[1].separater[0] = non_terminal;
        grammar[1].production[0] = TERM;
        grammar[1].separater[1] = non_terminal;
        grammar[1].production[1] = EXPR_;

        grammar[2].separater[0] = terminal;
        grammar[2].production[0] = PLUS;
        grammar[2].separater[1] = non_terminal;
        grammar[2].production[1] = TERM;
        grammar[2].separater[2] = non_terminal;
        grammar[2].production[2] = EXPR_;

        grammar[3].separater[0] = terminal;
        grammar[3].production[0] = MINUS;
        grammar[3].separater[1] = non_terminal;
        grammar[3].production[1] = TERM;
        grammar[3].separater[2] = non_terminal;
        grammar[3].production[2] = EXPR_;

        grammar[4].separater[0] = terminal;
        grammar[4].production[0] = _EOF;

        grammar[5].separater[0] = non_terminal;
        grammar[5].production[0] = FACTOR;
        grammar[5].separater[1] = non_terminal;
        grammar[5].production[1] = TERM_;

        grammar[6].separater[0] = terminal;
        grammar[6].production[0] = MULTIPLY;
        grammar[6].separater[1] = non_terminal;
        grammar[6].production[1] = FACTOR;
        grammar[6].separater[2] = non_terminal;
        grammar[6].production[2] = TERM_;

        grammar[7].separater[0] = terminal;
        grammar[7].production[0] = DIVIDE;
        grammar[7].separater[1] = non_terminal;
        grammar[7].production[1] = FACTOR;
        grammar[7].separater[2] = non_terminal;
        grammar[7].production[2] = TERM_;

        grammar[8].separater[0] = terminal;
        grammar[8].production[0] = _EOF;

        grammar[9].separater[0] = terminal;
        grammar[9].production[0] = LEFT_PARAN;
        grammar[9].separater[1] = non_terminal;
        grammar[9].production[1] = EXPR;
        grammar[9].separater[2] = terminal;
        grammar[9].production[2] = RIGHT_PARAN;

        grammar[10].separater[0] = terminal;
        grammar[10].production[0] = NUMBER;
        
        grammar[11].separater[0] = terminal;
        grammar[11].production[0] = NAME;

        parser(grammar);
    }