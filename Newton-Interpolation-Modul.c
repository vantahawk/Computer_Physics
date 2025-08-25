/*Newton-Verfahren
n-Auswertung bei festem X*/

#include <stdio.h>
#include <math.h>
/*Praeprozessor-Block*/
/*nrutil.c & nrutil.h includen, um variable Felder-Dimensionen zu verwenden! Ansonsten mit uebergrossen Felder Dimensionen arbeiten (z.B. 100)!*/
#define NRANSI
#define float double
#include "nrutil.c"
/*#include "ludcmp.c"
#include "lubksb.c"*/
#undef float

double dFunktion(double y)
{
    return (1/(1+(25*(y*y))));  /*Funktion variabel*/
}

double dPolynom(double y, double *x, double **t, int n)       /*Berechenung des Polynoms nach Horner-Schema*/
{
    int k;
    double p;
    p=t[0][n]*(y-x[n-1])+t[0][n-1]; /*Beachte Vorzeichen und Dekremierung!*/
    for (k=n-1; k>=1; k--)
        p=p*(y-x[k-1])+t[0][k-1];
    return p;
}

int main()  /*Fkt.name, -paramter und -Return ggfs. anpassen!!!*/
{
    int i, j, n=4;      /*n direkt initialisieren*/
    double a=-1, b=1;   /*Integralgrenzen*/
    double h, X, F, P, *x, **t;
    const double PI=3.141592654;
    x=vector(0,n);
    t=matrix(0,n,0,n);
    X=PI/4;     /*x-Wert zum auswerten direkt initialisieren (im Gegensatz zu Eingabe bei v1), hier: X=pi/4*/
    /*0.7853981634*/
    h=(b-a)/n;  /*Schrittweite fuer aequidistante Stuetzstellen*/

    for (i=0; i<=n; i++)        /*Stuetzstellen und -werte berechnen und einfuegen*/
    {
        x[i]=a+i*h;             /*Stuetzstellen-Verteilungsfunktion - aequidistant*/
        /*x[i]=cos(((2*i+1)*PI/(n+1))/2);*/ /*speziell => hohe Konvergenz*/
        t[i][i]=dFunktion(x[i]);
    }

    for (i=1; i<=n; i++)    /*Berechnung der Koeffizienten*/
    {
        for (j=0; j<=(n-i); j++)
            t[j][j+i]=(t[j][j+i-1]-t[j+1][j+i])/(x[j]-x[j+i]);
    }
    /*Rufe Fkt. und Polynom auf*/
    F=dFunktion(X);
    P=dPolynom(X,x,t,n);
    /*Ausgabe der Stuetzstellen*/
    /*printf("Stuetzstellen:\n\n");
    for (i=0; i<=n; i++)
        printf("a_%d = %lf\n",i,x[i]);*/
    /*Ausgabe der Koeff.*/
    /*printf("\n\nKoeffizienten:\n\n");
    for (i=0; i<=n; i++)
        printf("a_%d = %lf\n",i,t[0][i]);*/

    /*printf("n\tf(X)\t\tp(X)\t\tD(X)=|f(X)-p(X)|\n\n");
    printf("%d\t%lf\t%lf\t%lf\n",n,F,P,fabs(F-P));*/  /*Ausgabe von Fkt. F, Polynom P und deren Abweichung in Abh. von n*/
    return 0;
}
