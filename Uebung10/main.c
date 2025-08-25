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

const double h=0.001;         /*Zeitschrittweite*/
const double s=10, b=8/3, r=28;/*Parameter*/

double K1(double x, double y, double z, int j)
{
    if (j==1)
        return (s*(y-x));
    if (j==2)
        return ((r-z)*x-y);
    /*if (j==3)*/
    else return (x*y-b*z);
}

double K2(double x, double y, double z, int j)
{
    if (j==1)
        return K1((x+h*K1(x,y,z,1)/2),y,z,1);
    if (j==2)
        return K1(x,(y+h*K1(x,y,z,2)/2),z,2);
    /*if (j==3)*/
    else return K1(x,y,(z+h*K1(x,y,z,3)/2),3);
}

double K3(double x, double y, double z, int j)
{
    if (j==1)
        return K1((x+h*K2(x,y,z,1)/2),y,z,1);
    if (j==2)
        return K1(x,(y+h*K2(x,y,z,2)/2),z,2);
    /*if (j==3)*/
    else return K1(x,y,(z+h*K2(x,y,z,3)/2),3);
}

double K4(double x, double y, double z, int j)
{
    if (j==1)
        return K1((x+h*K3(x,y,z,1)),y,z,1);
    if (j==2)
        return K1(x,(y+h*K3(x,y,z,2)),z,2);
    /*if (j==3)*/
    else return K1(x,y,(z+h*K3(x,y,z,3)),3);
}

double Betrag(double x, double y, double z)
{
    return sqrt(x*x+y*y+z*z);
}

int main()
{
    double x, y, z;             /*Trajektorienwerte*/
    double x0=20, y0=20, z0=20; /*Startwerte*/
    double X, Y, Z, dr=0.01;    /*Vergleichs-Endwerte, Startwert-Verschiebung, Lyapunov-Exponent*/
    int i, n=1e6, m=5e4;        /*Laufvariable, Erstgrenze Zweitgrenze*/
    x=x0;   /*Startwert-Zuweisung*/
    y=y0;
    z=z0;
    FILE *fp1;/*Ausgabe-Datei*/
    fp1=fopen("ausgabe1.dat","w");
    /*Erstlauf*/
    for (i=1; i<=n; i++)
    {
        x+=h*(K1(x,y,z,1)+2*K2(x,y,z,1)+2*K3(x,y,z,1)+K4(x,y,z,1))/6;
        y+=h*(K1(x,y,z,2)+2*K2(x,y,z,2)+2*K3(x,y,z,2)+K4(x,y,z,2))/6;
        z+=h*(K1(x,y,z,3)+2*K2(x,y,z,3)+2*K3(x,y,z,3)+K4(x,y,z,3))/6;
    }
    /*Kopie der ersten Endwerte*/
    X=x;
    Y=y;
    Z=z;
    /*Neue Startwerte*/
    x=x0+dr;
    y=y0+dr;
    z=z0+dr;

    /*Zweitlauf*/
    for (i=1; i<=n; i++)
    {
        x+=h*(K1(x,y,z,1)+2*K2(x,y,z,1)+2*K3(x,y,z,1)+K4(x,y,z,1))/6;
        y+=h*(K1(x,y,z,2)+2*K2(x,y,z,2)+2*K3(x,y,z,2)+K4(x,y,z,2))/6;
        z+=h*(K1(x,y,z,3)+2*K2(x,y,z,3)+2*K3(x,y,z,3)+K4(x,y,z,3))/6;
    }
    /*Berechne und gebe Lya-Exp. aus*/
    printf("1. Lyapunov-Exponent Ly = %lf\n\n",(log(Betrag((x-X),(y-Y),(z-Z)))/(h*n)));

    /*Zusatzlauf*/
    int k=0;         /*Laufvarable fuer Sprungschritt*/
    int q=500, t=m/q;/*Sprungschritt, Pick-Grenze - m muss ganzes Vielfaches von q sein!!!*/
    double **Pick;   /*Pick-Matrix*/
    Pick=matrix(1,t,1,3);
    for (i=1; i<=m; i++)
    {
        x+=h*(K1(x,y,z,1)+2*K2(x,y,z,1)+2*K3(x,y,z,1)+K4(x,y,z,1))/6;
        y+=h*(K1(x,y,z,2)+2*K2(x,y,z,2)+2*K3(x,y,z,2)+K4(x,y,z,2))/6;
        z+=h*(K1(x,y,z,3)+2*K2(x,y,z,3)+2*K3(x,y,z,3)+K4(x,y,z,3))/6;
        fprintf(fp1,"%lf\t%lf\t%lf\n",x,y,z);
        if (i%q==0) /*Fuelle Pick-Vektor in Spruengen q auf*/
        {
            ++k;
            Pick[k][1]=x;
            Pick[k][2]=y;
            Pick[k][3]=z;
        }
    }
    fclose(fp1);

    /*Korrelationsfkt. & -dim.*/
    int j, nR=100; /*R-Schrittzahl*/
    double R, C, S=0, Rmin=0.6, Rmax=3, nR2=nR, hR=(Rmax-Rmin)/nR2;   /*R, C, C-Vorsumme, R-Unter- & Obergrenze, R-Schrittweite*/
    double Rlog, Clog, Sk;  /*Logarithmen von r & C, Vorsumme fuer Korr.dim.*/;
    FILE *fp2, *fp2_log;    /*Zweite Ausgabe-Datei sowie fuer logarithmische Werte*/
    fp2=fopen("ausgabe2.dat","w");
    fp2_log=fopen("ausgabe2-log.dat","w");
    for (k=1; k<=nR; k++)   /*Laufe von Rmin bis Rmax*/
    {
        R=Rmin+hR*k;
        for (i=1; i<=t; i++)/*Ziehe Element-Paare aus Pick-Vektor*/
        {
            for (j=1; j<=t; j++)
            {
                if (i!=j)   /*unteschiedliche Indizes!*/
                {
                    if (Betrag((Pick[i][1]-Pick[j][1]),(Pick[i][2]-Pick[j][2]),(Pick[i][3]-Pick[j][3]))<=R)   /*Inkrementiere S falls Korr.dim.bed. erfuellt*/
                        ++S;
                }
            }
        }
        C=S/(t*(t-1));  /*Berechne C aus S*/
        Rlog=log(R);    /*Berechne logarithmische Werte*/
        Clog=log(C);
        Sk+=fabs(Clog/Rlog);  /*Inkrementiere Vorsumme fuer Korr.dim.*/
        fprintf(fp2,"%lf\t%lf\n",R,C);          /*Schreibe in Datei 2*/
        fprintf(fp2_log,"%lf\t%lf\n",Rlog,Clog);/*Schreibe in Datei 2_logarithmisch*/
    }
    printf("Korrelationsdimension d_k = %lf\n\n",(Sk/nR));   /*Gebe berechnete Kor.dim. d_k aus*/
    fclose(fp2);
    fclose(fp2_log);
    return 0;
}
