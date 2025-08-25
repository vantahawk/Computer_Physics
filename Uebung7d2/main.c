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

double BewFkt(int i, int j, double h)   /*Bewegungsfkt. nach Poissongleichung*/
{
    /*double x=i*h, y=j*h;*/
    /*return (2*(x*(x-1.5)+y*(y-1))); fuer Testfkt. aus Aufg. 7b*/
    return (-2);    /*Bewfkt. fuer Aufg. 7c*/
}

double DirRand(int i, int j, double h)  /*I.A. Dirichlet-Randwerte der ges. Funktion*/
{
    /*double x=i*h;*/
    double y=j*h;
    /*return (x*(x-1.5)*y*(y-1)); fuer Testfkt. in Aufg. 7b*/
    if (y==0)
        return 0;
    else return 1;
}

double NeuRand(int i, int j, double h)  /*I.A. Neumann-Randwerte der ges. Funktion*/
{
    /*double x=i*h, y=j*h;*/
    return 0;
}

int main()
{
    const double Ix=1.5, Iy=1, h=0.25;    /*Ix & Iy muessen Vielfaches von h sein!!!, hier: Ix, Iy, h fest wegen a priori LGS-Matrix!*/
    double Mx=Ix/h, My=Iy/h;
    /*Achsenparameter & n*/
    int nx=Mx, ny=My;
    int n=(nx+1)*(ny-1);    /*### Setze n*/
    int i, j, k, m, p;
    /*### Initialisierung wichtigster Felder, etc.*/
    int *indx;
    double **a, d, *b, x, y;
    indx=ivector(1,n);
    a=matrix(1,n,1,n);
    b=vector(1,n);

    double B[3][3]={{4,-1,0},{-1,4,-1},{0,-1,4}};
    double I[3][3]={{1,0,0},{0,1,0},{0,0,1}};
    /*Nullsetzung der LGS-Matrix*/
    for (i=1; i<=n; i++)
    {
        for (j=1; j<=n; j++)
        {
            a[i][j]=0;
        }
    }

    /*### Bildung von LGS-Matrix a*/
    for (i=1; i<=n; i+=3)
    {
        for (j=1; j<=n; j+=3)
        {
            if (i==j)
            {
                for (k=0; k<=2; k++)
                {
                    for (m=0; m<=2; m++)
                        a[i+k][j+m]=B[k][m];
                }
                if ((i==1) || (i==n-2))
                    for (k=0; k<=2; k++)
                    {
                        for (m=0; m<=2; m++)
                            a[i+k][j+m]=B[k][m]/2;
                    }
            }
            if ((i==j-3) || (i==j+3))
            {
                for (k=0; k<=2; k++)
                {
                    for (m=0; m<=2; m++)
                        a[i+k][j+m]=-I[k][m];
                }
            }
        }
    }

    /*### Bildug des Ergebnis-Vektors b*/
    p=0;
    for (i=1; i<=n; i+=3)
    {
        b[i]=-h*h*BewFkt(p,1,h)/2+DirRand(p,1,h)/2;
        b[i+1]=-h*h*BewFkt(p,2,h)/2;
        b[i+2]=-h*h*BewFkt(p,3,h)/2+DirRand(p,3,h)/2;
        ++p;
    }

    b[1]=-h*h*BewFkt(0,1,h)/2+h*NeuRand(0,1,h)+DirRand(0,0,h)/2;
    b[2]=-h*h*BewFkt(0,2,h)/2+h*NeuRand(0,2,h);
    b[3]=-h*h*BewFkt(0,3,h)/2+h*NeuRand(0,3,h)+DirRand(0,4,h)/2;

    b[n-2]=-h*h*BewFkt(p,1,h)/2+h*NeuRand(p,1,h)+DirRand(p,0,h)/2;
    b[n-1]=-h*h*BewFkt(p,2,h)/2+h*NeuRand(p,2,h);
    b[n]=-h*h*BewFkt(p,3,h)/2+h*NeuRand(p,3,h)+DirRand(p,4,h)/2;

    /*### Loesungsfunktionen fuer LGS*/
    ludcmp(a,n,indx,&d);
    lubksb(a,n,indx,b);
    /*Danach kann der Loesungsvektor v (NEUES b[]) verwendet werden*/

    /*### Restteil des Skeletts*/
    free_ivector(indx,1,n);
    free_matrix(a,1,n,1,n);
    free_vector(b,1,n);

    p=0;
    FILE *fp;
    fp=fopen("ausgabe.dat","w");
    printf("x\t\ty\t\tu(x,y)\n\n");
    for (i=0; i<=nx; i++)
    {
        for (j=0; j<=ny; j++)
        {
            ++p;
            x=i*h;
            y=j*h;
            if ((j==0) || (j==ny))
            {
                printf("%lf\t%lf\t%lf\n", x, y, DirRand(i,j,h));
                fprintf(fp,"%lf\t%lf\t%lf\n", x, y, DirRand(i,j,h));
            }
            else
            {
                printf("%lf\t%lf\t%lf\n", x, y, b[p]);
                fprintf(fp,"%lf\t%lf\t%lf\n", x, y, b[p]);
            }
        }
    }
    fclose(fp);

    return 0;
}
