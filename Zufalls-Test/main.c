#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main()
{
    int i, n=1000;
    double q;
    long m=1e9+7, a=1009, b=1, Q=q;
    for (i=1; i<=n; i++)
    {
        Q=(a*Q+b)%m;
        q=Q;
        printf("%lf\n",q);
    }
    return 0;
}
