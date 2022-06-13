#include "parser.h"

int main()
{
    char* e = "2 / 3 - 4";
    scanner(e);
    parser_main();
}