#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/*Praeprozessor-Block*/
/*Muss mindestens fuer Pick-Vektor includet werden*/
#define NRANSI
#define float double
#include "nrutil.c"
/*#include "ludcmp.c"
#include "lubksb.c"*/
#undef float

/*Hier: Korr.Dim. am Bsp. der  logistische Gleichung*/
int main()  /*Fkt.name, -paramter und -Return ggfs. anpassen!!!*/
{
/*Aufg 9e: Korrelationsdimension*/
    r=4;    /*Setze Grenzen, seed und Parameter*/
    n=10000;
    m=1000;
    x=0.5;
    int k=0, q=25, nR=100, t=m/q;   /*Sprung-Schrittzahl q, nR: Schrittzahl fuer R, Pick-Grenze t: m muss Vielfaches von q sein!!!*/
    double R, C, S=0, Rmax=0.5, nR2=nR, hR=Rmax/nR2; /*R, C, S: Vorsumme fuer C, Rmax: max. Wert fuer R - hier 0.5, hR: Schrittweite fuer R*/
    double *Pick;           /*Vektor mit in Spruengen von q herausgepickten x-Werten*/
    double Rlog, Clog;      /*Logarithmen von r & C*/
    /*double Sk=0;*/        /*Vorsumme fuer Korr.dim.*/
    FILE *fp2, *fp2_log;/*Ausgabe-Datei sowie fuer logarithmische Werte: R gegen C(R)*/
    Pick=vector(1,t);

    for (i=1; i<=n; i++)/*Vor-Entw. der logistischen Gl.*/
            x=r*x*(1-x);/*Folge ggfs. anpassen!*/
    for (i=1; i<=m; i++)/*Zusatzlauf*/
        {
            x=r*x*(1-x);
            if (i%q==0) /*Pick-Bedingung*/
            {
                ++k;
                Pick[k]=x;
            }
        }

    /*Eigentliche Herleitung der Korr.Dim.*/
    fp2=fopen("ausgabe2.dat","w");
    fp2_log=fopen("ausgabe2-log.dat","w");
    for (k=1; k<=nR; k++)   /*Laufe von 0 bis Rmax*/
    {
        R=k*hR;
        for (i=1; i<=t; i++)/*Ziehe Element-Paare aus Pick-Vektor*/
        {
            for (j=1; j<=t; j++)
            {
                if (i!=j)   /*unteschiedliche Indizes!*/
                {
                    if (fabs(Pick[i]-Pick[j])<=R)   /*Inkrementiere S falls Korr.dim.bed. erfuellt*/
                        ++S;
                }
            }
        }
        C=S/(t*(t-1));  /*Berechne C aus S*/
        Rlog=log(R);    /*Berechne logarithmische Werte*/
        Clog=log(C);
        /*Sk+=fabs(Clog/Rlog);*/  /*Inkrementiere Vorsumme fuer Korr.dim.*/
        fprintf(fp2,"%lf\t%lf\n",R,C);          /*Schreibe in Datei 2*/
        fprintf(fp2_log,"%lf\t%lf\n",Rlog,Clog);/*Schreibe in Datei 2_logarithmisch*/
    }
    /*printf("Korrelationsdimension d_k = %lf\n",(Sk/nR));*/   /*Gebe berechnete Kor.dim. d_k aus*/
    fclose(fp2);
    fclose(fp2_log);
    return 0;
}
