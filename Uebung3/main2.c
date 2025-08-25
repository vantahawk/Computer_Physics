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
/* AS: Das e steht fuer 10^, 10e-6 ist deshalb 10*10^-6, also 10^-5.
    double REL=10e-6;
*/
    double REL=1.0e-6;   /*Relative Abweichung*/
    int N=2;            /*Startwert: Stuetzstellen-Ordnung*/
/* AS: pi/2 ist eine eigene Konstante und muss nicht berechnet werden.
    double a=0.0, b=M_PI/2.0;
*/
    double a=0.0, b=M_PI_2;   /*Integralgrenzen*/
    double I, I2, h, X, Amp;
    int i, k;           /*Schritt-Variablen*/
/* AS: pi/2 ist eine eigene Konstante und muss nicht berechnet werden.
    double c=M_PI/2.0;
*/
    double c=M_PI_2;  /*Maximalwert der auszuwertenden Amplituden*/
    int n=9;           /*Anzahl der Auswertungen. Ab n>10 faengt Programm an relativ aufwendig zu werden! Ab n>14 faengt es an untragbar lange zu laufen!*/

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
/* AS: Der hier verwendete Algorithmus fuehrt zwar zum richtigen Ergebnis, ist aber
   so geschrieben, dass die Berechnung der Funktionswerte des Integranden an den
   Stellen a+2*h, a+4*h, etc., jeweils zweimal erfolgt, naemlich in zwei aufeinander
   folgenden Schleifendurchlaeufen. In diesem Fall spielt es zwar keine Rolle, aber
   wenn die Funktionsauswertung zeitaufwaendig ist, brauchen Sie so unnoetigerweise
   50% mehr Rechenzeit. Eine bessere Effizienz erzielt man, wenn dFunktion an jeder
   Stuetzstelle nur einmal aufgerufen wird und die dabei erhaltenen Stuetzwerte mit
   den passenden Gewichten 1,4,2,4,2,...,4,2,4,1 aufaddiert werden. */
                I+=h*(dFunktion(X,dStelle((2*i-2),a,h))+4.0*dFunktion(X,dStelle((2*i-1),a,h))+dFunktion(X,dStelle((2*i),a,h)))/3.0;    /*Simpson-Summanden*/
            }
        while (fabs((I-I2)/I2)>=REL);       /*Abgleich der relativen Änderung mit REL*/

        printf("%d\t%lf\t%lf\n",k,Amp,I);   /*Ausgabe: Amplituden-Teil, Amplitude, K(sin(Amp/2))*/
    }

    return 0;
}
/* AS: Und wie lang muss der Faden des Pendels nun gewaehlt werden? */
