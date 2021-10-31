#include"AxiomAr.c"
#include"lexical_scanner.c"

Axiom next_token_1pass(FILE*F)
{
    return scanner(F,NULL); 
}
AxiomAr*Match_1pass(AxiomAr*Ar,Axiom A,Axiom ToMatch)
{
    if(A==ToMatch)
    {
        Ar_Insert_in_Fils(Ar,A);
    }else {
        syntax_Eror(A,ToMatch);
    }
    return Ar;
}
AxiomAr* Expression_1pass(FILE*F);

AxiomAr* Prim_1pass(FILE*F)
{
    Axiom tok=next_token_1pass(F);
    AxiomAr*Ar=create_AxiomAr(prim);
    switch (tok)
    {
    case lparem:
        Ar=Match_1pass(Ar,tok,lparem);
        Ar=Ar_Insert_in_Fils_Ar(Ar,Expression_1pass(F));
        tok=next_token_1pass(F);
        Ar=Match_1pass(Ar,tok,rparem);
        break;
    case id:
        Ar=Match_1pass(Ar,tok,id);
        break;
    case intliteral:
        Ar=Match_1pass(Ar,tok,intliteral);
        break;
    case floatt:
        Ar=Match_1pass(Ar,tok,floatt);
        break;    
    default:
        syntax_Eror(tok,prim);
        break;
    }
    return Ar;
}
AxiomAr*AddOP_1pass(FILE*F,Axiom t)
{
    AxiomAr*Ar=create_AxiomAr(add_op);
    if(t==plusOp){
       Ar=Match_1pass(Ar,t,plusOp);
    }else if(t==minusOp)
    {
        Ar=Match_1pass(Ar,t,minusOp);
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
         Ar=Match_1pass(Ar,comma,comma);
         Ar=Ar_Insert_in_Fils_Ar(Ar,Expression_1pass(F));
         rem=RememberPlace(F);
    }
   ReturnToplace(F,rem);
    return Ar;
}
AxiomAr*Id_list_1pass(FILE*F)
{
    AxiomAr*Ar=create_AxiomAr(id_list);
    Ar=Match_1pass(Ar,next_token_1pass(F),id);
    FilePlace rem=RememberPlace(F);
    while (next_token_1pass(F)==comma)
    {
         Ar=Match_1pass(Ar,comma,comma);
         Ar=Match_1pass(Ar,next_token_1pass(F),id);
         rem=RememberPlace(F);
    }
   ReturnToplace(F,rem);
    return Ar;
}
AxiomAr*Inst_1pass(FILE*F)
{
    AxiomAr*Ar=create_AxiomAr(inst);
    Axiom tok=next_token_1pass(F);
    switch (tok)
    {
    case id:
        Ar=Match_1pass(Ar,tok,id);
        Ar=Match_1pass(Ar,next_token_1pass(F),assignOp);
        Ar=Ar_Insert_in_Fils_Ar(Ar,Expression_1pass(F));
        Ar=Match_1pass(Ar,next_token_1pass(F),semicolon);
        break;
    case read:
        Ar=Match_1pass(Ar,tok,read);
        Ar=Match_1pass(Ar,next_token_1pass(F),lparem);
        Ar=Ar_Insert_in_Fils_Ar(Ar,Id_list_1pass(F));
        Ar=Match_1pass(Ar,next_token_1pass(F),rparem);
        Ar=Match_1pass(Ar,next_token_1pass(F),semicolon);
        break;
    case write:
        Ar=Match_1pass(Ar,tok,write);
        Ar=Match_1pass(Ar,next_token_1pass(F),lparem);
        Ar=Ar_Insert_in_Fils_Ar(Ar, Expr_list_1pass(F));
        Ar=Match_1pass(Ar,next_token_1pass(F),rparem);
        Ar=Match_1pass(Ar,next_token_1pass(F),semicolon);
        break;    
    default:
        syntax_Eror(tok,inst);
        break;
    }
    return Ar;
}
AxiomAr*Inst_list_1pass(FILE*F)
{
    AxiomAr*Ar=create_AxiomAr(inst_list);
    Ar=Ar_Insert_in_Fils_Ar(Ar,Inst_1pass(F));
    FilePlace rem=RememberPlace(F);
    while (1)
    {
         rem=RememberPlace(F);
         switch (next_token_1pass(F)) 
         {
         case id:
         case read:
         case write:
        ReturnToplace(F,rem);
         Ar=Ar_Insert_in_Fils_Ar(Ar,Inst_1pass(F));
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
    Ar=Match_1pass(Ar,next_token_1pass(F),begin);
    Ar=Ar_Insert_in_Fils_Ar(Ar,Inst_list_1pass(F));
    Ar=Match_1pass(Ar,next_token_1pass(F),end);
    return Ar;
}
AxiomAr* System_global_1pass(char*cheminF)
{
    NUMLIGNE=1;
    FILE*F=fopen(cheminF,"r");
    AxiomAr*Ar=create_AxiomAr(system_global);
    Ar=Ar_Insert_in_Fils_Ar(Ar,Program_1pass(F));
    Ar=Match_1pass(Ar,next_token_1pass(F),scanof);
    return Ar;
}