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
const long M=1e9+7, a=1009, b=1;    /*Parameter fuer Zufallsprozess*/
const int d=3, P=100;  /*Raumdimension, Dimension des Sammelvektors fuer ErfInv-Koeffizienten*/

double ErfInv(double z, double *c) /*mit z aus [-1,1]!!!*/
{
    int i, j;   /*Laufvariablen, Entw.grenze*/
    double Arg=sqrt(PI)*z/2;    /*"Argument"*/
    c[0]=1; /*Startwerte fuer Koeff. c*/
    c[1]=1;
    double Out=c[0]*Arg+c[1]*pow(Arg,3)/3;    /*Startwert fuer Output*/
    for (i=2; i<=P; i++)    /*Anullierung restlicher Komponenten*/
        c[i]=0;
    for (i=2; i<=P; i++)    /*Fuer volle Reihe siehe Wikipedia: "error function"*/
    {
        for (j=0; j<i; j++) /*Berechnung der Koeff.*/
        {
            c[i]+=c[j]*c[(i-1)-j]/((j+1)*(2*j+1));
        }
        Out+=c[i]*pow(Arg,(2*i+1))/(2*i+1);
    }
    return Out;
    /*return (sqrt(PI)*(z+(PI*pow(z,3)/12)+(7*PI*PI*pow(z,5)/480)+(127*pow(PI,3)*pow(z,7)/40320)+(4369*pow(PI,4)*pow(z,9)/5806080)+(34807*pow(PI,5)*pow(z,11)/182476800))/2);*/
}

double Betrag(double *v)
{
    int i;
    double B=0;
    for (i=1; i<=d; i++)
        B+=v[i]*v[i];
    return sqrt(B);
}

double Abstand(double *a, double *b)
{
    int i;
    for (i=1; i<=d; i++)
        a[i]=a[i]-b[i];
    return Betrag(a);
}

double Zufall(double q)
{
    long Q;
    Q=q;    /*Falls Uebernahme von double in long nicht funktioniert, andere Konversions-Loesung finden*/
    Q=(a*Q+b)%M;
    q=Q;
    return q;
}

int main()
{
    int i, j, k, t, N=1000, tEnd=1e5, nF=1, nZ=1;   /*Laufvariablen, Defektzahl, Zeitgrenze, Teilchensummen*/
    double m=M, q=17, limit=1e9;    /*Zufallsperiodizitaet, Seed, Limit...*/
    double L=100, s=30;             /*Kantenlaenge, Standardabweichung der z-Verteilung*/
    double RF, z_F=100, r_F=0.001282, E_F=0.1;  /*Driftrate, Driftkonstante, Energie*/
    double RZ, z_Z=105, r_Z=1.717, E_Z=1.37;
    double kB=8.617343e-5;          /*Boltzmann-Konstante*/
    double B, Sprung=2.35, Abst=4;  /*Betrag (Zwischenvariable), Sprungweite, Rekombinationsabstand*/
    double T=600;   /*Temperatur*/
    double **F, **Z, *Jump, *Pre, *vF, *vZ, *c; /*Defekt-Orts-Matrizen, Sprungvektor, Vorvektor fuer Sprung, Vergleichsvektoren, Sammelvektor fuer ErfInv-Koeffizienten*/
    double f, z;    /*Zwischenvariablen fuer z-Verteilung*/
    FILE *fp;       /*Ausgabe-Datei*/
    F=matrix(1,N,1,d);
    Z=matrix(1,N,1,d);
    Jump=vector(1,d);
    Pre=vector(1,d);
    vF=vector(1,d);
    vZ=vector(1,d);
    c=vector(0,P);

    /*Startverteilung der Defekte*/
    printf("Startverteilung\n");
    /*x- & y-Startwerte*/
    printf("\tx- & y-Startwerte\n");
    for (i=1; i<=N; i++)
    {
        F[i][1]=L*Zufall(q)/m;
        q=Zufall(q);
        F[i][2]=L*Zufall(q)/m;
        q=Zufall(q);
        Z[i][1]=L*Zufall(q)/m;
        q=Zufall(q);
        Z[i][2]=L*Zufall(q)/m;
        q=Zufall(q);
    }
    /*z-Startwerte entspr. Normalverteilung*/
    printf("\tz-Startwerte\n\t\tFehlstellen\n");
    i=1;
    while (i<=N)
    {
        f=z_F+s*sqrt(2)*ErfInv((2*Zufall(q)/m-1),c);
        q=Zufall(q);
        if (f>0)
        {
            if (f<-limit)
                f=-limit;
            if (f>limit)
                f=limit;
            F[i][d]=f;
            ++i;
        }
    }
    printf("\t\tZwischengitteratome\n\n");
    i=1;
    while (i<=N)
    {
        z=z_Z+s*sqrt(2)*ErfInv((2*Zufall(q)/m-1),c);
        q=Zufall(q);
        if (z>0)
        {
            if (z<-limit)
                z=-limit;
            if (z>limit)
                z=limit;
            Z[i][d]=z;
            ++i;
        }
    }

    /*Hauptzyklus*/
    RF=r_F*exp(-E_F/(kB*T));
    RZ=r_Z*exp(-E_Z/(kB*T));
    fp=fopen("ausgabe.dat","w");
    printf("Hauptzyklus\n\nt\tn_F\tn_Z\n\n");
    t=1;
    while (((nF>=0) && (nZ>=0)) && (t<=tEnd))
    /*for (t=1; t<=tEnd; t++)*/
    {
        nF=0;
        nZ=0;
        /*Sprungphase*/
        for (i=1; i<=N; i++)
        {
            /*Fehlstellen*/
            if (F[i][d]>0)
            {
                if (Zufall(q)/m<=RF)
                {
                    q=Zufall(q);
                    for (j=1; j<=d; j++)
                    {
                        Pre[j]=Zufall(q)/m;
                        q=Zufall(q);
                    }
                    B=Betrag(Pre);
                    for (j=1; j<=d; j++)
                        Jump[j]=Sprung*Pre[j]/B;
                    for (j=1; j<=d; j++)
                        F[i][j]+=Jump[j];
                    for (j=1; j<d; j++)
                    {
                        if (F[i][j]<0)
                            F[i][j]+=L;
                        if (F[i][j]>L)
                            F[i][j]-=L;
                    }
                }
                q=Zufall(q);
            }
            /*Zwischengitteratome*/
            if (Z[i][d]>0)
            {
                if (Zufall(q)/m<=RZ)
                {
                    q=Zufall(q);
                    for (j=1; j<=d; j++)
                    {
                        Pre[j]=Zufall(q)/m;
                        q=Zufall(q);
                    }
                    B=Betrag(Pre);
                    for (j=1; j<=d; j++)
                        Jump[j]=Sprung*Pre[j]/B;
                    for (j=1; j<=d; j++)
                        Z[i][j]+=Jump[j];
                    for (j=1; j<d; j++)
                    {
                        if (Z[i][j]<0)
                            Z[i][j]+=L;
                        if (Z[i][j]>L)
                            Z[i][j]-=L;
                    }
                }
                q=Zufall(q);
            }
        }

        /*Rekombinationsphase*/
        for (i=1; i<=N; i++)
        {
            if (F[i][d]>0)
            {
                for (j=1; j<=N; j++)
                {
                    if (Z[i][d]>0)
                    {
                        for (k=1; k<=d; k++)
                        {
                            vF[i]=F[i][k];
                            vZ[j]=Z[j][k];
                        }
                        if ((Abstand(vF,vZ)<Abst))
                        {
                            F[i][d]=-1;
                            Z[j][d]=-1;
                        }
                    }
                }
            }
        }

        /*Defektzaehlung*/
        for (i=1; i<=N; i++)
        {
            if (F[i][d]>0)
                ++nF;
            if (Z[i][d]>0)
                ++nZ;
        }

        /*Ausgabe*/
        printf("%d\t%d\t%d\n",t,nF,nZ);
        fprintf(fp,"%d\t%d\t%d\n",t,nF,nZ);
        ++t;
    }
    printf("\nFertig!\n");
    fclose(fp);
    return 0;
}
