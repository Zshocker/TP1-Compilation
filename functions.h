#pragma once
#include<stdio.h>
#include<ctype.h>
#include<string.h>
unsigned long NUMLIGNE=1;
char token_buffer[100];
typedef enum token_type{
    begin,end,read,write,id,identifier,intliteral,rparem,lparem,semicolon,comma,assignOp,plusOp,minusOp,scanof,err
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
    strncat(token_buffer,&car,1);
}
token lexical_error(FILE*f,char c,char*msg)
{
    printf('\n lexical error on line %l unexpected %c %s\n',NUMLIGNE,c,msg);
    exit(-1);
    return err;
}
token check_reserved(){
    if(strcasecmp(token_buffer,'begin'))return begin;
    if(strcasecmp(token_buffer,'end'))return end;    
    if(strcasecmp(token_buffer,'read'))return read;    
    if(strcasecmp(token_buffer,'write'))return write;    
    return err;
}
token scanner(FILE*file)
{
    char in_char,c;
    clear_buffer();
    if(feof(file))return scanof;
    while((in_char=getc(file))!=EOF)
    {
        if(isspace(in_char)){
            if(in_char=='\n')NUMLIGNE++;
            continue;
            }
        else if(isalpha(in_char)){
            buffer_char(in_char);
            for ( c = getc(file); isalnum(c)||c=='_' ;c=getc(file) ) buffer_char(c);      
            ungetc(c,file);
            return check_reserved();
        }else if(isdigit(in_char)){
            buffer_char(in_char);
            for ( c = getc(file); isdigit(c) ;c=getc(file) ) buffer_char(c);
            ungetc(c,file);
            return intliteral;
        }else if(in_char=='('){
            return lparem;
        }else if(in_char==')'){
            return rparem;
        }else if(in_char==';'){
            return semicolon;
        }else if(in_char==','){
            return comma;
        }else if(in_char=='+'){
            return plusOp;
        }else if(in_char==':'){
            c=getc(file);
            if(c=='=')return assignOp;
            ungetc(c,file);
            return lexical_error(file,in_char,'= was expected');
        }else if(in_char=='-'){
            c=getc(file);
            if(c=='-'){
                do in_char=getc(file); while(in_char!='\n'); 
            }else{
                ungetc(c,file);
                return minusOp;
            } 
        }else return lexical_error(file,in_char,' ');
    }
    return err;
}