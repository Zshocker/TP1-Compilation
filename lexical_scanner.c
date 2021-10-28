#pragma once
#include"structs.h"

Axiom check_reserved()
{
    if(!strcasecmp(token_buffer,"begin"))return begin;
    if(!strcasecmp(token_buffer,"end"))return end;    
    if(!strcasecmp(token_buffer,"read"))return read;    
    if(!strcasecmp(token_buffer,"write"))return write;    
    return id;
}
Axiom scanner(FILE*file,FILE*LEX)
{
    char in_char,c;
    clear_buffer();
    if(feof(file))return scanof;
    while((in_char=getc(file))!=EOF)
    {
        if(isspace(in_char)){
            if(in_char=='\n'){
                NUMLIGNE++;
                if(LEX)fprintf(LEX,"\n");
            }
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
            for ( c = getc(file); isdigit(c)||c=='.' ;c=getc(file) )
            { 
                if(c=='.')cmppoit++;
                if(cmppoit>1)return lexical_error(file,c,"multiple . detected in a float");
                buffer_char(c);
            }
            if(c=='e'||c=='E')
            {
                cmpe++;
                c=getc(file);
                if(c=='+'||c=='-')
                {
                    if(isdigit(getc(file)))for ( c = getc(file); isdigit(c) ;c=getc(file) ) buffer_char(c); 
                    else return lexical_error(file,c,"a number was expected in exp of the float after + or -");
                }else if(isdigit(c))for ( c = getc(file); isdigit(c) ;c=getc(file) ) buffer_char(c); 
                else return lexical_error(file,c,"a + or a - or a number was expected in exp of the float");
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
    NUMLIGNE=1;
    FILE*CodeSource;
    CodeSource=fopen(chemin,"r");
    Axiom tok;
    char*Lexia="Lexical_file.txt";
    FILE*LEX=fopen(Lexia,"w");
    while((tok=scanner(CodeSource,LEX))!=scanof)
    {
        putInFile(TableTokens[tok],LEX);
    }
    putInFile(TableTokens[scanof],LEX);
    fclose(CodeSource);
    fclose(LEX);
    return Lexia;
}