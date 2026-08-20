#include <stdio.h>
#include <math.h>
#include <assert.h>

#define INFROOTS -2

/* Quadratic Solver: finds the solution of quadratic equation with coefficients a, b, c.
Writes the found solutions to the address px1, px2.
If equation has only one solution, it will be written to addres px1.
Return the number of solutions or INFROOTS if there are infinite */
int QuadraticSolver(double a, double b, double c, double *px1, double *px2);

int main(){
    printf("Quadratic equations solver by Zotov Anton\n");
    printf("Enter the coefficients a, b, c like: 1 2 1\n");
    double a = 0, b = 0, c = 0;
    scanf("%lf %lf %lf", &a, &b, &c);

    double x1 = 0, x2 = 0;
    int nroots = QuadraticSolver(a, b, c, &x1, &x2);

    switch(nroots){
        case INFROOTS:
            printf("Infinitely many solutions\n");
            break;
        case 0:
            printf("There isn't any solution\n");
            break;
        case 1:
            printf("There is only solution x1: %.4lf\n", x1);
            break;
        case 2:
            printf("There are two solutions x1: %.4lf; x2: %.4lf\n", x1, x2);
            break;
        default:
            printf("unexpected value of nroot returned by QuadraticSolver()");
            return 1;
    }
    return 0;
}

/* Quadratic Solver: finds the solution of quadratic equation with coefficients a, b, c.
Writes the found solutions to the address px1, px2.
If equation has only one solution, it will be written to addres px1.
Return the number of solutions or INFROOTS if there are infinite */
int QuadraticSolver(double a, double b, double c, double *px1, double *px2){
    assert(px1 != NULL);
    assert(px2 != NULL);
    assert(px1 != px2);

    if (a == 0){
        if (b == 0){
            return (c == 0) ? INFROOTS : 0;
        }
        else{ /* (b != 0) */
            if (c != 0)
                *px1 = -c/b;
            else /* (c == 0) */
                *px1 = 0.0; /* without this fragment of code function may write -0.0 to px1 */
            return 1;
        }
    }
    else{ /* (a != 0) => it's quadric education */
        double D = b*b - 4*a*c;
        if (D == 0){
            if (b != 0)
                *px1 = -b / (2*a);
            else /* (b == 0) */
                *px1 = 0.0; /* without this fragment of code function may write -0.0 to px1 */
            return 1;
        }
        else if (D > 0){
            double sqD = sqrt(D);
            if (-b + sqD != 0)
                *px1 = (-b + sqD) / (2*a);
            else /* (-b + sqD == 0) */
                *px1 = 0.0; /* without this fragment of code function may write -0.0 to px1 */
            if (-b - sqD != 0)
                *px2 = (-b - sqD) / (2*a);
            else /* (-b - sqD == 0) */
                *px2 = 0.0; /* without this fragment of code function may write -0.0 to px2 */
            return 2;
        }
        else{ /* (D < 0) => there is no solutions */
            return 0;
        }
    }
}
