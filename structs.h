#pragma once
#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>
unsigned long NUMLIGNE=1;
char token_buffer[100];
typedef enum token_type{
    begin,end,read,write,id,intliteral,rparem,lparem,semicolon,comma,assignOp,plusOp,minusOp,floatt,scanof,err
}token;
char TableTokens[16][20]={"begin","end","read","write","id","intliteral","rparem","lparem","semicolon","comma","assignOp","plusOp","minusOp","float","scanof"};
typedef enum {
    begin,end,read,write,id,intliteral,rparem,lparem,semicolon,comma,assignOp,plusOp,minusOp,floatt,scanof,system_global,program,inst_list,inst,id_list,expression,expr_list,add_op,prim,err
}Axiom;
char TableAxiom[32][20]={"begin","end","read","write","id","intliteral","rparem","lparem","semicolon","comma","assignOp","plusOp","minusOp","float","scanof",
                        "system_global","program","inst_list","expression","expr_list","add_op","prim"};