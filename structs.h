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
