#include<stdio.h>
#include"lexical_scanner.c"
#include"syntax_1pass.c"
#include"syntax.c"
int main()
{
    int choix;
    char*ls;
    AxiomAr*Ar=NULL;
    printf(" \n 1-analyse lexical. ");
    printf("\n 2-analyse syntax 2 pass. ");
    printf("\n 3-analyse lexical. \n");
    printf("\n other- exit. \n");
    do{
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
            Ar=System_global(ls);
            printf("\narbre genere:\n");
            PrintMyAxiomAr(Ar);
            break;
        case 3: 
            Ar=System_global_1pass("test.txt");
            printf("\narbre genere:\n");
            PrintMyAxiomAr(Ar);
            break;
        default:
            break;
        }
    }while(choix!=0);




   

  
   
    
    return 0;
}