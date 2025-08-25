/*Maschinengenauigkeit*/

#include <stdio.h>
#include <float.h>

int main()
{
    int n=0;
    double alpha=1, MG=1;
    double MG2=DBL_EPSILON;
    printf("%lf\n",MG2);
    while(alpha!=alpha+2*MG)
        {
            MG/=2;
            ++n;
        }
    printf("Die Maschiengenauigkeit betraegt: %lf\nDie binaere Aufloesung betraegt: %d bits\n",MG,n);

    return 0;
}
