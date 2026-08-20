#include <stdio.h>
#include <math.h>
#include <assert.h>

#define INFROOTS -2

/* QuadraticSolver: finds the solution of quadratic equation with coefficients coef_a, coef_b, coef_c
and writes them to the address proot1, proot2.
If equation has only one solution, it will be written to addres proot1.
Returns the number of solutions or INFROOTS if there are infinite. */
int QuadraticSolver(double coef_a, double coef_b, double coef_c, double* proot1, double* proot2);

/* PrintRoots: print roots depending on the value of nroots.
Returns 1 if everything is correct or 0 otherwise */
int PrintRoots(int nroots, double root1, double root2);

int main() {
    printf("Quadratic equations solver by Zotov Anton\n");
    printf("Enter the coefficients coef_a, coef_b, coef_c like: 1 2 1\n");

    double coef_a = 0, coef_b = 0, coef_c = 0;
    scanf("%lf %lf %lf", &coef_a, &coef_b, &coef_c);

    double root1 = 0, root2 = 0;
    int nroots = QuadraticSolver(coef_a, coef_b, coef_c, &root1, &root2);

    if (PrintRoots(nroots, root1, root2))
        return 1;
    return 0;
}


/* LinearSolver: finds the solution of linear equation with coefficients coef_a, coef_b
and writes them to the address proot.
Returns the number of solutions or INFROOTS if there are infinite. */
int LinearSolver(double coef_a, double coef_b, double* proot);

/* SmartDivision: returns the quotient of dividend/divider preventing the return of -0.0 */
double SmartDivision(double dividend, double divider);

int QuadraticSolver(double coef_a, double coef_b, double coef_c, double* proot1, double* proot2) {
    assert(proot1 != NULL);
    assert(proot2 != NULL);
    assert(proot1 != proot2);

    if (coef_a == 0) {
        return LinearSolver(coef_b, coef_c, proot1);
    }
    else { /* (coef_a != 0) => it's quadric education */
        double D = coef_b*coef_b - 4*coef_a*coef_c;
        if (D == 0) {
            *proot1 = SmartDivision(-coef_b, 2 * coef_a);
            return 1;
        }
        else if (D > 0) {
            double sqD = sqrt(D);
            *proot1 = SmartDivision(-coef_b + sqD, 2*coef_a);
            *proot2 = SmartDivision(-coef_b - sqD, 2*coef_a);
            return 2;
        }
        else { /* (D < 0) => there is no solutions */
            return 0;
        }
    }
}


int LinearSolver(double coef_a, double coef_b, double* proot){
    if (coef_a == 0) {
        if (coef_b == 0)
            return INFROOTS;
        else /* (coef_c != 0) */
            return 0;
    }
    else { /* (coef_b != 0) */
        *proot = SmartDivision(-coef_b, coef_a);
        return 1;
    }
}


double SmartDivision(double dividend, double divider){
    if (dividend != 0)
        return dividend / divider;
    else
        return 0;
}


int PrintRoots(int nroots, double root1, double root2) {  //TODO any parameters count
    if(nroots == 2 && root1 == root2)
        nroots = 1;

    switch(nroots) {
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
