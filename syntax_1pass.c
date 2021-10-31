#include"AxiomAr.c"
#include"lexical_scanner.c"

Axiom next_token_1pass(FILE*F)
{
    return scanner(F,NULL); 
}
AxiomAr* Expression_1pass(FILE*F);
AxiomAr* Prim_1pass(FILE*F)
{
    Axiom tok=next_token_1pass(F);
    AxiomAr*Ar=create_AxiomAr(prim);
    switch (tok)
    {
    case lparem:
        Ar=Match(Ar,tok,lparem);
        Ar=Ar_Insert_in_Fils_Ar(Ar,Expression_1pass(F));
        tok=next_token_1pass(F);
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
        Prim_1pass(F);
        break;           
    default:
        syntax_Eror(tok,prim);
        Ar=Ar_Insert_in_Fils(Ar,LexErr);
        break;
    }
    return Ar;
}
AxiomAr*AddOP_1pass(FILE*F,Axiom t)
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
AxiomAr* Expression_1pass(FILE*F)
{
    Axiom t;
    AxiomAr*Ar=create_AxiomAr(expression);
    Ar=Ar_Insert_in_Fils_Ar(Ar,Prim_1pass(F));
    FilePlace rem=RememberPlace(F);
    for ( t = next_token_1pass(F); t==plusOp||t==minusOp; t=next_token_1pass(F))
    {
        Ar=Ar_Insert_in_Fils_Ar(Ar,AddOP_1pass(F,t));
        Ar=Ar_Insert_in_Fils_Ar(Ar,Expression_1pass(F));
        rem=RememberPlace(F);
    }
   ReturnToplace(F,rem);
    return Ar;
}
AxiomAr *Expr_list_1pass(FILE*F)
{
    AxiomAr*Ar=create_AxiomAr(expr_list);
    Ar=Ar_Insert_in_Fils_Ar(Ar,Expression_1pass(F));
     FilePlace rem=RememberPlace(F);
    while (next_token_1pass(F)==comma)
    {
         Ar=Match(Ar,comma,comma);
         Ar=Ar_Insert_in_Fils_Ar(Ar,Expression_1pass(F));
         rem=RememberPlace(F);
    }
   ReturnToplace(F,rem);
    return Ar;
}
AxiomAr*Id_list_1pass(FILE*F)
{
    AxiomAr*Ar=create_AxiomAr(id_list);
    Ar=Match(Ar,next_token_1pass(F),id);
    FilePlace rem=RememberPlace(F);
    while (next_token_1pass(F)==comma)
    {
         Ar=Match(Ar,comma,comma);
         Ar=Match(Ar,next_token_1pass(F),id);
         rem=RememberPlace(F);
    }
   ReturnToplace(F,rem);
    return Ar;
}
AxiomAr*Inst_1pass(FILE*F,Axiom*s)
{
    AxiomAr*Ar=create_AxiomAr(inst);
    Axiom tok;
    if(*s!=LexErr)
    {
        tok=*s;
    }else tok=next_token_1pass(F);
    switch (tok)
    {
    case id:
        Ar=Match(Ar,tok,id);
        Ar=Match(Ar,next_token_1pass(F),assignOp);
        Ar=Ar_Insert_in_Fils_Ar(Ar,Expression_1pass(F));
        Ar=Match(Ar,*s=next_token_1pass(F),semicolon);
        break;
    case read:
        Ar=Match(Ar,tok,read);
        Ar=Match(Ar,next_token_1pass(F),lparem);
        Ar=Ar_Insert_in_Fils_Ar(Ar,Id_list_1pass(F));
        Ar=Match(Ar,tok=next_token_1pass(F),rparem); 
        if(tok==semicolon)Ar=Match(Ar,*s=tok,semicolon);
        else Ar=Match(Ar,*s=next_token_1pass(F),semicolon);
        break;
    case write:
        Ar=Match(Ar,tok,write);
        Ar=Match(Ar,next_token_1pass(F),lparem);
        Ar=Ar_Insert_in_Fils_Ar(Ar, Expr_list_1pass(F));
        Ar=Match(Ar,tok=next_token_1pass(F),rparem);
        if(tok==semicolon)Ar=Match(Ar,*s=tok,semicolon);
        else Ar=Match(Ar,*s=next_token_1pass(F),semicolon);
        break;    
    default:
        Ar=Match(Ar,*s=syntax_Eror(tok,inst),LexErr);
        break;
    }
    return Ar;
}
AxiomAr*Inst_list_1pass(FILE*F)
{
    Axiom tok=LexErr;
    AxiomAr*Ar=create_AxiomAr(inst_list);
    FilePlace rem=RememberPlace(F);
    do
    {
        tok=LexErr;
        Ar=Ar_Insert_in_Fils_Ar(Ar,Inst_1pass(F,&tok));
    }while(tok!=semicolon&&tok!=end);
    tok=LexErr;
    while (1)
    {
        rem=RememberPlace(F);
        switch (next_token_1pass(F)) 
        {
        case id:
        case read:
        case write:
        ReturnToplace(F,rem);
        do{
            tok=LexErr;
            Ar=Ar_Insert_in_Fils_Ar(Ar,Inst_1pass(F,&tok));
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

AxiomAr*Program_1pass(FILE*F)
{
    AxiomAr*Ar=create_AxiomAr(program);
    Ar=Match(Ar,next_token_1pass(F),begin);
    Ar=Ar_Insert_in_Fils_Ar(Ar,Inst_list_1pass(F));
    Ar=Match(Ar,next_token_1pass(F),end);
    return Ar;
}
AxiomAr* System_global_1pass(char*cheminF)
{
    NUMLIGNE=1;
    FILE*F=fopen(cheminF,"r");
    AxiomAr*Ar=create_AxiomAr(system_global);
    Ar=Ar_Insert_in_Fils_Ar(Ar,Program_1pass(F));
    Ar=Match(Ar,next_token_1pass(F),scanof);
    return Ar;
}