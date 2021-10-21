#include<stdio.h>
#include"lexical_scanner.h"
//extern char token_buffer[];
int main()
{
    char*ls=analyse_lexical("test.txt");
    return 0;
}