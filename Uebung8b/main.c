/*Richardson-Verfahren fuer Aufg. 8b*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/*Praeprozessor-Block*/
#define NRANSI
#define float double
#include "nrutil.c"
/*#include "ludcmp.c"
#include "lubksb.c"*/
#undef float

const double PI=3.141592654;

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

double DirFkt(int i, double t)  /*Dirichlet-RB*/
{
    if (i==0)
        return (sin(PI*t));
    else return 0;
    /*return (sin(PI*t));*/
}

double NeuFkt() /*Neumann-RB: i.A. mit int i, int j, double h als Parameter*/
{
    return 0;
}

int main()
{
    int i, j;   /*Laufvariablen: ggfs. i fuer x und j fuer x; bei Matrizen i und j als Indizes*/
    const int n=20, m=1000, q=1;    /*x-Weite; t-Weite bis 1; t-Periodenzahl*/
    double n2=n, m2=m;
    double h=1/n2;
    double k=1/m2;   /*Es muss gelten: k<=h^2/2 !!!*/
    double **I, **J, **Z, *c, *v, *z;
    double x, t;
    I=matrix(0,n,0,n);
    J=matrix(0,n,0,n);
    Z=matrix(0,n,0,n);
    c=vector(0,n);
    v=vector(0,n);
    z=vector(0,n);

    v=Vek(0,0,n);       /*Setze Anfangsvektor (hier 0)*/
    c=Vek(0,0,n);       /*Setze Zusatzvektor auf 0*/

    I=Mat(0,0,n);       /*Setze Einheitsmatrix I auf 0*/
    I=Dia(I,1,0,n,0,0); /*Fuelle Einheitsmatrix I diagonal auf*/

    J=Mat(0,0,n);       /*Setze Hilfsmatrix J auf 0*/
    J=Dia(J,2,0,n,0,0); /*Fuelle Hilfsmatrix J diagonal auf*/
    J=Dia(J,-1,0,n-1,1,0);
    J=Dia(J,-1,0,n-1,0,1);
    J[n][n-1]=-2;       /*Eckwert fuer Neumann-RB*/

    for (i=0; i<=n; i++)    /*Berechne Zwischenmatrix Z*/
    {
        for (j=0; j<=n; j++)
            Z[i][j]=I[i][j]-k*J[i][j]/(h*h);
    }

    printf("x\t\tt\t\tu(x,t)\n\n");
    FILE *fp;
    fp=fopen("ausgabe.dat","w");
    for (j=1; j<=m*q; j++)    /*Berechne naechsten Vektor v*/
    {
        t=j*k;
        c[0]=DirFkt(0,t);
        c[n]=2*h*NeuFkt();
        /*c[n]=DirFkt(1,t);*/
        z=MVP(Z,v,0,n);
        for (i=0; i<=n; i++)
        {
            x=i*h;
            v[i]=z[i]+k*c[i]/(h*h);
            printf("%lf\t%lf\t%lf\n",x,t,v[i]);
            fprintf(fp,"%lf\t%lf\t%lf\n",x,t,v[i]);
        }
    }
    fclose(fp);
    return 0;
}

