#include<stdio.h>
#include"lexical_scanner.c"
#include"syntax.c"
//extern char token_buffer[];
int main()
{
    char*ls=analyse_lexical("test.txt");
    AxiomAr*Ar=System_global(ls);
    PrintMyAxiomAr(Ar);
    return 0;
}