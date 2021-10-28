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
void syntax_Eror(Axiom A,Axiom Expected)
{
    printf("\nSyntax Error %s detected %s was expected in line %li \n",TableAxiom[A],TableAxiom[Expected],NUMLIGNE);
    exit(-1);
}
Axiom lexical_error(FILE*file,char c,char*msg)
{
    printf("\n lexical error on line %d unexpected %c %s\n",NUMLIGNE,c,msg);
    fclose(file);
    exit(-1);
    return err;
}
void print_fichier(char *chemin)
{
    FILE *file=fopen(chemin,"r");
    char c;
    if(file){
        while((c=getc(file))!=EOF)printf("%c",c);
    }
    fclose(file);
}