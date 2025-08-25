/*Alle Kommentare mit '###' sind Teil des Skeletts Aufg. 7 & 8*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/*Praeprozessor-Block*/
#define NRANSI
#define float double
#include "nrutil.c"
#include "ludcmp.c"
#include "lubksb.c"
#undef float

int main()
{
    double Ix=..., Iy=..., h=...;    /*Ix & Iy muessen Vielfaches von h sein!!!*/
    double Mx=Ix/h, My=Iy/h;
    /*Achsenparameter & n*/
    int nx=Mx, ny=My;
    int n=(nx-1)*(ny-1);    /*### Setze n*/
    int i, j, k;
    /*### Initialisierung wichtigster Felder, etc.*/
    int *indx;
    double **a, d, *b;
    indx=ivector(1,n);
    a=matrix(1,n,1,n);
    b=vector(1,n);

    /*Nullsetzung der LGS-Matrix*/
    for (i=1; i<=n; i++)
    {
        for (j=1; j<=n; j++)
        {
            a[i][j]=0;
        }
    }

    /*### Bildung von LGS-Matrix a und Ergebnis-Vektor b*/
    for (i=1; i<=(nx-1); i++)
    {
        for (j=1; j<=(ny-1); j++)
        {
            /*Hochspezifisch je nach Anwendung*/
            a[i][j]=...
            b[k]=...
        }
    }

    /*### Loesungsfunktionen fuer LGS*/
    ludcmp(a,n,indx,&d);
    lubksb(a,n,indx,b);
    /*Danach kann der Loesungsvektor v (NEUES b[]) verwendet werden*/

    /*### Restteil des Skeletts*/
    free_ivector(indx,1,n);
    free_matrix(a,1,n,1,n);
    free_vector(b,1,n);

    return 0;
}

