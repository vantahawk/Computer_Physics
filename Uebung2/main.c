/*Newton-Verfahren*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int n=4;
double x[100], t[100][100];     /*Da Dimension nicht frei waehlbar bei Feldern, uebermaessig viele als hinreichender Vorrat*/

double dFunktion(double y)
{
    return (1/(1+(25*(y*y))));  /*Funktion variabel*/
}


double dPolynom(double y)
{
    int k;
    double p;
    p=t[0][n]*(y-x[n-1])+t[0][n-1];
    for (k=n-1; k>=1; k--)
        p=p*(y-x[k-1])+t[0][k-1];
    return p;
}


/*
double dPolynom(double y)
{
    int j,k;
    double p=A[0],q=1;

    for (j=1; j<=n; j++)
    {
        for (k=0; k<=j-1; k++)
            q*=(y-x[k]);
        p+=A[j]*q;
    }

    return p;
}
*/

int main()
{
    int i, j;
    double a=-1, b=1;
    double h, X, F, P;
    h=(b-a)/n;

    for (i=0; i<=n; i++)        /*Stuetzstellen und -werte berechnen und einfuegen*/
    {
        x[i]=a+i*h;             /*Stuetzstellen-Verteilungsfunktion*/
        t[i][i]=dFunktion(x[i]);
    }

    for (i=1; i<=n; i++)
    {
        for (j=0; j<=(n-i); j++)
            t[j][j+i]=(t[j][j+i-1]-t[j+1][j+i])/(x[j]-x[j+i]);
    }

    /*for (i=0; i<=n; i++)
        A[i]=t[0][i];*/


    printf("x-Values:\t");
    for (i=0; i<=n; i++)
        printf("%lf\t",x[i]);
        printf("\ny-Values:\t");
    for (i=0; i<=n; i++)
        printf("%lf\t",t[i][i]);
    printf("\nCoefficients:\t");
    for (i=0; i<=n; i++)
        printf("%lf\t",t[0][i]);

    printf("\n\nEnter X: ");
    scanf("%lf",&X);        /*Eingabe immer mit Punkt statt Komma!!!*/
    F=dFunktion(X);
    P=dPolynom(X);
    printf("f(x)=%lf\np(X)=%lf\nD(X)=|f(x)-p(X)|=%lf\n",F,P,fabs(F-P));

    /*
    printf("\nEnter a character to quit!\n");
    scanf("%d",&m);
    */
    return 0;
}
