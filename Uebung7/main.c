/*Rein-Dirichlet-Poisson*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NRANSI
#define float double
#include "nrutil.c"
#include "ludcmp.c"
#include "lubksb.c"
#undef float

const double PI=3.141592654;

double BewFkt(int i, int j, double h)   /*Bewegungsfkt. nach Poissongleichung*/
{
    double x=i*h, y=j*h;
    /*return (2*(x*(x-1.5)+y*(y-1))); fuer Testfkt. aus Aufg. 7b*/
    return (-2);    /*Bewfkt. fuer Aufg. 7c*/
}

double DirRand(int i, int j, double h)  /*I.A. Dirichlet-Randwerte der ges. Funktion*/
{
    double x=i*h, y=j*h;
    /*return (x*(x-1.5)*y*(y-1)); fuer Testfkt. in Aufg. 7b*/
    if (y==0)
        return (sin(2*PI*x/3));
    else return 0;
}

int main()
{
    double Ix=1.5, Iy=1, h=0.05;    /*Ix & Iy muessen Vielfaches von h sein!!!*/
    double Mx=Ix/h, My=Iy/h;
    double x, y;

    int nx=Mx, ny=My;
    int n=(nx-1)*(ny-1);

    int *indx;
    double **a, d, *b, **Nummer;
    indx=ivector(1,n);
    a=matrix(1,n,1,n);
    b=vector(1,n);
    Nummer=matrix(0,nx,0,ny);  /*Matrix, auf der die Nummerierung der Gitterpunkte eingetragen wird*/
    /*Lauf-Variablen*/
    int i, j, k, p=0;
    /*Nummerierung-Matrix*/
    for (i=1; i<=(nx-1); i++)
    {
        for (j=1; j<=(ny-1); j++)
        {
            ++p;
            Nummer[i][j]=p;
        }
    }
    /*Nullsetzung der LGS-Matrix*/
    for (i=1; i<=n; i++)
    {
        for (j=1; j<=n; j++)
        {
            a[i][j]=0;
        }
    }

    p=0;
    /*Eintragung der Koeffizienten und Werte*/
    for (i=1; i<=(nx-1); i++)
    {
        for (j=1; j<=(ny-1); j++)
        {
            ++p;
            k=Nummer[i][j];
            a[p][k]=4;
            k=Nummer[i-1][j];
            a[p][k]=-1;
            k=Nummer[i+1][j];
            a[p][k]=-1;
            k=Nummer[i][j-1];
            a[p][k]=-1;
            k=Nummer[i][j+1];
            a[p][k]=-1;
            b[p]=-h*h*BewFkt(i,j,h);
            if ((i-1)==0)
            {
                k=Nummer[i-1][j];
                a[p][k]=0;
                b[p]+=DirRand((i-1),j,h);
            }
            if ((i+1)==nx)
            {
                k=Nummer[i+1][j];
                a[p][k]=0;
                b[p]+=DirRand((i+1),j,h);
            }
            if ((j-1)==0)
            {
                k=Nummer[i][j-1];
                a[p][k]=0;
                b[p]+=DirRand(i,(j-1),h);
            }
            if ((j+1)==ny)
            {
                k=Nummer[i][j+1];
                a[p][k]=0;
                b[p]+=DirRand(i,(j+1),h);
            }
        }
    }

    /*Vergleichsteil: Auffuellung von a & b*/
     /*for (i=1;i<=n;i++)
    {
        b[i]=rand();
        for (j=1;j<=n;j++)
        {
            a[i][j]=rand();
        }
    }*/

    ludcmp(a,n,indx,&d);
    lubksb(a,n,indx,b);

    free_ivector(indx,1,n);
    free_matrix(a,1,n,1,n);
    free_vector(b,1,n);

    /*FILE *fp*/
    FILE *fp_num;
    /*fp=fopen("ausgabe.dat","w");*/
    fp_num=fopen("ausgabe2.dat","w");
    printf("x\t\ty\t\tu_num\n\n");
    for (i=0; i<=nx; i++)
    {
        for (j=0; j<=ny; j++)
        {
            x=i*h;
            y=j*h;
            k=Nummer[i][j];
            if (((i==0) || (i==nx)) || ((j==0) || (j==ny)))
            {
                printf("%lf\t%lf\t%lf\n", x, y, DirRand(i,j,h));
                /*fprintf(fp,"%lf\t%lf\t%lf\t%lf\n", x, y, b[k], DirRand(i,j,h));*/
                fprintf(fp_num,"%lf\t%lf\t%lf\n", x, y, DirRand(i,j,h));
            }
            else
            {
                printf("%lf\t%lf\t%lf\n", x, y, b[k]);
                /*fprintf(fp,"%lf\t%lf\t%lf\t%lf\n", x, y, b[k], DirRand(i,j,h));*/
                fprintf(fp_num,"%lf\t%lf\t%lf\n", x, y, b[k]);
            }
        }
    }
    /*fclose(fp);*/
    fclose(fp_num);
    return 0;
}
