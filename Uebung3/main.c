/*Uebung 3 - Numerische Integration*/

#include <stdio.h>
#include <math.h>

double dFunktion(double x, double alpha)    /*Auszuwertende Funktion*/
{
    return (1.0/sqrt(1.0-pow((x*sin(alpha)),2.0)));
}

double dStelle(int j, double A, double H) /*Stuetzstellen-Verteilungsfunktion*/
{
    return (A+j*H);
}

int main()
{
    double REL=1.0e-6;   /*Relative Abweichung*/
    int N=2;            /*Startwert: Stuetzstellen-Ordnung*/
    double a=0.0, b=M_PI_2;   /*Integralgrenzen*/
    double I, I2, h, X, Amp;
    int i, k;           /*Schritt-Variablen*/
    double c=M_PI_2;    /*Maximalwert der auszuwertenden Amplituden*/
    int n=10;           /*Anzahl der Auswertungen. Ab n>10 faengt Programm an relativ aufwendig zu werden! Ab n>14 faengt es an untragbar lange zu laufen!*/

    printf("c=%lf, n=%d\n\nk\tAmp=k*Pi/(2*n)\tK(sin(Amp/2))\n\n",c,n);   /*Tabelle*/

    for (k=1; k<=n; k++)    /*Auswertung für n verschiedene Amplituden*/
    {
        Amp=k*c/n;          /*Amplitude*/
        X=sin(Amp/2.0);     /*Einzusetzender Integranden-Parameter X*/

        I=(b-a)*(dFunktion(X,a)+4.0*dFunktion(X,((a+b)/2.0))+dFunktion(X,b))/(3.0*N); /*Startintegral*/
        do {
            I2=I;       /*Vergleichswert-Uebernahme*/
            I=0.0;      /*Zuruecksetzung von Integral*/
            N*=2;       /*N-Verdopplung*/
            h=(b-a)/N;  /*Schrittweite*/
            for (i=1; i<=N/2; i++)  /*nur N/2, da in Doppelschritten iteriert wird!*/
                I+=h*(dFunktion(X,dStelle((2*i-2),a,h))+4.0*dFunktion(X,dStelle((2*i-1),a,h))+dFunktion(X,dStelle((2*i),a,h)))/3.0;    /*Simpson-Summanden*/
            /*for (i=1; i<N/2; i++)
                I+=h*(2.0*dFunktion(X,dStelle((2*i),a,h))+4.0*dFunktion(X,dStelle((2*i+1),a,h)))/3.0;
            I+=h*(dFunktion(X,dStelle(0,a,h))+dFunktion(X,dStelle(1,a,h))+dFunktion(X,dStelle(N,a,h)))/3.0;*/
            }
        while (fabs((I-I2)/I2)>=REL);       /*Abgleich der relativen Änderung mit REL*/

        printf("%d\t%lf\t%lf\n",k,Amp,I);   /*Ausgabe: Amplituden-Teil, Amplitude, K(sin(Amp/2))*/
    }

    return 0;
}
