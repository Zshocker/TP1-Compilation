#pragma once
#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>
unsigned long NUMLIGNE=1;
char token_buffer[100];
/*typedef enum token_type{
    begin,end,read,write,id,intliteral,rparem,lparem,semicolon,comma,assignOp,plusOp,minusOp,floatt,scanof,err
}token;*/
char TableTokens[15][20]={"begin","end","read","write","id","intliteral","rparem","lparem","semicolon","comma","assignOp","plusOp","minusOp","float","scanof"};
typedef enum {
    begin,end,read,write,id,intliteral,rparem,lparem,semicolon,comma,assignOp,plusOp,minusOp,floatt,scanof,system_global,program,inst_list,inst,id_list,expression,expr_list,add_op,prim,err
}Axiom;
char TableAxiom[32][20]={"begin","end","read","write","id","intliteral","rparem","lparem","semicolon","comma","assignOp","plusOp","minusOp","float","scanof",
                        "system_global","program","inst_list","inst","id_list","expression","expr_list","add_op","prim"};
typedef struct ls
{
    void* Elem;
    struct ls *next;
}ListAr;
typedef struct Ar
{
    Axiom Ax;
    ListAr*children;
}AxiomAr;


void init_buffer()
{
    token_buffer[0]='\0';
}
void clear_buffer()
{
    int i=0;
    while(token_buffer[i]!='\0')token_buffer[i++]='\0';
}
void buffer_char(char car)
{
    strncat(token_buffer,&car,1);
}