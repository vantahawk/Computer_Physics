/*Uebung2 - Polynominterpolation*/
/*CANCELLED!!!*/

#include <stdio.h>
#include <math.h>

FILE *fp;   /*Zeiger fuer Ausgabedateien zu Aufg. d & e*/

double dFunktion(double y)
{
    return (1.0/(1.0+(25.0*(y*y))));  /*Funktion variabel*/
}

double dPolynom(int n, double y, double t[n+2][n+1])       /*Berechnung des Polynomwertes nach Horner-Schema*/
{
    int k;
    double p;
    p=t[0][n]*(y-t[n+1][n-1])+t[0][n-1];
    for (k=n-1; k>=1; k--)
        p=p*(y-t[n+1][k-1])+t[0][k-1];
    return p;
}

double dNewton(int n, int e, double a, double b)  /*Bestimmung von Koeffizienten aus Stuetzstellen*/
{
    int i, j;
    double t[n+2][n+1]; /*Stuetzstellen, Koeffizienten*/
    double h=(b-a)/n;
    for (i=0; i<=n; i++)        /*Stuetzstellen und -werte berechnen und einfuegen*/
    {
        if (e)
            t[n+1][i]=a+i*h;     /*Stuetzstellen-Verteilungsfunktion - aequidistant*/
        else t[n+1][i]=cos(((2*i+1)*M_PI/(n+1))/2); /*speziell => hohe Konvergenz*/
        t[i][i]=dFunktion(t[n+1][i]);
    }

    for (i=1; i<=n; i++)    /*Koeffizienten berechnen und einfuegen*/
    {
        for (j=0; j<=(n-i); j++)
            t[j][j+i]=(t[j][j+i-1]-t[j+1][j+i])/(t[n+1][j]-t[n+1][j+i]);
    }

    return t;
}

void vTabelle(int e, double a, double b)
{
    int n, i, j, k=0, m=100, start=5, end=20, step=5, count=(end-start)/step; /*(end-start) muss ganzes Vielfaches von step sein!*/
    double H, X, F, P, s;
    double c[count][end+1];/*Feld mit Koeffizienten fuer verschiedene n (Aufg. d & e)*/
    X=M_PI/4;         /*x-Wert zum Auswerten in c.*/

    /*Auswertung an X*/
    for (n=start; n<=end; n+=step)
    {
        double t[n+2][n+1];
        t=dNewton(n,e,a,b);
        F=dFunktion(X); /*Funktionswert*/
        P=dPolynomn(n,X,t);  /*Polynomwert*/
        printf("%d\t%lf\t%lf\t%lf\n",n,F,P,fabs(F-P));
        for (i=0; i<=n; i++)    /*Einsetzen der Koeffizienten fuer verschiedene n*/
            c[k][i]=t[0][i];
        ++k;
    }

    /*Ausgabedateien mit Funktionsplots*/
    H=(b-a)/m;
    for (j=0; j<=m; j++)
    {
        s=a+j*H;
        k=0;
        fprintf(fp,"\n%lf\t%lf",s,dFunktion(s));
        for (n=start; n<=end; n+=step)
        {
            for (i=0; i<=n; i++)    /*Wiederabrufen der Koeffizienten fuer verschiedene n*/
                t[0][i]=c[k][i];
            fprintf(fp,"\t%lf",dPolynom(n,s,t));
            ++k;
        }
    }
}

int main()
{
    int i, n, e=1;
    double a=-1, b=1;

    /*Aufg. b*/
    n=4;
    printf("Aufg. b: Koeffizienten fuer n = 4:\n\n");
    double t[n+2][n+1];
    t=dNewton(n,e,a,b);

    printf("x-Werte:\t");
    for (i=0; i<=n; i++)
        printf("%lf\t",t[n+1][i]);
    printf("\ny-Werte:\t");
    for (i=0; i<=n; i++)
        printf("%lf\t",t[i][i]);
    printf("\nKoeffizienten:\t");
    for (i=0; i<=n; i++)
        printf("%lf\t",t[0][i]);

    /*Aufg. c & d*/
    printf("\n\nAufg. c: Auswertungen fuer x = pi/4 bei *aequidistanten* Stuetzstellen:\n\nn\tf(X)\t\tp(X)\t\t|f(X)-p(X)|\n\n");
    fp=fopen("ausgabe_d.dat","w");  /*Oeffne Datei fuer Funktionsplots fuer Aufg. d*/
    vTabelle(e,a,b);
    fclose(fp);

    /*Aufg. e*/
    e=0;
    printf("\n\nAufg. e: Auswertungen fuer x = pi/4 bei *speziellen* Stuetzstellen:\n\nn\tf(X)\t\tp(X)\t\t|f(X)-p(X)|\n\n");
    fp=fopen("ausgabe_e.dat","w");  /*Oeffne Datei fuer Funktionsplots fuer Aufg. e*/
    vTabelle(e,a,b);
    fclose(fp);

    /*In den zwei erzeugten Ausgabedateien befinden sich die Werte fuer den Plot der Funktion (x-Werte: Spalte 1, y-Werte: Spalte 2)
    sowie der entsprechenden Interpolationspolynome fuer die Grade n=5 (Spalte 3), 10 (4), 15 (5) und 20 (6).
    Anscheinend wird bei mir allerdings nur das Polynom fuer n=20 bei den speziellen Stuetzstellen (Aufg. e, Spalte 6) annaehernd numerisch stabil berechnet.*/

    return 0;
}
