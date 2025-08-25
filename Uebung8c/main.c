/*Richardson-Verfahren fuer Aufg. 8c*/
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

double DirFkt(int i, double t)
{
    if (i==0)
        return (sin(PI*t));
    else return 0;
    /*return (sin(PI*t));*/
}

double NeuFkt() /*i.A. mit int i, int j, double h als Parameter*/
{
    return 0;
}

int main()
{
    int i, j;   /*Laufvariablen: ggfs. i fuer x und j fuer x; bei Matrizen i und j als Indizes*/
    const int n=50, m=100, q=1;    /*x-Weite; t-Weite bis 1; t-Periodenzahl*/
    double n2=n, m2=m;
    double h=1/n2;
    double k=1/m2;   /*Es muss gelten: k<=h^2/2 !!!*/
    double **I, **J, **Z, *c, *v, *z;
    int *indx;
    double **a, d, *b, *c2;
    double x, t;
    I=matrix(0,n,0,n);
    J=matrix(0,n,0,n);
    Z=matrix(0,n,0,n);
    c=vector(0,n);
    v=vector(0,n);
    z=vector(0,n);

    indx=ivector(0,n);
    a=matrix(0,n,0,n);
    b=vector(0,n);
    c2=vector(0,n);

    v=Vek(0,0,n);       /*Setze Anfangsvektor (hier 0)*/
    c=Vek(0,0,n);       /*Setze Zusatzvektor auf 0*/
    c2=Vek(0,0,n);       /*Setze Zusatzvektor 2 auf 0*/

    I=Mat(0,0,n);       /*Setze Einheitsmatrix I auf 0*/
    I=Dia(I,1,0,n,0,0); /*Fuelle Einheitsmatrix I diagonal auf*/

    J=Mat(0,0,n);       /*Setze Hilfsmatrix J auf 0*/
    J=Dia(J,2,0,n,0,0); /*Fuelle Hilfsmatrix J diagonal auf*/
    J=Dia(J,-1,0,n-1,1,0);
    J=Dia(J,-1,0,n-1,0,1);
    J[n][n-1]=-2;       /*Eckwert fuer Neumann-RB*/

    for (i=0; i<=n; i++)    /*Berechne Zwischenmatrix a*/
    {
        for (j=0; j<=n; j++)
            a[i][j]=2*I[i][j]+k*J[i][j]/(h*h);
            Z[i][j]=2*I[i][j]-k*J[i][j]/(h*h);
    }

    /*### LU-Zerlegung*/
    ludcmp(a,n,indx,&d);

    printf("x\t\tt\t\tu(x,t)\n\n");
    FILE *fp;
    fp=fopen("ausgabe.dat","w");
    for (j=1; j<=m*q; j++)    /*Berechne naechsten Vektor v*/
    {
        t=j*k;
        c[0]=DirFkt(0,t);
        c[n]=2*h*NeuFkt();
        /*c[n]=DirFkt(1,t);*/
        c2[0]=DirFkt(0,t+k);
        c2[n]=2*h*NeuFkt();
        /*2[n]=DirFkt(1,t+k);*/
        z=MVP(Z,v,0,n);

        for (i=0; i<=n; i++)
        {
            b[i]=z[i]+k*(c[i]+c2[i])/(h*h);
        }
        lubksb(a,n,indx,b);     /*### Loesung des LGS, gibt neues b als Loesungsvektor aus*/
        v=b;
        for (i=0; i<=n; i++)
        {
            x=i*h;
            printf("%lf\t%lf\t%lf\n",x,t,v[i]);
            fprintf(fp,"%lf\t%lf\t%lf\n",x,t,v[i]);
        }
    }
    free_ivector(indx,0,n); /*### Restteil des Skeletts*/
    free_matrix(a,0,n,0,n);
    free_vector(b,0,n);

    fclose(fp);
    return 0;
}
