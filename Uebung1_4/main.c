#include <stdio.h>
#include <math.h>
#include <float.h>
#include <limits.h>

double y=1;

double dFkt1(double x)
{
    return exp(x);  /*als 2. Funktion x^3 */
}

double dFkt2(double x)
{
    return pow(x,3);
}

double dVorAbleitung1(double h)
{
    return ((dFkt1(y+h)-dFkt1(y))/h);
}

double dZenAbleitung1(double h)
{
    return ((dFkt2(y+h)-dFkt2(y-h))/(2*h));
}

double dVorAbleitung2(double h)
{
    return ((dFkt1(y+h)-dFkt1(y))/h);
}

double dZenAbleitung2(double h)
{
    return ((dFkt2(y+h)-dFkt2(y-h))/(2*h));
}

double dAbl1(double x)
{
    return -exp(x); /*als 2. Ableitung 3x^2 */
}

double dAbl2(double x)
{
    return 3*pow(x,2); /*als 2. Ableitung 3x^2 */
}

double dBetrag(double c)
{
    if(c<0)
        return -c;
    else return c;
}

int main()
{
    char a;
    int j, b, N;
    double dAbleit1V, dAbleit1Z, dAbleit2V, dAbleit2Z, h, J;
    N=80;
    b=2;
    FILE *fp1V, *fp1Z, *fp2V, *fp2Z, *fpG;
    fp1V=fopen("ausgabe1Vor.dat","w");
    fp1Z=fopen("ausgabe1Zen.dat","w");
    fp2V=fopen("ausgabe2Vor.dat","w");
    fp2Z=fopen("ausgabe2Zen.dat","w");
    fpG=fopen("ausgabeGen.dat","w");

    printf("log|h|\t\tDelta_exp_V\tDelta_exp_Z\tDelta_cubic_V\tDelta_cubic_Z\n\n");
    for (j=1; j<=N; j++)
    {
        h=pow(b,-j);
        J=-log(h);
        dAbleit1V=log(dBetrag(dAbl1(y)-dVorAbleitung1(h)));
        dAbleit1Z=log(dBetrag(dAbl1(y)-dZenAbleitung1(h)));
        dAbleit2V=log(dBetrag(dAbl2(y)-dVorAbleitung2(h)));
        dAbleit2Z=log(dBetrag(dAbl2(y)-dZenAbleitung2(h)));
        printf("%lf\t%lf\t%lf\t%lf\t%lf\n",J,dAbleit1V,dAbleit1Z,dAbleit2V,dAbleit2Z);
        fprintf(fpG,"%lf\t%lf\t%lf\t%lf\t%lf\n",J,dAbleit1V,dAbleit1Z,dAbleit2V,dAbleit2Z);
        fprintf(fp1V,"%lf\t%lf\n",J,dAbleit1V);
        fprintf(fp1Z,"%lf\t%lf\n",J,dAbleit1Z);
        fprintf(fp2V,"%lf\t%lf\n",J,dAbleit2V);
        fprintf(fp2Z,"%lf\t%lf\n",J,dAbleit2Z);
    }

    fclose(fp1V);
    fclose(fp1Z);
    fclose(fp2V);
    fclose(fp2Z);
    fclose(fpG);

    printf("\nEnter character to quit!\n");
    scanf("%c",&a);
    return 0;
}
