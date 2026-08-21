#include "title.h"

int main() {
    double coef_a = 0.0, coef_b = 0.0, coef_c = 0.0;
    if (CoefficientsRequest(&coef_a, &coef_b, &coef_c)) {
        return 0;
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
    while(1) {                          // request the introduction of coefficients unless input is correct or program is closed
        printf("Enter the coefficients coef_a, coef_b, coef_c like: 1 2 1\n");

        if (scanf("%lf %lf %lf", p_coef_a, p_coef_b, p_coef_c) != 3){
            printf("CoefficientsRequest: There must be three coefficients\n");
            printf("Do you want to try again? (write Y if you do or N otherwise)\n");

            if(AskYesOrNo())            // answer is Y
                continue;
            else                        // answer is N
                return 1;
        }
        else /* number of arguments is 3 => input is correct */
            break;
    }

    return 0;
}


int AskYesOrNo() {
    char answer = 0;
    while(1) {                  // request answer unless it is correct
        ClearInputBuf();        // clear the input buffer from excess symbols
        scanf("%c", &answer);
        if (answer == 'Y')
            return 1;
        else if (answer == 'N')
            return 0;
        printf("PLease write Y or N\n");
    }
}


void ClearInputBuf(){
    char c;
    while(((c = getchar()) != EOF) && (c != '\n'));
}


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
