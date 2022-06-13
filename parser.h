#include "tokenizer.h"

#define identifier -1

typedef enum OPT
{
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    EQUAL,
    NONE
} OPT;

typedef struct Node
{
    struct Node* left;
    struct Node* right;
    TOKENS tkn;
    OPT opera;
    float value; // if tkn is INT or FLOAT
} Node;

typedef struct AST
{
    Node* start;
} AST;

AST resultTree;

Node* parseE();
Node* parseF();
Node* parseT();

int count = -1;
TOKENS tkn;

Node* make_node(Node* a, Node* b)
{
    Node* temp   = (Node*) malloc (sizeof(Node));
    temp->left   = a;
    temp->right  = b;

    return temp;
}

Node* parseF()
{
    Node* n = (Node*) malloc (sizeof(Node));

    if(tkn == ID)
    {
        count++;

        tkn         = tbl[count].tkn;
        n->value    = identifier;
        n->opera    = NONE;
        n->tkn      = ID;

        return n;
    }

    else if(tkn == VALUE)
    {
        n->value    = atof(tbl[count].var);
        n->opera    = NONE;
        n->tkn      = VALUE;

        count++;

        tkn = tbl[count].tkn;

        return n;
    }

    else if(tkn == PARANTHESIS & !strcmp(tbl[count].var, "("))
    {
        count++;

        tkn     = tbl[count].tkn;
        Node* a = parseE();

        if(tkn == PARANTHESIS & !strcmp(tbl[count].var, ")"))
        {
            count++;

            tkn = tbl[count].tkn;

            return a;
        }

        else
        {
            return NULL;
        }
    }

    else
    {
        return NULL;
    }
}

Node* parseT()
{
    Node* a = parseF();

    while(true)
    {
        if(tkn == OPERATOR & !strcmp(tbl[count].var, "*"))
        {
            count++;

            tkn         = tbl[count].tkn;
            Node* b     = parseF();
            a           = make_node(a, b);
            a->opera    = MULTIPLY;
            a->tkn      = OPERATOR;
        }

        else if(tkn == OPERATOR & !strcmp(tbl[count].var, "/"))
        {
            count++;

            tkn         = tbl[count].tkn;
            Node* b     = parseF();
            a           = make_node(a, b);
            a->opera    = DIVIDE;
            a->tkn      = OPERATOR;
        }
        return a;
    }
}

Node* parseE()
{
    Node* a = parseT();

    while(true)
    {
        if(tkn == OPERATOR & !strcmp(tbl[count].var, "+"))
        {
            count++;

            tkn         = tbl[count].tkn;
            Node* b     = parseT();
            a           = make_node(a, b);
            a->opera    = PLUS;
            a->tkn      = OPERATOR;
        }

        else if(tkn == OPERATOR & !strcmp(tbl[count].var, "-"))
        {
            count++;

            tkn         = tbl[count].tkn;
            Node* b     = parseT();
            a           = make_node(a, b);
            a->opera    = MINUS;
            a->tkn      = OPERATOR;
        }   

        return a;
    }
}

void parser_main()
{
    count++;

    resultTree.start = (Node*) malloc (sizeof(Node));
    tkn              = tbl[count].tkn;
    Node* start      = parseE();
    
    printf("%f", start->left->value);
    printf("  %f", start->right->value);
}