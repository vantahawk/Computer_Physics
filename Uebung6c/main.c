/*Numerov-Verfahren fuer Photon an einfacher Potentialstufe*/
#include <stdio.h>
#include <math.h>

const double PI=3.141592654;

double Potential(double x)  /*hier einfache Potentialstufe*/
{
    if (x<0)
        return 0;
    else return -1;
}

double AnalytRe(double x, double E, double V)   /*Realteil der analyt. Vergleichsfkt.*/
{
    double A, B, C=1;
    double q=sqrt(2*E), k=sqrt(2*(E-V));
    A=(sqrt(E/(E-V))-1)/2;
    B=1-A;
    if (x>0)
        return ((A+B)*cos(k*x));
    else return (C*cos(q*x));
}

double AnalytIm(double x, double E, double V)   /*Imaginaerteil der analyt. Vergleichsfkt.*/
{
    double A, B, C=1;
    double q=sqrt(2*E), k=sqrt(2*(E-V));
    A=(sqrt(E/(E-V))-1)/2;
    B=1-A;
    if (x>0)
        return ((B-A)*sin(k*x));
    else return (-C*sin(q*x));
}

int main()
{
    int j, N=1000;          /*Schrittzahl*/
    double E=0.5, V=-1;     /*Gesamtenergie & Potential*/
    double x, M=N;          /*x-Achsenwert*/
    double x0=-10, xEnde=10;/*Start- und End-x-Wert*/
    double dx=fabs((xEnde-x0)/M);   /*Schrittweite*/
    double ReB, ReC, ImB, ImC, ReA, ImA, gA, gB, gC, Re, Im;
    double q=sqrt(2*E);     /*"freie Wellenzahl"*/
    ReC=cos(q*x0);          /*Anfangs-Wellenfunktionswerte*/
    ReB=cos(q*(x0+dx));
    ImC=-sin(q*x0);
    ImB=-sin(q*(x0+dx));
    /*double ReB=Re1, ReC=Re0, ImB=Im1, ImC=Im0;*/

    FILE *fp;
    fp=fopen("ausgabe.dat","w");    /*Schreibe parallel in Datei*/
    printf("x\t\tRe[Y(x)]\tIm[Y(x)]\tRe'\t\tIm'\n\n");  /*Tabellenkopf*/

    for (j=0; j<=N; j++)    /*Numerov-Verfahren*/
    {
        x=x0+j*dx;  /*x-Wert*/
        gA=2*(E-Potential(x+dx));   /*g-Fkt.werte*/
        gB=2*(E-Potential(x));
        gC=2*(E-Potential(x-dx));

        ReA=(ReB*(2-5*dx*dx*gB/6)-ReC*(1+dx*dx*gC/12))/(1+dx*dx*gA/12); /*Berechnung von Re- & Im-Teil der num. Wfkt.*/
        ImA=(ImB*(2-5*dx*dx*gB/6)-ImC*(1+dx*dx*gC/12))/(1+dx*dx*gA/12);

        Re=AnalytRe(x,E,V); /*Berechnung von Re- & Im-Teil der analyt. Wfkt.*/
        Im=AnalytIm(x,E,V);
        printf("%lf\t%lf\t%lf\t%lf\t%lf\n",x,ReA,ImA,Re,Im);    /*Ausgabe auf Konsole & Datei*/
        fprintf(fp,"%lf\t%lf\t%lf\t%lf\t%lf\n",x,ReA,ImA,Re,Im);
        ReC=ReB;    /*Anpassung der num. Wfkt.werte*/
        ReB=ReA;
        ImC=ImB;
        ImB=ImA;
    }

    fclose(fp);
    return 0;
}
