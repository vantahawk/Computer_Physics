/*Euler-Zahl-Approximation*/

#include <stdio.h>
#include <math.h>
#include <float.h>
#include <limits.h>

long j;

long lFakultaet(long n)
{
    long i;
    j=1;
    for(i=1; i<=n; i++)
        j*=i;

    return j;
}

/*
long lPotenz(double y, int n)
{
    int i;
    double y2=y;
    for(i=1; i<=n; i++)
        y*=y;

    return (y/y2);
}
*/

double dBetrag(double c)
{
    if (c<0)
        return -c;
    else return c;
}

int main()
{
    long N=1;
    double x, e, f, s, MG;
    /*double p;*/
    e=1;
    s=1;
    MG=1/1000;

    for(x=-40; x<=40; x+=10)
    {
        while(dBetrag(s)>=2*MG && j<=LONG_MAX/2);
            {
                /*p=lPotenz(x,N);*/
                f=lFakultaet(N);
                s=pow(x,N)/f;
                e+=s;
                ++N;
            }
        printf("%lf\t%lf\n",x,e);
    }

    return 0;
}
