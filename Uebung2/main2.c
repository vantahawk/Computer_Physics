#include <stdio.h>
#include <math.h>

int main()
{
    double x, y;
    printf("Enter x: ");
    scanf("%lf",&x)
    y=1/(1+25*(x*x));
    printf("\nResult: y = %lf\n",y);
    return(0);
}
