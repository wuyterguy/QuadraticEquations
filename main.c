#include <stdio.h>
#include <math.h>
#include <assert.h>

#define INFROOTS -2

/* Quadratic Solver: finds the solution of quadratic equation with coefficients a, b, c.
Writes the found solutions to the address px1, px2.
If equation has only one solution, it will be written to addres px1.
Return the number of solutions or INFROOTS if there are infinite */
int QuadraticSolver(double coef_a, double coef_b, double coef_c, double* proot1, double* proot2);

int main(){
    printf("Quadratic equations solver by Zotov Anton\n");
    printf("Enter the coefficients coef_a, coef_b, coef_c like: 1 2 1\n");
    double coef_a = 0, coef_b = 0, coef_c = 0;
    scanf("%lf %lf %lf", &coef_a, &coef_b, &coef_c);

    double root1 = 0, root2 = 0;
    int nroots = QuadraticSolver(coef_a, coef_b, coef_c, &root1, &root2);

    switch(nroots){
        case INFROOTS:
            printf("Infinitely many solutions\n");
            break;
        case 0:
            printf("There isn't any solution\n");
            break;
        case 1:
            printf("There is only solution x1: %.4lf\n", root1);
            break;
        case 2:
            printf("There are two solutions x1: %.4lf; x2: %.4lf\n", root1, root2);
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
int QuadraticSolver(double coef_a, double coef_b, double coef_c, double* proot1, double* proot2){
    assert(proot1 != NULL);
    assert(proot2 != NULL);
    assert(proot1 != proot2);

    if (coef_a == 0){
        if (coef_b == 0){
            if (coef_c == 0)
                return INFROOTS;
            else /* (coef_c != 0) */
                return 0;
        }
        else{ /* (coef_b != 0) */
            if (coef_c != 0)
                *proot1 = -coef_c/coef_b;
            else /* (coef_c == 0) */
                *proot1 = 0.0; /* without this fragment of code function may write -0.0 to proot1 */
            return 1;
        }
    }
    else{ /* (coef_a != 0) => it's quadric education */
        double D = coef_b*coef_b - 4*coef_a*coef_c;
        if (D == 0){
            if (coef_b != 0)
                *proot1 = -coef_b / (2*coef_a);
            else /* (coef_b == 0) */
                *proot1 = 0.0; /* without this fragment of code function may write -0.0 to proot1 */
            return 1;
        }
        else if (D > 0){
            double sqD = sqrt(D);
            if (-coef_b + sqD != 0)
                *proot1 = (-coef_b + sqD) / (2*coef_a);
            else /* (-coef_b + sqD == 0) */
                *proot1 = 0.0; /* without this fragment of code function may write -0.0 to proot1 */
            if (-coef_b - sqD != 0)
                *proot2 = (-coef_b - sqD) / (2*coef_a);
            else /* (-coef_b - sqD == 0) */
                *proot2 = 0.0; /* without this fragment of code function may write -0.0 to proot2 */
            return 2;
        }
        else{ /* (D < 0) => there is no solutions */
            return 0;
        }
    }
}
