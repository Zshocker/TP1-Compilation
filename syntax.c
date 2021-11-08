#include"AxiomAr.c"
#include"lexical_scanner.c"
FilePlace WQ;
Axiom turnBufferToToken()
{
    for (int i = 0; i < 16; i++)
    {
        if(!strcmp(token_buffer,TableTokens[i]))return (Axiom)i;
    }
    return LexErr;
}
Axiom next_token(FILE*F)
{
    WQ=RememberPlace(F);
    if(Mode==2){
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
        return LexErr;
    }
    while((c=getc(F))!=']')
    {
        buffer_char(c);
    }
    return turnBufferToToken();
    }else if(Mode==1) return scanner(F,NULL);
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
    case LexErr:
        Ar=Match(Ar,tok,LexErr);
        syntax_Eror(tok,prim);
        Prim(F);
        break;   
    default:
        syntax_Eror(tok,prim);
        Ar=Ar_Insert_in_Fils(Ar,LexErr);
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
    }else{
        Ar=Match(Ar,syntax_Eror(t,add_op),LexErr);
         }
    return Ar;
}
AxiomAr* Expression(FILE*F)
{
    Axiom t;
    AxiomAr*Ar=create_AxiomAr(expression);
    Ar=Ar_Insert_in_Fils_Ar(Ar,Prim(F));
    FilePlace rem=RememberPlace(F);
    for ( t = next_token(F); t==plusOp||t==minusOp; t=next_token(F))
    {
        Ar=Ar_Insert_in_Fils_Ar(Ar,AddOP(F,t));
        Ar=Ar_Insert_in_Fils_Ar(Ar,Expression(F));
        rem = RememberPlace(F);
    }
   ReturnToplace(F,rem);
    return Ar;
}
AxiomAr *Expr_list(FILE*F)
{
    AxiomAr*Ar=create_AxiomAr(expr_list);
    Ar=Ar_Insert_in_Fils_Ar(Ar,Expression(F));
     FilePlace rem=RememberPlace(F);
    while (next_token(F)==comma)
    {
         Ar=Match(Ar,comma,comma);
         Ar=Ar_Insert_in_Fils_Ar(Ar,Expression(F));
         rem = RememberPlace(F);
    }
   ReturnToplace(F,rem);
    return Ar;
}
AxiomAr*Id_list(FILE*F)
{
    AxiomAr*Ar=create_AxiomAr(id_list);
    Ar=Match(Ar,next_token(F),id);
    FilePlace rem=RememberPlace(F);
    while (next_token(F)==comma)
    {
         Ar=Match(Ar,comma,comma);
         Ar=Match(Ar,next_token(F),id);
         rem = RememberPlace(F);
    }
   ReturnToplace(F,rem);
    return Ar;
}
AxiomAr*Inst(FILE*F,Axiom*s)
{
    FilePlace rem;
    AxiomAr*Ar=create_AxiomAr(inst);
    Axiom tok;
    if(*s!=LexErr)
    {
        tok=*s;
    }else tok=next_token(F);
    switch (tok)
    {
    case id:
        Ar=Match(Ar,tok,id);
        Ar=Match(Ar,next_token(F),assignOp);
        Ar=Ar_Insert_in_Fils_Ar(Ar,Expression(F));
        Ar=Match(Ar,*s=next_token(F),semicolon);
        break;
    case read:
        Ar=Match(Ar,tok,read);
        Ar=Match(Ar,next_token(F),lparem);
        Ar=Ar_Insert_in_Fils_Ar(Ar,Id_list(F));
        Ar=Match(Ar,tok=next_token(F),rparem); 
        if(tok==semicolon)Ar=Match(Ar,*s=tok,semicolon);
        else Ar=Match(Ar,*s=next_token(F),semicolon);
        break;
    case write:
        Ar=Match(Ar,tok,write);
        Ar=Match(Ar,next_token(F),lparem);
        Ar=Ar_Insert_in_Fils_Ar(Ar, Expr_list(F));
        Ar=Match(Ar,tok=next_token(F),rparem);
        if(tok==semicolon)Ar=Match(Ar,*s=tok,semicolon);
        else Ar=Match(Ar,*s=next_token(F),semicolon);
        break;    
    default:
        Ar=Match(Ar,*s=syntax_Eror(tok,inst),LexErr);
        int ns=NUMLIGNE;
        while((tok=next_token(F))!=scanof&& ns==NUMLIGNE)
        {
            Ar_Insert_in_Fils(Ar,tok);
        }
        ReturnToplace(F,WQ);
        break;
    }
    return Ar;
}
AxiomAr*Inst_list(FILE*F)
{
    Axiom tok=LexErr;
    AxiomAr*Ar=create_AxiomAr(inst_list);
    FilePlace rem=RememberPlace(F);
    do
    {
        tok=LexErr;
        Ar=Ar_Insert_in_Fils_Ar(Ar,Inst(F,&tok));
    }while(tok!=semicolon&&tok!=end);
    tok=LexErr;
    while (1)
    {
        rem=RememberPlace(F);
        switch (next_token(F)) 
        {
        case id:
        case read:
        case write:
        ReturnToplace(F,rem);
        do{
            tok=LexErr;
            Ar=Ar_Insert_in_Fils_Ar(Ar,Inst(F,&tok));
        }while(tok!=semicolon&&tok!=end);
        break;
        default:
            ReturnToplace(F,rem);
            return Ar;
            break;
        }
    }
   ReturnToplace(F,rem);
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