#include <stdio.h>
#include <stdlib.h>
/*Praeprozessor-Block*/
#define NRANSI
#define float double
#include "nrutil.c"
/*#include "ludcmp.c"
#include "lubksb.c"*/
#undef float

double *Vek(double d, int m, int n)     /*Setzt beliebigen Vektor auf Wert d*/
{
    int i;
    double *vec;
    vec=vector(m,n);
    for (i=m; i<=n; i++)
        vec[i]=d;
    return vec;
}

double **Mat(double d, int m, int n)   /*Setzt beliebige Matrix auf Wert d*/
{
    int i, j;
    double **mat;
    mat=matrix(m,n,m,n);
    for (i=m; i<=n; i++)
    {
        for (j=m; j<=n; j++)
            mat[i][j]=d;
    }
    return mat;
}

/*Fuellt beliebige Matrix diagonal auf mit Wert d mit Zeilen-Offset iO und Spalten Offset jO;
Um Diagonale zu Matrix a hinzuzufuegen, muss a in sich selbst eingesetzt werden: a=Dia(a,d,m,n,iO,jO);
auf Grenzen m, n und Offsets achten!!!*/
double **Dia(double **mat, double d, int m, int n, int iO, int jO)
{
    int i;
    for (i=m; i<=n; i++)
        mat[i+iO][i+jO]=d;
    return mat;
}

double *MVP(double **a, double *b, int m, int n)    /*Matrix-Vektor-Produkt*/
{
    int k, q;
    double *p;
    /*a=matrix(0,n,0,n);
    b=vector(0,n);*/
    p=vector(m,n);
    for (k=m; k<=n; k++)    /*Setze Ergebnisvektor auf 0*/
        p[k]=0;
    for (k=m; k<=n; k++)    /*Berechne Einzel-Produkt-Summen*/
    {
        for (q=m; q<=n; q++)
            p[k]+=a[k][q]*b[q];
    }
    return p;
}

int main()
{
    int i, j, n=4;
    double **a, *b, *c;
    a=matrix(0,n,0,n);
    b=vector(0,n);
    c=vector(0,n);

    a=Mat(0,0,n);
    a=Dia(a,2,0,n,0,0);
    a=Dia(a,3,0,n-1,1,0);
    a=Dia(a,1,0,n-1,0,1);
    b=Vek(2,0,n);

    for (i=0; i<=n; i++)    /*Gebe Vektor b aus*/
        printf("%lf\t",b[i]);
    printf("\n\n");
    for (i=0; i<=n; i++)    /*Gebe Matrix a aus*/
    {
        for (j=0; j<=n; j++)
            printf("%lf\t",a[i][j]);
        printf("\n");
    }
    printf("\n\n");          /*Gebe Vektor c aus*/
    c=MVP(a,b,0,n);
    for (i=0; i<=n; i++)
        printf("%lf\t",c[i]);
    return 0;
}
/*Noch weitere Felder-Module folgen: Vektor-Addierer (VAdd), Matrix-Addierer (MAdd),Skalar-Produkt (SP), Matrix-Matrix-Produkt (MMP),
Kreuzprodukt (KP) (nur fuer dim=3 (m=0, n=2)!), Transformierer (Trans)*, Determinante (zunaechst nur fuer dim=2 und dim=3)/
