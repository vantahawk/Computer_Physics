/*Uebung2 - Polynominterpolation*/

/* FS: Sorry, dein Programm konnte ich nicht mit ein paar Korrekturen zum Laufen bringen, da ist etwas mehr Arbeit noetig. Wenn du meinen Kommentaren folgst, sollte es aber machbar sein. */

#include <stdio.h>
#include <math.h>

int i, j, n, e=1;
double a=-1, b=1;
const double PI=3.141592654; /*FS: Wenn math.h eingebunden ist, kann man auch einfach M_PI verwenden */
double x[21], t[21][21]; /*FS: Das ist unsauber und ein ziemlicher Fehlermagnet, denn jedes Mal, wenn man die Anzahl der Stuetzstellen aendert, muss man an mehreren Stellen im Programm Werte aendern und wenn man nur einen einzigen vergisst, geht es in die Hose (so wie hier). Besser "const int n=20;" definieren und alle Arrays etc in Abhaengigkeit von n definieren, zB "double x[n+1];". Ausserdem gehoert sowas in die main-Funktion, nicht davor! */
/*double x[n+1], t[n+1][n+1];  FS: So sollte es eigentlich aussehen; klappt aber nicht, weil es vor dem main steht... */
FILE *fp;   /*Zeiger fuer Ausgabedateien zu Aufg. d & e*/

/*FS: Meistens vermeidet man es, Variablen ausserhalb einer Funktion (main ist auch eine Funktion) zu deklarieren und uebergibt lieber Variablen (oder bei Arrays Zeiger darauf). In kleineren Programmen mag es zwar auch so funktionieren, aber in groesseren Programmen mit vielen Hilfsfunktionen bekommt man schnell Probleme, zB wenn man einen Variablennamen versehentlich mehrfach verwendet oder eine Variable noch nicht definiert ist.*/

double dFunktion(double y)
{
    return (1/(1+(25*(y*y))));  /*Funktion variabel*/
    /*FS: Man sollte sicherheitshalber 1.0 statt 1 zu schreiben, wenn insgesamt ein Datentyp double rauskommen soll. Andernfalls kann (muss aber nicht) der C-Compiler die Datentypen falsch interpretieren und Schaetze wie 1/3=0 ausgeben...*/
}

double dPolynom(double y)       /*Berechnung des Polynomwertes nach Horner-Schema*/
{
    int k;
    double p;
    p=t[0][n]*(y-x[n-1])+t[0][n-1];
    for (k=n-1; k>=1; k--)
        p=p*(y-x[k-1])+t[0][k-1];
    return p;
}

void vNewton()  /*Bestimmung von Koeffizienten aus Stuetzstellen*/
{
    double h=(b-a)/n;
    for (i=0; i<=n; i++)    /*Stuetzstellen und -werte berechnen und einfuegen*/
    {
        if (e)
            x[i]=a+i*h;     /*Stuetzstellen-Verteilungsfunktion - aequidistant*/
        else x[i]=cos(((2*i+1)*PI/(n+1))/2); /*speziell => hohe Konvergenz*/
        t[i][i]=dFunktion(x[i]);
    }

    for (i=1; i<=n; i++)    /*Koeffizienten berechnen und einfuegen*/
    {
        for (j=0; j<=(n-i); j++)
            t[j][j+i]=(t[j][j+i-1]-t[j+1][j+i])/(x[j]-x[j+i]);
    }
}

void vTabelle()
{
    int k=0, m=100;
    double H, X, F, P, s;
    double c[4][21];/*Feld mit Koeffizienten fuer verschiedene n (Aufg. d & e)*/
    X=PI/4;         /*x-Wert zum Auswerten in c.*/

    /*Auswertung an X*/
    for (n=5; n<=20; n+=5)
    {
        vNewton();
        F=dFunktion(X); /*Funktionswert*/
        P=dPolynom(X);  /*Polynomwert*/
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
        for (n=5; n<=20; n+=5)
        {
            for (i=0; i<=n; i++)    /*Wiederabrufen der Koeffizienten fuer verschiedene n*/
                t[0][i]=c[k][i];
            fprintf(fp,"\t%lf",dPolynom(s));
            ++k;
        }
    }
}

int main()
{
    /*Aufg. b*/
    n=5;
    printf("Aufg. b: Werte und Koeffizienten fuer n = 4:\n\n");
    vNewton();

    printf("x-Werte:\t");
    for (i=0; i<=n; i++)
        printf("%lf\t",x[i]);
    printf("\ny-Werte:\t");
    for (i=0; i<=n; i++)
        printf("%lf\t",t[i][i]);
    printf("\nKoeffizienten:\t");
    for (i=0; i<=n; i++)
        printf("%lf\t",t[0][i]);

    /*Aufg. c & d*/
    printf("\n\nAufg. c: Auswertungen fuer x = pi/4 bei *aequidistanten* Stuetzstellen:\n\nn\tf(X)\t\tp(X)\t\t|f(X)-p(X)|\n\n");
    fp=fopen("ausgabe_d.dat","w");  /*Oeffne Datei fuer Funktionsplots fuer Aufg. d*/
    vTabelle();
    fclose(fp);

    /*Aufg. e*/
    e=0;
    printf("\n\nAufg. e: Auswertungen fuer x = pi/4 bei *speziellen* Stuetzstellen:\n\nn\tf(X)\t\tp(X)\t\t|f(X)-p(X)|\n\n");
    fp=fopen("ausgabe_e.dat","w");  /*Oeffne Datei fuer Funktionsplots fuer Aufg. e*/
    vTabelle();
    fclose(fp);

    /*In den zwei erzeugten Ausgabedateien befinden sich die Werte fuer den Plot der Funktion (x-Werte: Spalte 1, y-Werte: Spalte 2)
    sowie der entsprechenden Interpolationspolynome fuer die Grade n=5 (Spalte 3), 10 (4), 15 (5) und 20 (6).
    Anscheinend wird bei mir allerdings nur das Polynom fuer n=20 bei den speziellen Stuetzstellen (Aufg. e, Spalte 6) annaehernd numerisch stabil berechnet.*/
    
    /*FS: Ok, prinzipiell funktioniert dein Programm, aber es ist halt extrem unsauber geschrieben. Ich wuerde dir empfehlen, es noch einmal neu zu schreiben. Dieses Mal solltest du keine Variablen vor dem Main deklarieren sondern darin und stattdessen Variablen uebergeben. Dein Programm funktioniert deshalb nur fuer n=20, weil am Anfang x[21] statt x[n+1] deklariert wird (und zwischendrin in Hilfsfunktionen versteckt Felder a la "double c[4][21]" deklariert werden) und dadurch alles auf n=20 zugeschnitten ist. Wenn man jetzt die Anzahl der Stuetzstellen aendern will, muss man hoellisch aufpassen und macht trotzdem was falsch. Achte beim Programmieren immer darauf, dass man Parameter leicht aendern kann! (Also zB am Anfang der main-Funktion einen Block mit Konvergenzparametern einfuegen, die man dann NUR AN DIESER STELLE aendern muss).
     Wenn beim neu Programmieren alles richtig laeuft, solltest du beobachten, dass mit steigendem n der Bereich um x=0 besser beschrieben wird, es bei aequidistanten Stuetzstellen aber zu immer staerkeren Oszillationen am Rand kommt. Bei den speziellen Stuetzstellen in Teil e werden die Osziallationen unterdrueckt (wie man fuer n=20 schon sehen kann).*/

    return 0;
}
