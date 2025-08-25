/*!Notwendige Libraries zum einholen, speziell nrutil.c !!!*/
#include <stdio.h>
/*#include <stdlib.h>*/
#include <math.h>
/*!Praeprozessor-Block*/
#define NRANSI
#define float double
#include "nrutil.c"
/*#include "ludcmp.c"
#include "lubksb.c"*/
#undef float

/*!Vektoren mit double *vec deklarieren, mit vec=vector(m,n) initialieren! m=1.Element, n=letztes Element*/
/*!Matrizen mit double **mat deklarieren, mit mat=vector(m,n,p,q) initialieren! m=1.Zeile, n=letzte Zeile, o=1.Spalte, p=letzte Spalte*/
/*!Allgemein darauf achten, dass eingegebene und ausgegebene Dimensionen von Feldern stimmen!*/
/*!Bei allen Funktionen, bei denen in ein Feld a nur Elemente eingefuellt werden (Vek(), Mat(), Dia(), DiaS()), muss a in sich selbst eingesetzt werden bei Aufruf!
Bsp: Dia():  a=Dia(a,d,m,n,iO,jO);
Einige Funktionen, v.a. 'Produkt'-Funktionen, deklarieren ihr eigenes Ausgabe-Feld/-Element.
Dies kann bei häufigem oder verschachteltem Aufruf der Funktionen zu hoher Belastung fuehren!*/

/*!Setzt Vektor auf Wert d
Vektor in sich selbst einsetzen!*/
double *Vek(double *vec, double d, int m, int n)
{
    int i;
    for (i=m; i<=n; i++)
        vec[i]=d;
    return vec;
}

/*!Setzt !quadratische! Matrix auf Wert d
Matrix in sich selbst einsetzen!*/
double **QMat(double **mat, double d, int m, int n)
{
    int i, j;
    for (i=m; i<=n; i++)
    {
        for (j=m; j<=n; j++)
            mat[i][j]=d;
    }
    return mat;
}

/*!Setzt !allgemeine! Matrix auf Wert d
Matrix in sich selbst einsetzen!*/
double **Mat(double **mat, double d, int m, int n)
{
    int i, j;
    for (i=m; i<=n; i++)
    {
        for (j=p; j<=q; j++)
            mat[i][j]=d;
    }
    return mat;
}

/*!Fuellt !quadratische! Matrix diagonal auf mit Wert d mit Zeilen-Offset iO & Spalten-Offset jO.
Auf Offset achten!!! - Zellenfuellung sollte nicht ueber Matrix hinaus gehen.*/
double **Dia(double **mat, double d, int m, int n, int iO, int jO)
{
    int i;
    for (i=m; i<=n; i++)
        mat[i+iO][i+jO]=d;
    return mat;
}

/*!Fuellt !quadratische! Matrix diagonal auf mit Wert d mit Zeilen/Spalten-Offset kO.
kO<0 fuer Zeilen-Offset!,
kO>0 fuer Spalten-Offset!*/
double **DiaS(double **mat, double d, int m, int n, int kO)
{
    int i;
    if (k<=0)
    {
        for (i=m; i<=n+kO; i++)
            mat[i-kO][i]=d;
    }
    if (k>0)
    {
        for (i=m; i<=n-kO; i++)
            mat[i][i+kO]=d;
    }
    return mat;
}

/*!Addiert zwei Vektoren
Fuer Subtraktion (z.B. Abstandsvektor) Vorzeichen in Fkt. wechseln bzw. entsprechendes Argument invertieren*/
double *AddV(double *a, double *b, int m, int n)
{
    int i;
    for (i=m; i<=n; i++)
        a[i]=a[i]+b[i];
    return a;
}

/*!Addiert zwei !quadratische! Matrizen*/
double **QMatV(double **a, double **b, int m, int n)
{
    int i,j;
    for (i=m; i<=n; i++)
    {
        for (j=m; i<=n; i++)
            a[i][j]=a[i][j]+b[i][j];
    }
    return a;
}

/*!Addiert zwei !allgemeine! Matrizen*/
double **QMatV(double **a, double **b, int m, int n, int p, int q)
{
    int i,j;
    for (i=m; i<=n; i++)
    {
        for (j=p; i<=q; i++)
            a[i][j]=a[i][j]+b[i][j];
    }
    return a;
}

/*!Betrag*/
double Betrag(double *vec, int m, int n)
{
    int i;
    double B=0;
    for (i=m; i<=n; i++)
        B+=vec[i]*vec[i];
    return sqrt(B);
}

/*!Skalarprodukt
Auch Betragsquadrat verwendbar - selben Vektor doppelt einsetzen*/
double SP(double *a, double *b, int m, int n)
{
    int i;
    double P=0;
    for (i=m; i<=n; i++)
        P+=a[i]*b[i];
    return P;
}

/*!Quad.! Matrix-Vektor-Produkt*/
double *QMVP(double **a, double *b, int m, int n)
{
    int i, j, k;
    double *vec;
    vec=vector(m,n);
    for (i=m; i<=n; i++)    /*Setze Ergebnisvektor auf 0*/
        vec[i]=0;
    for (i=m; i<=n; i++)    /*Berechne Einzel-Produkt-Summen*/
    {
        for (j=m; j<=n; j++)
            vec[i]+=a[i][j]*b[j];
    }
    return vec;
}

/*!Allg.! Matrix-Vektor-Produkt
n-m: Zeilenlaenge der Matrix a bzw. Laenge des Ausgabe-Vektors vec!
q-p: Spaltenlaenge der Matrix a bzw. Laenge des Eingabe-Vektors b!*/
double *MVP(double **a, double *b, int m, int n, int p, int q)
{
    int i, j;
    double *vec;
    vec=vector(m,n);
    for (i=m; i<=n; i++)    /*Setze Ergebnisvektor auf 0*/
        vec[i]=0;
    for (i=m; i<=n; i++)    /*Berechne Einzel-Produkt-Summen*/
    {
        for (j=p; j<=q; j++)
            vec[i]+=a[i][j]*b[j];
    }
    return vec;
}

/*!Quad! Matrix-Matrix-Produkt*/
double **QMMP(double **a, double **b, int m, int n)
{
    int i, j, k;
    double **mat;
    mat=matrix(m,n,m,n);
    for (i=m; i<=n; i++)    /*Setze Ergebnisvektor auf 0*/
    {
        for (j=m; j<=n; j++)
            mat[i][j]=0;
    }
    for (i=m; i<=n; i++)    /*Berechne Einzel-Produkt-Summen*/
    {
        for (j=m; j<=n; j++)
        {
            for (k=m; k<=n; k++)
                mat[i][j]+=a[i][k]*b[k][j];
        }
    }
    return mat;
}

/*!Allg.! Matrix-Matrix-Produkt
n-m: Zeilenlaenge der 1. Matrix a bzw. Ausgabe-Matrix mat!
q-p: Spaltenlaenge der 2. Matrix b bzw. Ausgabe-Matrix mat!
s-r: Spaltenlaenge der 1. Matrix a bzw. Zeilenlaenge der 2. Matrix b!*/
double **MMP(double **a, double **b, int m, int n, int p, int q, int r, int s)
{
    int i, j, k;
    double **mat;
    mat=matrix(m,n,p,q);
    for (i=m; i<=n; i++)    /*Setze Ergebnisvektor auf 0*/
    {
        for (j=p; j<=q; j++)
            mat[i][j]=0;
    }
    for (i=m; i<=n; i++)    /*Berechne Einzel-Produkt-Summen*/
    {
        for (j=p; j<=q; j++)
        {
            for (k=r; k<=s; k++)
                mat[i][j]+=a[i][k]*b[k][j];
        }
    }
    return mat;
}

/*!Transformiert !quadratische! Matrix*/
double **QTrans(double **a, int m, int n)
{
    int i, j;
    for (i=m; i<=n; i++)
    {
        for (j=m; j<=n; i++)
            a[i][j]=a[j][i];
    }
    return a;
}

/*!Transformiert !allgemeine! Matrix*/
double **Trans(double **a, int m, int n, int p, int q)
{
    int i, j;
    double *mat;
    mat=matrix(p,q,m,n);
    for (i=p; i<=q; i++)
    {
        for (j=m; j<=n; i++)
            mat[i][j]=a[j][i];
    }
    return a;
}

/*!Kreuzprodukt - nur 3D! - m je nach Start der Nummerierung waehlen!*/
double *KP(double *a, double *b, int m)
{
    double *vec;
    vec=vector(m,n);
    vec[m]=a[m+1]*b[m+2]-a[m+2]*b[m+1];
    vec[m+1]=a[m+2]*b[m]-a[m]*b[m+2];
    vec[m+2]=a[m]*b[m+1]-a[m+1]*b[m];
    return vec;
}

/*!Determinante fuer 2D - m je nach Start der Nummerierung waehlen!*/
double Det2D(double **a, int m)
{
    return (a[m][m]*a[m+1][m+1]-a[m+1][m]*a[m][m+1]);
}

/*!Determinante fuer 3D - m je nach Start der Nummerierung waehlen!*/
double Det2D(double **a, int m)
{
    double pos, neg;
    pos=a[m][m]*a[m+1][m+1]*a[m+2][m+2]+a[m+1][m]*a[m+2][m+1]*a[m][m+2]+a[m+2][m]*a[m+1][m]*a[m+2][m+1];
    neg=a[m+2][m]*a[m+1][m+1]*a[m][m+2]+a[m+1][m]*a[m][m+1]*a[m+2][m+2]+a[m][m]*a[m+2][m+1]*a[m+1][m+2];
    return (pos-neg);
}
