#pragma once
#include<stdio.h>
#include<ctype.h>
extern char token_buffer[];
typedef enum token_type{
    begin,end,read,write,id,identifier,rparem,lparem,semicolon,comma,assignOp,plusOp,minusOp,scanof
}token;
void init_buffer(){
    token_buffer[0]='\0';
}
void clear_buffer()
{
    int i=0;
    while(token_buffer[i]!='\0')token_buffer[i++]='\0';
}
void buffer_char(char car)
{
    
}
token scanner(FILE*file)
{
    char in_char,c;
    clear_buffer();
    if(feof(file))return scanof;
    while((in_char=getc(file))!=EOF)
    {
        if(isspace(in_char))continue;
        else if(isalpha(in_char)){
            buffer_char(in_char);
        }
    }
}