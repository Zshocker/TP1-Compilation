#include"AxiomAr.c"
Axiom turnBufferToToken()
{
    for (int i = 0; i < 16; i++)
    {
        if(!strcmp(token_buffer,TableTokens[i]))return (Axiom)i;
    }
    return err;
}
Axiom next_token(FILE*F)
{
    init_buffer();
    char c;
    while(isspace(c=fgetc(F))){
    if(c=='\n'){
        NUMLIGNE++;
        }
        continue;
    }
    if(c!='['){
        printf("False reading token");
        exit(-1);
    }
    while((c=getc(F))!=']')
    {
        buffer_char(c);
    }
    return turnBufferToToken();
}
AxiomAr*Match(AxiomAr*Ar,Axiom A,Axiom ToMatch)
{
    if(A==ToMatch)
    {
        Ar_Insert_in_Fils(Ar,A);
    }else {
        syntax_Eror(A,ToMatch);
    }
    return Ar;
}
AxiomAr* Expression(FILE*F);

AxiomAr* Prim(FILE*F)
{
    Axiom tok=next_token(F);
    AxiomAr*Ar=create_AxiomAr(prim);
    switch (tok)
    {                               
    case lparem:
        Ar=Match(Ar,tok,lparem);
        Ar=Ar_Insert_in_Fils_Ar(Ar,Expression(F));
        tok=next_token(F);
        Ar=Match(Ar,tok,rparem);
        break;
    case id:
        Ar=Match(Ar,tok,id);
        break;
    case intliteral:
        Ar=Match(Ar,tok,intliteral);
        break;
    case floatt:
        Ar=Match(Ar,tok,floatt);
        break;    
    default:
        syntax_Eror(tok,prim);
        break;
    }
    return Ar;
}
AxiomAr*AddOP(FILE*F,Axiom t)
{
    AxiomAr*Ar=create_AxiomAr(add_op);
    if(t==plusOp){
       Ar=Match(Ar,t,plusOp);
    }else if(t==minusOp)
    {
        Ar=Match(Ar,t,minusOp);
    }else syntax_Eror(t,add_op);
    return Ar;
}
AxiomAr* Expression(FILE*F)
{
    Axiom t;
    AxiomAr*Ar=create_AxiomAr(expression);
    Ar=Ar_Insert_in_Fils_Ar(Ar,Prim(F));
    long rem=ftell(F);
    for ( t = next_token(F); t==plusOp||t==minusOp; t=next_token(F))
    {
        Ar=Ar_Insert_in_Fils_Ar(Ar,AddOP(F,t));
        Ar=Ar_Insert_in_Fils_Ar(Ar,Expression(F));
        rem = ftell(F);
    }
    fseek(F,rem,SEEK_SET);
    return Ar;
}
AxiomAr *Expr_list(FILE*F)
{
    AxiomAr*Ar=create_AxiomAr(expr_list);
    Ar=Ar_Insert_in_Fils_Ar(Ar,Expression(F));
     long rem=ftell(F);
    while (next_token(F)==comma)
    {
         Ar=Match(Ar,comma,comma);
         Ar=Ar_Insert_in_Fils_Ar(Ar,Expression(F));
         rem = ftell(F);
    }
    fseek(F,rem,SEEK_SET);
    return Ar;
}
AxiomAr*Id_list(FILE*F)
{
    AxiomAr*Ar=create_AxiomAr(id_list);
    Ar=Match(Ar,next_token(F),id);
    long rem=ftell(F);
    while (next_token(F)==comma)
    {
         Ar=Match(Ar,comma,comma);
         Ar=Match(Ar,next_token(F),id);
         rem = ftell(F);
    }
    fseek(F,rem,SEEK_SET);
    return Ar;
}
AxiomAr*Inst(FILE*F)
{
    AxiomAr*Ar=create_AxiomAr(inst);
    Axiom tok=next_token(F);
    switch (tok)
    {
    case id:
        Ar=Match(Ar,tok,id);
        Ar=Match(Ar,next_token(F),assignOp);
        Ar=Ar_Insert_in_Fils_Ar(Ar,Expression(F));
        Ar=Match(Ar,next_token(F),semicolon);
        break;
    case read:
        Ar=Match(Ar,tok,read);
        Ar=Match(Ar,next_token(F),lparem);
        Ar=Ar_Insert_in_Fils_Ar(Ar,Id_list(F));
        Ar=Match(Ar,next_token(F),rparem);
        Ar=Match(Ar,next_token(F),semicolon);
        break;
    case write:
        Ar=Match(Ar,tok,write);
        Ar=Match(Ar,next_token(F),lparem);
        Ar=Ar_Insert_in_Fils_Ar(Ar, Expr_list(F));
        Ar=Match(Ar,next_token(F),rparem);
        Ar=Match(Ar,next_token(F),semicolon);
        break;    
    default:
        syntax_Eror(tok,inst);
        break;
    }
    return Ar;
}
AxiomAr*Inst_list(FILE*F)
{
    AxiomAr*Ar=create_AxiomAr(inst_list);
    Ar=Ar_Insert_in_Fils_Ar(Ar,Inst(F));
    long rem=ftell(F);
    while (1)
    {
         rem=ftell(F);
         switch (next_token(F)) 
         {
         case id:
         case read:
         case write:
         fseek(F,rem,SEEK_SET);
         Ar=Ar_Insert_in_Fils_Ar(Ar,Inst(F));
         break;
         default:
            fseek(F,rem,SEEK_SET);
            return Ar;
            break;
         }
    }
    fseek(F,rem,SEEK_SET);
    return Ar;
}

AxiomAr*Program(FILE*F)
{
    AxiomAr*Ar=create_AxiomAr(program);
    Ar=Match(Ar,next_token(F),begin);
    Ar=Ar_Insert_in_Fils_Ar(Ar,Inst_list(F));
    Ar=Match(Ar,next_token(F),end);
    return Ar;
}
AxiomAr* System_global(char*cheminF)
{
    NUMLIGNE=1;
    FILE*F=fopen(cheminF,"r");
    AxiomAr*Ar=create_AxiomAr(system_global);
    Ar=Ar_Insert_in_Fils_Ar(Ar,Program(F));
    Ar=Match(Ar,next_token(F),scanof);
    return Ar;
}