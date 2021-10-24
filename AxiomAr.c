#define _CRT_SECURE_NO_WARNINGS
#include"structs.h"
#define Massive 10000
char MassiveString[Massive];
void init_Mass()
{
    MassiveString[0]='\0';
}
void append(char* car)
{
    strcat(MassiveString,car);
}
void append_car(char car)
{
    strncat(MassiveString,&car,1);
}
AxiomAr *create_AxiomAr(Axiom A)
{
    AxiomAr *ND=(AxiomAr*)malloc(sizeof(AxiomAr));
    if(!ND){
        printf("Error Out of memory\n");
        exit(1);
    }
    ND->Ax=A;
    ND->children=NULL;
    return ND;
}
ListAr*Create_Child(AxiomAr*A)
{   
    ListAr *ND=(ListAr*)malloc(sizeof(ListAr));
    if(!ND){
        printf("Error Out of memory\n");
        exit(1);
    }
    ND->Elem=A;
    ND->next=NULL;
    return ND;
}
AxiomAr*Ar_Insert_in_Fils(AxiomAr*Pere,Axiom A)
{
    if(!Pere){
        Pere=create_AxiomAr(A);
        return Pere;
    }
    ListAr *crt=Pere->children;
    if(!crt){
        Pere->children=Create_Child(create_AxiomAr(A));
        return Pere;
    }
    while(crt->next)crt=crt->next;
    crt->next=Create_Child(create_AxiomAr(A));
    return Pere;
}
AxiomAr*Ar_Insert_in_Fils_Ar(AxiomAr*Pere,AxiomAr*Ar)
{
    if(!Pere){
        return Ar;
    }
    ListAr *crt=Pere->children;
    if(!crt){
        Pere->children=Create_Child(Ar);
        return Pere;
    }
    while(crt->next)crt=crt->next;
    crt->next=Create_Child(Ar);
    return Pere;
}
char* turnAxiom(Axiom A)
{
    return TableAxiom[A];
}

void PutArbInMassStr(AxiomAr*root,char*prefix,char*childPrefix)
{
    if(prefix)append(prefix);
    append(turnAxiom(root->Ax));
    append_car('\n');
    char cpyChild[500];
    char cpyChild2[500];
    ListAr*crt=root->children;
    while(crt)
    {
        if(childPrefix){
        strcpy(cpyChild,childPrefix);
        strcpy(cpyChild2,cpyChild);
        }
        else{
            cpyChild[0]='\0';
            cpyChild2[0]='\0';
        }
        if(crt->next){
            strcat(cpyChild,"|-- ");
            strcat(cpyChild2,"|  ");
            PutArbInMassStr(crt->Elem,cpyChild,cpyChild2);
        }else{
            
            strcat(cpyChild,"+-- ");
            strcat(cpyChild2,"    ");
            PutArbInMassStr(crt->Elem,cpyChild,cpyChild2);
        }
        crt=crt->next;
    }

}
void printblancs(int num)
{
	int ind;
	for ( ind = 0; ind < num; ind++)printf(" ");	
}
void V2_print_tree(AxiomAr*root,int ide)
{
    printblancs(ide);
    printf("--%s\n",turnAxiom(root->Ax));
    ListAr*crt=root->children;
    while(crt){
        V2_print_tree(crt->Elem,ide+2);
        crt=crt->next;
    }
}


void PrintMyAxiomAr(AxiomAr*Ar)
{
    init_Mass();
    PutArbInMassStr(Ar,NULL,NULL);
    printf("%s",MassiveString);
}