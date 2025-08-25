#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const double PI=3.141592654;

double Betrag(double A[2])
{
    return sqrt(A[0]*A[0]+A[1]*A[1]);
}

double Energie(double r[2], double v[2])
{
    return (pow(Betrag(v),2)/2-4*PI*PI/Betrag(r));
}

double BewFkt(int w, double r[2])
{
    return (-4*PI*PI*r[w]/pow(Betrag(r),3));
}

/*
double ModEul(int w, double dt, double r[2])
{
{
    double x, y;
    x=r[0]+(dt/2)*BewFkt(0,r);
    y=r[1]+(dt/2)*BewFkt(1,r);
    double z[2]={x,y};
    return (BewFkt(w,z));
}

double Heun(int w, double dt, double r[2])
{
    double x, y;
    x=r[0]+dt*BewFkt(0,r);
    y=r[1]+dt*BewFkt(1,r);
    double z[2]={x,y};
    return ((BewFkt(w,r)+BewFkt(w,z))/2);
}
*/

double RelFehl(double E, double E0) /**/
{
    return (fabs((E-E0)/E0));
}

int main()
{
    int i, k, N;        /*Laufvariablen und N*/
    double M, dt;       /*M=N, Zeitschritt dt*/
    double v[2], z[2];  /*Hilfsvektoren*/
    double r_E[2], r_ME[2], r_V[2], r_H[2]; /*Orts-Vektoren*/
    double v_E[2], v_ME[2], v_V[2], v_H[2]; /*Geschwindigkeits-Vektoren*/
    double r0[2]={1,0}, v0[2]={0,6.282};    /*Orts-& Geschwindigkeits-Startvektoren*/
    double E_E, E_ME, E_H, E_V; /*Energie-Werte*/
    double E0=Energie(r0,v0);   /*Energie-Startwert*/
    r_E[0]=r0[0], r_ME[0]=r0[0], r_V[0]=r0[0], r_H[0]=r0[0];    /*Setze alle Orts- & Geschwindigkeits-Vektoren auf Start*/
    r_E[1]=r0[1], r_ME[1]=r0[1], r_V[1]=r0[1], r_H[1]=r0[1];
    v_E[0]=v0[0], v_ME[0]=v0[0], v_V[0]=v0[0], v_H[0]=v0[0];
    v_E[1]=v0[1], v_ME[1]=v0[1], v_V[1]=v0[1], v_H[1]=v0[1];

    /*
    fp_E=fopen("ausgabe_E.dat","w");
    fp_ME=fopen("ausgabe_ME.dat","w");
    fp_H=fopen("ausgabe_H.dat","w");
    fp_V=fopen("ausgabe_V.dat","w");
    */
    FILE *fp;
    fp=fopen("ausgabe.dat","w");    /*Schreibe 'Tabellen-Datei': dt gegen jeweils dE/E der 4 Verfahren, hier linear, in gnuplot logarithmisch*/
    printf("F0_x=%lf, F0_y=%lf\nE0=%lf\n\ndt\t\tdE_E\t\tdE_ME\t\tdE_H\t\tdE_V\n\n",BewFkt(0,r0),BewFkt(1,r0),E0);

    for (k=10; k<=1000; k+=10)  /*Durchlauf fuer verschiedene N*/
    {
    N=k;
    M=N;
    dt=1/M;

    for (i=1; i<=20*N; i++) /*hier: 20*N: 20 Umlaeufe/Jahre*/
    {
    /*Euler*/
        v_E[0]+=dt*BewFkt(0,r_E);
        v_E[1]+=dt*BewFkt(1,r_E);
        r_E[0]+=dt*v_E[0];
        r_E[1]+=dt*v_E[1];
        /*fprintf(fp_E,"%lf\t%lf\n",r_E[0],r_E[1]);*/
    /*Mod. Euler*/
        /*v_ME[0]+=dt*ModEul(0,dt,r_ME);
        v_ME[1]+=dt*ModEul(1,dt,r_ME);
        r_ME[0]+=dt*v_ME[0];
        r_ME[1]+=dt*v_ME[1];*/
    /*Mod. Euler explizit*/
        z[0]=r_ME[0]+(dt/2)*BewFkt(0,r_ME);
        z[1]=r_ME[1]+(dt/2)*BewFkt(1,r_ME);
        v_ME[0]+=dt*BewFkt(0,z);
        v_ME[1]+=dt*BewFkt(1,z);
        r_ME[0]+=dt*v_ME[0];
        r_ME[1]+=dt*v_ME[1];
        /*fprintf(fp_ME,"%lf\t%lf\n",r_ME[0],r_ME[1]);*/
    /*Heun*/
        /*v_H[0]+=dt*Heun(0,dt,r_H);
        v_H[1]+=dt*Heun(1,dt,r_H);
        r_H[0]+=dt*v_H[0];
        r_H[1]+=dt*v_H[1];*/
        /*fprintf(fp_H,"%lf\t%lf\n",r_H[0],r_H[1]);*/
    /*Heun explizit*/
        z[0]=r_H[0]+dt*BewFkt(0,r_H);
        z[1]=r_H[1]+dt*BewFkt(1,r_H);
        v_H[0]+=dt*(BewFkt(0,r_H)+BewFkt(0,z))/2;
        v_H[1]+=dt*(BewFkt(1,r_H)+BewFkt(1,z))/2;
        r_H[0]+=dt*v_H[0];
        r_H[1]+=dt*v_H[1];
    /*Verlet*/
        v[0]=v_V[0]+(dt/2)*BewFkt(0,r_V);
        v[1]=v_V[1]+(dt/2)*BewFkt(1,r_V);
        r_V[0]+=dt*v[0];
        r_V[1]+=dt*v[1];
        v_V[0]=v[0]+(dt/2)*BewFkt(0,r_V);
        v_V[1]=v[1]+(dt/2)*BewFkt(1,r_V);
        /*fprintf(fp_V,"%lf\t%lf\n",r_V[0],r_V[1]);*/
    }
    E_E=Energie(r_E,v_E);   /*Berechne Energiewerte fuer jedes Verfahren*/
    E_ME=Energie(r_ME,v_ME);
    E_H=Energie(r_H,v_H);
    E_V=Energie(r_V,v_V);
    printf("%lf\t%lf\t%lf\t%lf\t%lf\n", dt, RelFehl(E_E,E0), RelFehl(E_ME,E0), RelFehl(E_H,E0), RelFehl(E_V,E0));       /*Ausgabe: s.o. entsp. 'Tabellen-Datei', hier linear, in gnuplot logarithmisch*/
    fprintf(fp,"%lf\t%lf\t%lf\t%lf\t%lf\n", dt, RelFehl(E_E,E0), RelFehl(E_ME,E0), RelFehl(E_H,E0), RelFehl(E_V,E0));   /*Schreibe in Datei: s.o. entsprechend 'Tabellen-Datei'*/

    }
    fclose(fp); /*Schliesse Datei*/
    /*
    fclose(fp_E);
    fclose(fp_ME);
    fclose(fp_H);
    fclose(fp_V);
    */
    return 0;
}
