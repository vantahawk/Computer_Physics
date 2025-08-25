/*Uebung 4 - Nullstellenbestimmung*/
#include <stdio.h>
#include <math.h>

double dBestFkt(double Z, double S, int j)  /*Bestimmungsfunktion*/
{
    /*if (j%2==0)
        return (1.0/tan(Z)+sqrt(S-Z*Z)/Z);
    else
        return (tan(Z)-sqrt(S-Z*Z)/Z);*/
    return (tan(Z)-sqrt(S-Z*Z)/Z);
    /*return (1.0/tan(Z)+sqrt(S-Z*Z)/Z);*/
}

int main()
{
    const double C=3.8099817;   /*Konstant, siehe (a)*/
    int i, j, r, k=0, n=6, o=25;/*k: Zaehlvariable, n: Ordnung d. Wellenfunktion bzw. Energieniveaus, o: Anzahl der Auswertungen fuer Potential*/
    double z, a, b, a0, b0, P, E, S;
    double V, V0=0.2, A=5, RelFehl=1.0e-6; /*V: Potential, V0: kleinstes Potential, A: halbe Topfbreite, RelFehl:  Grenze d. rel. Fehlers*/
    FILE *fp;
    fp=fopen("ausgabe.dat","w");

    printf("V");    /*Tabelle: Potential V (0,2-5,0eV), Ordnungen n*/
    for (j=1; j<=n; j++)
        printf("\t\tn=%d",j);
    printf("\n\n");

    for (r=1; r<=o; r++)
    {
        V=V0*r;     /*Variierung des Ptentials*/
        S=A*A*V/C;  /*entspricht xi*/
        printf("%lf",V);
        for (j=1; j<=n; j++)
        {
            a=a0=M_PI_2*(j-1);  /*Intervallgrenzen*/
            b=b0=a0+M_PI;
            do  /*Intervallschachtelung*/
            {
                z=(a+b)/2;
                P=dBestFkt(a,S,j)*dBestFkt(z,S,j);
                if (P==0)
                    break;
                if (P<0)
                    b=z;
                if (P>0)
                    a=z;
                ++k;
            }
            while (fabs((b0-a0)/pow(2.0,(k+1)))>=RelFehl);  /*a priori Fehlerbegrenzung*/
            E=C*z*z/(A*A);    /*Berechnung der Energie*/
            printf("\t%lf",E);/*Ausgabe*/
            fprintf(fp,"\t%lf",E);
        }
        printf("\n");
        fprintf(fp,"\n");
    }

    fclose(fp);
    return 0;
}
