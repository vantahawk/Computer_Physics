/*Numerische Integration
hier: Summierte Simpson-Regel*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double dFunktion(double x, double alpha)    /*Auszuwertende Funktion, entsprechend einzusetzen, Parameter ggfs. anpassen*/
{
    return (1/sqrt(1-pow((x*sin(alpha)),2)));
}

double dStelle(int j, double A, double H) /*Stuetzstellen-Verteilungsfunktion, hier: aequidistante Stuetzstellen*/
{
    return (A+j*H);
}

int main()  /*Fkt.name, -paramter und -Return ggfs. anpassen!!!*/
{
    const double PI=3.141592654;/*Konstante Pi*/
    double REL=10e-6;           /*Relative Abweichung - Genauigkeit der Integration*/
    int N=2;      /*Startwert: Stuetzstellen-Ordnung*/
    double a=0, b=PI/2;         /*Integralgrenzen*/
    double I, I2, h, X, Amp=PI/2;   /*X: Spezieller Integranden-Parameter bei Uebung 3, ggfs. weglassen. Amplitude: hier direkt einzusetzen. Bei Uebung 3 fuer versch. Werte durchlaufen!*/
    int i;          /*Schritt-Variablen*/
    double c=PI/2; /*Maximalwert der auszuwertenden Amplituden*/

    h=(b-a)/N;      /*Schrittweite fuer aequidistante Stuetzstellen*/
    /*printf("c=%lf, n=%d\n\nk\tAmp=k*Pi/2\tK(sin(Amp/2))\n\n",c,n);*/   /*Tabelle*/
    X=sin(Amp/2);   /*Einzusetzender Integranden-Parameter X*/

    I=(b-a)*(dFunktion(X,a)+4*dFunktion(X,((a+b)/2))+dFunktion(X,b))/(3*N); /*Startintegral*/
    do {
        I2=I;       /*Vergleichswert-Uebernahme*/
        I=0;        /*Zuruecksetzung von Integral*/
        N*=2;       /*N-Verdopplung*/
        for (i=1; i<=N/2; i++)  /*nur N/2, da in Doppelschritten iteriert wird!*/
            I+=h*(dFunktion(X,dStelle((2*i-2),a,h))+4*dFunktion(X,dStelle((2*i-1),a,h))+dFunktion(X,dStelle((2*i),a,h)))/3;    /*Simpson-Summanden*/
        }
    while (fabs((I-I2)/I2)>=REL);       /*Abgleich der relativen Änderung mit REL, Integration wird solange mit wachsendem N wiederholt, bis REL erreicht*/

    /*printf("%d\t%lf\t%lf\n",k,Amp,I);*/   /*Ausgabe: Amplituden-Teil, Amplitude, K(sin(Amp/2))*/

    return 0;
}

