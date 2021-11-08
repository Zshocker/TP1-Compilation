#include<stdio.h>
#include"lexical_scanner.c"
//#include"syntax_1pass.c"
#include"syntax.c"
int main()
{
    int choix;
    char*ls;
    AxiomAr*Ar=NULL;
    printf(" \n 1-analyse lexical. ");
    printf("\n 2-analyse syntax 2 pass. ");
    printf("\n 3-analyse syntax 1 pass. \n");
    printf("\n other- exit. \n");
    do{
        NUMLIGNE=1;
        printf("\nvotre choix:");
        scanf("%d",&choix);
        switch (choix)
        {
        case 1:
            ls=analyse_lexical("test.txt");
            printf("\nfichier genere:\n");
            print_fichier(ls);
        break;
        case 2: 
            ls=analyse_lexical("test.txt");
            printf("\nfichier genere:\n");
            print_fichier(ls);
            Mode=2;
            Ar=System_global(ls);
            printf("\narbre genere:\n");
            PrintMyAxiomAr(Ar);
            break;
        case 3: 
            Mode=1;
            Ar=System_global("test.txt");
            printf("\narbre genere:\n");
            PrintMyAxiomAr(Ar);
            break;
        default:
            break;
        }
    }while(choix!=0);
    return 0;
}