/*Allgemeines-Poisson-Loesungsverfahren*/
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

double BewFkt(double x, double y)   /*Bewegungsfkt. nach Poissongleichung*/
{
    /*return (2*(x*(x-1.5)+y*(y-1)));*/ /*fuer Testfkt. aus Aufg. 7b*/
    return (-2);    /*Bewfkt. fuer Aufg. 7c*/
}

/*I.A. Dirichlet-Randwerte der ges. Funktion, Werte muessen an Eckpunkten eindeutig sein!*/
double DirRand(double x, double y, double Ix, double Iy)
{
    /*I.A. mit bis zu 4 Fallunterscheidungen*/
    /*return (x*(x-1.5)*y*(y-1));*/   /*fuer Testfkt. in Aufg. 7b*/
    if (y==0)   /*fuer Aufg. 7d*/
        return 0;
        /*return (sin(2*PI*x/3));*/ /*fuer Aufg. 7c*/
    if (y==Iy)  /*fuer Aufg. 7d*/
        return 1;
    /*else return 0;*/  /*fuer Aufg. 7b*/
}

/*I.A. Neumann-Randwerte der ges. Funktion, Werte muessen an Eckpunkten eindeutig sein!*/
double NeuRand(double x, double y, double Ix, double Iy)
{
    /*I.A. mit bis zu 4 Fallunterscheidungen*/
    return 0;
}

int main()
{
    double Ix=1.5, Iy=1, h=0.25;    /*Ix & Iy muessen Vielfaches von h sein!!! - bei Aufg. 7d: keine Loesung bei h<=0.05 oder failt, ausserdem Loesung uneindeutig bei Aufg. 7d*/
    double Mx=Ix/h, My=Iy/h;
    double x, y;

    int nx=Mx, ny=My;
    int n=(nx+1)*(ny+1);

    int *indx, **Nummer, **Rand;
    double **a, d, *b;
    indx=ivector(1,n);
    a=matrix(1,n,1,n);
    b=vector(1,n);
    Nummer=imatrix(0,nx,0,ny);  /*Matrix, auf der die Nummerierung der Gitterpunkte eingetragen wird*/
    Rand=imatrix(0,nx,0,ny);    /*Matrix, auf der die Randbedingungen markiert werden*/
    /*Lauf-Variablen*/
    int i, j, k, p=0;

    /*Nummerierungs-Matrix & Nullsetzung der Randpunkt-Matrix*/
    for (i=0; i<=nx; i++)
    {
        for (j=0; j<=ny; j++)
        {
            ++p;
            Nummer[i][j]=p;
            Rand[i][j]=0;
        }
    }

    /*Eintragung der Randbedingungs-Marker, spezifisch fuer Randbed.!!!, hier fuer Randbed. in Aufg. 7d*/
    /*Codierung:
    0: frei, 1: Dirichlet,
    2: Neumann links, 3: Neumann rechts, 4: Neumann unten, 5: Neumann oben,
    6: Neumann links-unten, 7: Neumann links-oben, 8: Neumann rechts-unten, 9: Neumann rechts-oben*/
    for (i=0; i<=nx; i++)
    {
        Rand[i][0]=1;
        Rand[i][ny]=1;
    }
    for (i=1; i<ny; i++)
    {
        /*fuer Aufg. 7b & 7c*/
        /*Rand[0][i]=1;
        Rand[nx][i]=1;*/
        /*fuer Aufg. 7d*/
        Rand[0][i]=2;
        Rand[nx][i]=3;
    }

    /*Nullsetzung der LGS-Matrix*/
    for (i=1; i<=n; i++)
    {
        for (j=1; j<=n; j++)
        {
            a[i][j]=0;
        }
    }

    /*Eintragung der Koeffizienten und Werte*/
    p=0;
    for (i=0; i<=nx; i++)
    {
        for (j=0; j<=ny; j++)
        {
            ++p;
            x=i*h;
            y=j*h;
            /*Dirichlet-Punkt*/
            if (Rand[i][j]==1)
            {
                a[p][Nummer[i][j]]=1;
                b[p]=DirRand(x,y,Ix,Iy);
            }
            else
            {
                a[p][Nummer[i][j]]=4;
                b[p]=-h*h*BewFkt(x,y);
                /*Freier Punkt*/
                if (Rand[i][j]==0)
                {
                    a[p][Nummer[i-1][j]]=-1;
                    a[p][Nummer[i+1][j]]=-1;
                    a[p][Nummer[i][j-1]]=-1;
                    a[p][Nummer[i][j+1]]=-1;
                    if (Rand[i-1][j]==1)
                    {
                        a[p][Nummer[i-1][j]]=0;
                        b[p]+=DirRand((x-h),y,Ix,Iy);
                    }
                    if (Rand[i+1][j]==1)
                    {
                        a[p][Nummer[i+1][j]]=0;
                        b[p]+=DirRand((x+h),y,Ix,Iy);
                    }
                    if (Rand[i][j-1]==1)
                    {
                        a[p][Nummer[i][j-1]]=0;
                        b[p]+=DirRand(x,(y-h),Ix,Iy);
                    }
                    if (Rand[i][j+1]==1)
                    {
                        a[p][Nummer[i][j+1]]=0;
                        b[p]+=DirRand(x,(y+h),Ix,Iy);
                    }
                }
                /*4 Gleichungen fuer Neumann-RB fuer jeweils links, rechts, oben und unten*/
                /*Fuer jede Neumann-Gl. 3 entsprechende Abzugs-Bed.en fuer Dirichlet-RBen*/
                if (Rand[i][j]>=2)
                {
                    b[p]+=2*h*NeuRand(x,y,Ix,Iy);
                    /*Neumann-Punkt links*/
                    if (Rand[i][j]==2)
                    {
                        a[p][Nummer[i+1][j]]=-2;
                        a[p][Nummer[i][j-1]]=-1;
                        a[p][Nummer[i][j+1]]=-1;
                        if (Rand[i+1][j]==1)
                        {
                            a[p][Nummer[i+1][j]]=0;
                            b[p]+=2*DirRand((x+h),y,Ix,Iy);
                        }
                        if (Rand[i][j-1]==1)
                        {
                            a[p][Nummer[i][j-1]]=0;
                            b[p]+=DirRand(x,(y-h),Ix,Iy);
                        }
                        if (Rand[i][j+1]==1)
                        {
                            a[p][Nummer[i][j+1]]=0;
                            b[p]+=DirRand(x,(y+h),Ix,Iy);
                        }
                    }
                    /*Neumann-Punkt rechts*/
                    if (Rand[i][j]==3)
                    {
                        a[p][Nummer[i-1][j]]=-2;
                        a[p][Nummer[i][j-1]]=-1;
                        a[p][Nummer[i][j+1]]=-1;
                        if (Rand[i-1][j]==1)
                        {
                            a[p][Nummer[i-1][j]]=0;
                            b[p]+=2*DirRand((x-h),y,Ix,Iy);
                        }
                        if (Rand[i][j-1]==1)
                        {
                            a[p][Nummer[i][j-1]]=0;
                            b[p]+=DirRand(x,(y-h),Ix,Iy);
                        }
                        if (Rand[i][j+1]==1)
                        {
                            a[p][Nummer[i][j+1]]=0;
                            b[p]+=DirRand(x,(y+h),Ix,Iy);
                        }
                    }
                    /*Neumann-Punkt unten*/
                    if (Rand[i][j]==4)
                    {
                        a[p][Nummer[i-1][j]]=-1;
                        a[p][Nummer[i+1][j]]=-1;
                        a[p][Nummer[i][j+1]]=-2;
                        if (Rand[i-1][j]==1)
                        {
                            a[p][Nummer[i-1][j]]=0;
                            b[p]+=DirRand((x-h),y,Ix,Iy);
                        }
                        if (Rand[i+1][j]==1)
                        {
                            a[p][Nummer[i+1][j]]=0;
                            b[p]+=DirRand((x+h),y,Ix,Iy);
                        }
                        if (Rand[i][j+1]==1)
                        {
                            a[p][Nummer[i][j+1]]=0;
                            b[p]+=2*DirRand(x,(y+h),Ix,Iy);
                        }
                    }
                    /*Neumann-Punkt oben*/
                    if (Rand[i][j]==5)
                    {
                        a[p][Nummer[i-1][j]]=-1;
                        a[p][Nummer[i+1][j]]=-1;
                        a[p][Nummer[i][j-1]]=-2;
                        if (Rand[i-1][j]==1)
                        {
                            a[p][Nummer[i-1][j]]=0;
                            b[p]+=DirRand((x-h),y,Ix,Iy);
                        }
                        if (Rand[i+1][j]==1)
                        {
                            a[p][Nummer[i+1][j]]=0;
                            b[p]+=DirRand((x+h),y,Ix,Iy);
                        }
                        if (Rand[i][j-1]==1)
                        {
                            a[p][Nummer[i][j-1]]=0;
                            b[p]+=2*DirRand(x,(y-h),Ix,Iy);
                        }
                    }
                }
                /*4 Gleichungen fuer Neumann-RB fuer jeden der 4 Eckpunkte*/
                /*Fuer jede Neumann-Gl. 2 entsprechende Abzugs-Bed.en fuer Dirichlet-RBen*/
                if (Rand[i][j]>=6)
                {
                    b[p]+=4*h*NeuRand(x,y,Ix,Iy);
                    /*Neumann-Punkt links-unten*/
                    if(Rand[i][j]==6)
                    {
                        a[p][Nummer[i+1][j]]=-2;
                        a[p][Nummer[i][j+1]]=-2;
                        if (Rand[i+1][j]==1)
                        {
                            a[p][Nummer[i+1][j]]=0;
                            b[p]+=2*DirRand((x+h),y,Ix,Iy);
                        }
                        if (Rand[i][j+1]==1)
                        {
                            a[p][Nummer[i][j+1]]=0;
                            b[p]+=2*DirRand(x,(y+h),Ix,Iy);
                        }
                    }
                    /*Neumann-Punkt links-oben*/
                    if (Rand[i][j]==7)
                    {
                        a[p][Nummer[i+1][j]]=-2;
                        a[p][Nummer[i][j-1]]=-2;
                        if (Rand[i+1][j]==1)
                        {
                            a[p][Nummer[i+1][j]]=0;
                            b[p]+=2*DirRand((x+h),y,Ix,Iy);
                        }
                        if (Rand[i][j-1]==1)
                        {
                            a[p][Nummer[i][j-1]]=0;
                            b[p]+=2*DirRand(x,(y-h),Ix,Iy);
                        }
                    }
                    /*Neumann-Punkt rechts-unten*/
                    if (Rand[i][j]==8)
                    {
                        a[p][Nummer[i-1][j]]=-2;
                        a[p][Nummer[i][j+1]]=-2;
                        if (Rand[i-1][j]==1)
                        {
                            a[p][Nummer[i-1][j]]=0;
                            b[p]+=2*DirRand((x-h),y,Ix,Iy);
                        }
                        if (Rand[i][j+1]==1)
                        {
                            a[p][Nummer[i][j+1]]=0;
                            b[p]+=2*DirRand(x,(y+h),Ix,Iy);
                        }
                    }
                    /*Neumann-Punkt rechts-oben*/
                    if (Rand[i][j]==9)
                    {
                        a[p][Nummer[i-1][j]]=-2;
                        a[p][Nummer[i][j-1]]=-2;
                        if (Rand[i-1][j]==1)
                        {
                            a[p][Nummer[i-1][j]]=0;
                            b[p]+=2*DirRand((x-h),y,Ix,Iy);
                        }
                        if (Rand[i][j-1]==1)
                        {
                            a[p][Nummer[i][j-1]]=0;
                            b[p]+=2*DirRand(x,(y-h),Ix,Iy);
                        }
                    }
                }
            }
        }
    }

    /*LGS-Loesung*/
    ludcmp(a,n,indx,&d);
    lubksb(a,n,indx,b);
    /*Rest-Skelett*/
    free_ivector(indx,1,n);
    free_matrix(a,1,n,1,n);
    free_vector(b,1,n);

    /*Ausgabe-Teil*/
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
            if (Rand[i][j]==1)
            {
                printf("%lf\t%lf\t%lf\n", x, y, DirRand(x,y,Ix,Iy));
                /*fprintf(fp,"%lf\t%lf\t%lf\t%lf\n", x, y, b[k], DirRand(x,y,Ix,Iy));*/
                fprintf(fp_num,"%lf\t%lf\t%lf\n", x, y, DirRand(x,y,Ix,Iy));
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
