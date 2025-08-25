#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const double PI=3.141592654;

double Add(double a, double b)
{
    double c;
    c=a+b;
    return c;
}

double Mult(double d[2])
{
    return (d[0]*d[1]);
}

double Betrag(double A[2])
{
    return (sqrt(A[0]*A[0]+A[1]*A[1]));
}

double BewFkt(int w, double r[2])
{
    return (-4*PI*PI*r[w]/pow(Betrag(r),3));
}

int main()
{
    int i;
    double N=10;
    double a[2]={0.5,0.6}, b[2]={0.7,0.8}, c[2], d[2]={3,4}, e;
    double g, h;
    c[0]=Add(a[0],b[0]);
    c[1]=Add(a[1],b[1]);
    printf("a\t\tb\t\tc\n\n");
    for (i=0; i<=1; i++)
        printf("%lf\t%lf\t%lf\n",a[i],b[i],c[i]);

    e=Mult(d);
    printf("\n\nd0=%lf\td1=%lf\td0*d1=%lf",d[0],d[1],e);

    double f[2]={e,e}, v_E[2]={0,6.282}, r_E[2]={1,0};
    printf("\n\nf=[%lf,%lf]",f[0],f[1]);

    g=BewFkt(0,d);
    h=BewFkt(1,d);
    printf("\n\nF1=%lf\tF2=%lf",g,h);

    double dt=1/N;
    printf("\n\n\ndt=%lf\nv_x\t\t_y\n\n",dt);
    for (i=1; i<=N; i++)
    {
        v_E[0]+=dt*BewFkt(0,r_E);
        v_E[1]+=dt*BewFkt(1,r_E);
        r_E[0]+=dt*v_E[0];
        r_E[1]+=dt*v_E[1];
        printf("%lf\t%lf\n",v_E[0],v_E[1]);
    }
    return 0;
}
