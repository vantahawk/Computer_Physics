#include <stdio.h>
#include <math.h>

double dFunktion(double x)  /*Auszuwertende Funktion mit x als Eingabe*/
{
    /*x+=1;*/       /*Verschiebe x zum Testvergleich mit ausgegebener Nullstelle...*/
    return (x*exp(-(x*x)));    /*Funktionsausgabe, hier: Beispielfunktion x*exp(-x^2)*/
}

int main()  /*Fkt.name, -paramter und -Return ggfs. anpassen!!!*/
{
    int k=0;    /*Grad fuer a prori Fehlerabschaetzung*/
    double a, a0, b, b0, z, RelFehl, Produkt; /*a0! = b0! = 0 !!!*/
    a=a0=-1;  /*Intervallstart und -ende initialisieren!!!*/
    b=b0=2;
    RelFehl=1e-4;   /*relativer Fehler !a priori!*/

    do
    {
        z=(a+b)/2;
        Produkt=dFunktion(a)*dFunktion(z);  /*Schachtelungsprodukt*/
        if (Produkt==0)     /*Nullstelle direkt getroffen*/
            break;
        if (Produkt<0)      /*Schrankenverkleinerung Richtung a*/
            b=z;
        if (Produkt>0)      /*Schrankenverkleinerung Richtung b*/
            a=z;
        ++k;
    }
    while (fabs((b0-a0)/pow(2,(k+1)))>=RelFehl);    /*Iterationsbedingung bzgl. Fehlerabschaetzung*/

    printf("Nullstelle von f(x) in [a0,b0]:\n\nX = %lf\n",z); /*Ausgabe von Nullstelle*/

    return 0;
}
