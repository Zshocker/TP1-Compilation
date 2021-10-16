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
char TableTokens[16][20]={"begin","end","read","write","id","intliteral","rparem","lparem","semicolon","comma","assignOp","plusOp","minusOp","float"};

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
token lexical_error(FILE*file,char c,char*msg)
{
    printf("\n lexical error on line %d unexpected %c %s\n",NUMLIGNE,c,msg);
    fclose(file);
    exit(-1);
    return err;
}
token check_reserved()
{
    if(!strcasecmp(token_buffer,"begin"))return begin;
    if(!strcasecmp(token_buffer,"end"))return end;    
    if(!strcasecmp(token_buffer,"read"))return read;    
    if(!strcasecmp(token_buffer,"write"))return write;    
    return id;
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
        }else if(isdigit(in_char)||in_char=='.'){
            buffer_char(in_char);
            int cmppoit=0;
            int cmpe=0;
            if(in_char=='.')cmppoit++;
            for ( c = getc(file); isdigit(c)||c=='e'||c=='E'||c=='.' ;c=getc(file) )
            { 
                if(c=='.')cmppoit++;
                else if(c=='e'||c=='E')cmpe++;
                if(cmppoit>1)
                lexical_error(file,c,"mulitple . detected");
                if(cmpe>1)lexical_error(file,c,"mulitple e detected");
                buffer_char(c);
            }
            ungetc(c,file);
            if(cmpe>0||cmppoit>0)return floatt;
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
            return lexical_error(file,c,"= was expected");
        }else if(in_char=='-'){
            c=getc(file);
            if(c=='-'){
                do in_char=getc(file); while(in_char!='\n'); 
            }else{
                ungetc(c,file);
                return minusOp;
            } 
        }else return lexical_error(file,in_char," ");
    }
    return scanof;
}
void putInFile(char*car,FILE*file)
{
    fprintf(file,"[%s]",car);
}
char* analyse_lexical(char*chemin)
{
    FILE*CodeSource;
    CodeSource=fopen(chemin,"r");
    token tok;
    char*Lexia="Lexical_file.txt";
    FILE*LEX=fopen(Lexia,"w");
    while((tok=scanner(CodeSource))!=scanof)
    {
        putInFile(TableTokens[tok],LEX);
    }
    fclose(CodeSource);
    fclose(LEX);
    return Lexia;
}