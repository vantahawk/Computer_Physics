#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const double PI=3.141592654;
const double C=3.8099817;

double dFkt(double x, double a, double q, double k, double A, int n)  /*Fuer sowohl gerade als auch ungerade Paritaet*/
{
    if (fabs(x)<=a)
    {
        if (n%2==0)
            return (A*sin(n*q*x));
        else return (A*cos(n*q*x));
    }
    else
    {
        if (x<a)
        {
            if (n%2==0)
                return (-exp(k*x));
            else return (exp(k*x));
        }
        else return (exp(-k*x));

    }
}

double dBestfkt(double a, double V, int n)  /*Fuer sowohl gerade als auch ungerade Paritaet*/
{
    if (n%2==0)
        return (atan(-1/sqrt(V/C-1))/a);
    else return (atan(sqrt(V/C-1))/a);
}

int main()
{
    double a=5;
    /*double L=2*a;*/
    int i;
    int N=100, R=100;
    double h=a/N;
    double x, f, q, k, s;
    double V=20, A=1; /*Muss groesser als C sein!!!*/
    int n=5;

    q=dBestfkt(a,V,n);
    s=a*sqrt(V/C);
    k=sqrt(V/C-pow((n*q),2));
    FILE *fp;
    fp=fopen("ausgabe.dat","w");

    printf("a=%lf, n=%d, V=%lf, s=%lf\nq=%lf, k=%lf\nN=%d, R=%d\n\nx\t\tPsi(x)\n\n",a,n,V,s,q,k,N,R);
    for (i=-N-R; i<=N+R; i++)
    {
        x=i*h;
        f=dFkt(x,a,q,k,A,n);
        printf("%lf\t%lf\n",x,f);
        fprintf(fp,"%lf\t%lf\n",x,f);
    }
    fclose(fp);

    return 0;
}
