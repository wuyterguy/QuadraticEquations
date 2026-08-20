#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <math.h>

#define EPSILON 0.00001                     //comparison accuracy
typedef enum {NOROOTS, ONEROOT, TWOROOTS, INFROOTS = -2} rootsc;

/* CoefficientsRequest: transmit three values from input to addresses pcoef_a, pcoef_b, pcoef_c.
Returns 1 if the number of coefficients is three or 0 otherwise. */
int CoefficientsRequest(double* const p_coef_a, double* const p_coef_b, double* const p_coef_c);

/* QuadraticSolver: finds the solution of quadratic equation with coefficients coef_a, coef_b, coef_c
and writes them to the address proot1, proot2.
If equation has only one solution, it will be written to addres proot1.
Returns the number of solutions or INFROOTS if there are infinite. */
rootsc QuadraticSolver(const double coef_a, double const coef_b, double const coef_c, double* const p_root1, double* const p_root2);

/* PrintRoots: print roots depending on the value of nroots.
Returns 1 if everything is correct or 0 otherwise. */
int PrintRoots(rootsc nroots, const double root1, const double root2);

int main() {
    double coef_a = 0.0, coef_b = 0.0, coef_c = 0.0;
    if (CoefficientsRequest(&coef_a, &coef_b, &coef_c)) {
        fprintf(stderr, "CoefficientsRequest: There must be three coefficients\n");
        return 1;
    }

    double root1 = 0.0, root2 = 0.0;
    rootsc nroots = QuadraticSolver(coef_a, coef_b, coef_c, &root1, &root2);

    if (PrintRoots(nroots, root1, root2)) {
        fprintf(stderr, "PrintRoots: Unexpected value of nroot\n");
        return 2;
    }

    return 0;
}


int CoefficientsRequest(double* const p_coef_a, double* const p_coef_b, double* const p_coef_c) {    //TODO any parameters count
    assert(p_coef_a != NULL);
    assert(p_coef_b != NULL);
    assert(p_coef_c != NULL);
    assert(p_coef_a != p_coef_b && p_coef_a != p_coef_c && p_coef_b != p_coef_c);

    printf("Quadratic equations solver by Zotov Anton\n");
    printf("Enter the coefficients coef_a, coef_b, coef_c like: 1 2 1\n");

    if (scanf("%lf %lf %lf", p_coef_a, p_coef_b, p_coef_c) != 3)
        return 1;

    return 0;
}


/* EpsPrecComparison: compares a and b taking EPSILON as accuracy into account.
Returns 1 if a = b or 0 otherwise. */
int EpsPrecComparison(const double a, const double b);

/* LinearSolver: finds the solution of linear equation with coefficients coef_a, coef_b
and writes them to the address proot.
Returns the number of solutions or INFROOTS if there are infinite. */
rootsc LinearSolver(const double coef_a, const double coef_b, double* const p_root);

/* SmartDivision: returns the quotient of dividend/divider preventing the return of -0.0 */
double SmartDivision(const double dividend, const double divider);

rootsc QuadraticSolver(const double coef_a, const double coef_b, const double coef_c, double* const p_root1, double* const p_root2) {
    assert(p_root1 != NULL);
    assert(p_root2 != NULL);
    assert(p_root1 != p_root2);

    if (EpsPrecComparison(coef_a, 0)) {
        return LinearSolver(coef_b, coef_c, p_root1);
    }
    else { /* (coef_a != 0) => it's quadric education */
        double D = coef_b*coef_b - 4*coef_a*coef_c;
        if (EpsPrecComparison(D, 0)) {
            *p_root1 = SmartDivision(-coef_b, 2 * coef_a);
            return ONEROOT;
        }
        else if (D > 0) {
            double sqD = sqrt(D);
            *p_root1 = SmartDivision(-coef_b + sqD, 2*coef_a);
            *p_root2 = SmartDivision(-coef_b - sqD, 2*coef_a);
            return TWOROOTS;
        }
        else { /* (D < 0) => there is no solutions */
            return NOROOTS;
        }
    }
}


rootsc LinearSolver(const double coef_a, const double coef_b, double* const p_root) {
    assert(p_root != NULL);

    if (EpsPrecComparison(coef_a, 0)) {
        if (EpsPrecComparison(coef_b, 0))
            return INFROOTS;
        else /* (coef_c != 0) */
            return NOROOTS;
    }
    else { /* (coef_b != 0) */
        *p_root = SmartDivision(-coef_b, coef_a);
        return ONEROOT;
    }
}


double SmartDivision(const double dividend, const double divider) {
    if (!EpsPrecComparison(dividend, 0))
        return dividend / divider;
    else
        return 0;
}


int EpsPrecComparison(const double a, const double b) {
    if ((a <= b + EPSILON) && (a >= b - EPSILON))
        return 1;
    else
        return 0;
}


int PrintRoots(rootsc nroots, const double root1, const double root2) {
    if(EpsPrecComparison(nroots, TWOROOTS) && EpsPrecComparison(root1, root2))
        nroots = ONEROOT;

    switch(nroots) {
        case INFROOTS:
            printf("Infinitely many solutions\n");
            break;
        case NOROOTS:
            printf("There isn't any solution\n");
            break;
        case ONEROOT:
            printf("There is only solution x1: %.5lf\n", root1);
            break;
        case TWOROOTS:
            printf("There are two solutions x1: %.5lf; x2: %.5lf\n", root1, root2);
            break;
        default:
            return 1;
    }

    return 0;
}
