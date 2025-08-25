#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/*Preprozessor-Block aus Skelett*/
#define NRANSI 
#define float double
#include "nrutil.c"
#include "ludcmp.c"
#include "lubksb.c"
#undef float

int main()
{
    /*Initialisierungen aus Skelett*/
    int i, j, m=0, k=0;
    const int n=5;
    int *indx;
    double **a, d, *b;
    indx=ivector(1,n);
    a=matrix(1,n,1,n);
    b=vector(1,n);
    /*Fuelle Matrix a auf*/
    for (i=1;i<=n;i++)
    {
        for (j=1;j<=n;j++)
        {
            a[i][j]=rand();
        }
    }
    /*Fuelle Vektor b auf*/
    for (i=1;i<=n;i++)
    {
        b[i]=rand();
    }
    /*Gebe Matrix a aus*/
    printf("a\n\n");
    for (i=1;i<=n;i++)
    {
        for (j=1;j<=n;j++)
            {printf("%lf\t",a[i][j]);}
        printf("\n");
    }
    /*Gebe Vektor b aus*/
    printf("\n\nb\n\n");
    for (i=1;i<=n;i++)
        printf("%lf\t",b[i]);
    /*LU-Zerlegung & LGS-Loesung*/
    ludcmp(a,n,indx,&d);
    lubksb(a,n,indx,b);
    /*Rest vonSkelett*/
    free_ivector(indx,1,n);
    free_matrix(a,1,n,1,n);
    free_vector(b,1,n);
    /*Gebe Loesungs-Vektor aus (neuer Vektor b)*/
    printf("\n\n\nv\n\n");
    for (i=1;i<=n;i++)
        printf("%lf\t",b[i]);
    printf("\n\n");

    return 0;
}
