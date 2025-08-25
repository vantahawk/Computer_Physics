/*Numerov-Verfahren fuer Photon an allgemeiner/komplexer Potentialstufe*/
#include <stdio.h>
#include <math.h>

const double PI=3.141592654;

double Potential(double x)  /*s. Aufg. 6e*/
{
    if (x<-1)
        return 0;
    if (x>0)
        return -1;
    else return (2*pow(sin(PI*x),2)-pow((x*x-1),2));
}

double AnalytRe(double x, double a, double Pa, double k)    /*Realteil der analyt. Vergleichswfkt.*/
{
    return (a*cos(Pa-k*x));
}

double AnalytIm(double x, double b, double Pb, double k)    /*Imaginaerteil der analyt. Vergleichswfkt.*/
{
    return (b*sin(Pb-k*x));
}

int main()
{
    int j, N=1000;          /*Schrittzahl*/
    double E=0.5, V=-1;     /*Gesamtenergie & Potential*/
    double x, M=N;          /*x-Achsenwert*/
    double x0=-5, zEnde=5;  /*Start- und End-x-Wert*/
    double dx=fabs((zEnde-x0)/M);   /*Schrittweite*/
    double ReB, ReC, ImB, ImC, ReA, ImA, gA, gB, gC, Re, Im;
    double q=sqrt(2*E), k=sqrt(2*(E-V));    /*Wellenzahlen*/
    ReC=cos(q*x0);      /*Anfangs-Wfkt.werte*/
    ReB=cos(q*(x0+dx));
    ImC=-sin(q*x0);
    ImB=-sin(q*(x0+dx));
    /*double ReB=Re1, ReC=Re0, ImB=Im1, ImC=Im0;*/
    double a=1.57, b=0.48, Pa=0.17, Pb=0.87;    /*Parameter der analyt. Vergleichswfkt*/

    FILE *fp;
    fp=fopen("ausgabe.dat","w");
    printf("x\t\tRe[Y(x)]\tIm[Y(x)]\tRe'\t\tIm'\n\n");

    for (j=0; j<=N; j++)    /*Numerov-Verfahren*/
    {
        x=x0+j*dx;  /*x-Wert*/
        gA=2*(E-Potential(x+dx));   /*g-Fkt.werte*/
        gB=2*(E-Potential(x));
        gC=2*(E-Potential(x-dx));

        ReA=(ReB*(2-5*dx*dx*gB/6)-ReC*(1+dx*dx*gC/12))/(1+dx*dx*gA/12); /*Berechnung von Re- & Im-Teil der num. Wfkt.*/
        ImA=(ImB*(2-5*dx*dx*gB/6)-ImC*(1+dx*dx*gC/12))/(1+dx*dx*gA/12);

        Re=AnalytRe(x,a,Pa,k);  /*Berechnung von Re- & Im-Teil der analyt. Wfkt.*/
        Im=AnalytIm(x,b,Pb,k);
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
