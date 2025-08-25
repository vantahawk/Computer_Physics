#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/*Praeprozessor-Block*/
#define NRANSI
#define float double
#include "nrutil.c"
/*#include "ludcmp.c"
#include "lubksb.c"*/
#undef float

int main()
{
    const double PI=3.141592654;
    double z=1;
    int i, j, P=1000;   /*Laufvariablen, Entw.grenze*/
    double *c;  /*Sammelvektor fuer Koeffizienten*/
    double Arg=sqrt(PI)*z/2;    /*"Argument"*/
    c=vector(0,P);
    c[0]=1; /*Startwerte fuer Koeff. c*/
    c[1]=1;
    double Out=c[0]*Arg+c[1]*pow(Arg,3)/3;    /*Startwert fuer Output*/
    for (i=2; i<=P; i++)    /*Anullierung restlicher Komponenten*/
        c[i]=0;
    for (i=2; i<=P; i++)    /*Fuer volle Reihe siehe Wikipedia: "error function"*/
    {
        for (j=0; j<i; j++) /*Berechnung der Koeff.*/
        {
            c[i]+=c[j]*c[(i-1)-j]/((j+1)*(2*j+1));
        }
        Out+=c[i]*pow(Arg,(2*i+1))/(2*i+1);
    }
    printf("Out = %lf",Out);
    return 0;
}
