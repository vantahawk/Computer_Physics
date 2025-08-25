#include <stdio.h>
#include <math.h>

double dBetrag(double c)
{
    if(c<0)
        return -c;
    else return c;
}

int main()
{
    int n, i;
    double a=-10, b=10;
    double h, x, F1, F2;

    printf("Enter number of iterations n!\n");
    scanf("%d",&n);

    if (n<1)
    {
        printf("n too small!");
        return 0;
    }
    else if(n>10000)
    {
        printf("n too big!");
        return 0;
    }

    printf("\n----------\n\n");
    FILE *fp;
    fp=fopen("ausgabe.dat", "w");

    h=(b-a)/n;
    printf("i\tx\t\tsin(x^2)\tatan(sqrt(abs(x)))\n");
    for(i=0; i<=n; i++)
    {
        x=a+i*h;
        F1=sin(x*x);
        F2=atan(sqrt(dBetrag(x)));
        printf("%d\t%f\t%f\t%f\n",i,x,F1,F2);
        fprintf(fp,"%f %f %f\n",x,F1,F2);
    }
    fclose(fp);
    return 0;
}

