/*Newton-Verfahren 2
n-Auswertung bei festem X*/

#include <stdio.h>
#include <math.h>

int n;
double x[100], t[100][100];     /*Da Dimension nicht frei waehlbar bei Feldern, uebermaessig viele als hinreichender Vorrat, hier: 100*/

double dFunktion(double y)
{
    return (1/(1+(25*(y*y))));  /*Funktion variabel*/
}

double dPolynom(double y)       /*Berechnung des Polynoms nach Horner-Schema*/
{
    int k;
    double p;
    p=t[0][n]*(y-x[n-1])+t[0][n-1]; /*Beachte Vorzeichen und Dekremierung!*/
    for (k=n-1; k>=1; k--)
        p=p*(y-x[k-1])+t[0][k-1];
    return p;
}

int main()
{
    int i, j;
    double a=-1, b=1;
    double h, X, F, P;
    const double PI=3.141592654;
    X=PI/4;     /*x-Wert zum auswerten direkt initialisieren (im Gegensatz zu Eingabe bei v1), hier: X=pi/4*/
    /*0.7853981634*/
    printf("n\tf(X)\t\tp(X)\t\tD(X)=|f(X)-p(X)|\n\n");

    for (n=5; n<=20; n+=5)
    {
        h=(b-a)/n;

        for (i=0; i<=n; i++)        /*Stuetzstellen und -werte berechnen und einfuegen*/
        {
            /*x[i]=a+i*h;*/             /*Stuetzstellen-Verteilungsfunktion - aequidistant*/
            x[i]=cos(((2*i+1)*PI/(n+1))/2); /*speziell => hohe Konvergenz*/
            t[i][i]=dFunktion(x[i]);
        }

        for (i=1; i<=n; i++)
        {
            for (j=0; j<=(n-i); j++)
                t[j][j+i]=(t[j][j+i-1]-t[j+1][j+i])/(x[j]-x[j+i]);
        }

        F=dFunktion(X);
        P=dPolynom(X);
        printf("%d\t%lf\t%lf\t%lf\n",n,F,P,fabs(F-P));
    }

    /*for (i=0; i<=n; i++)
        A[i]=t[0][i];*/

    /*
    printf("\nEnter a character to quit!\n");
    scanf("%d",&m);
    */
    return 0;
}
