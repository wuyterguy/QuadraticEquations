#include "title.h"

int main() {
    double coef_a = 0.0, coef_b = 0.0, coef_c = 0.0;

    inptype askt_result = AskInputType();
    if (askt_result == FILE_INPUT) {
        if (FileRequestCoefficients(&coef_a, &coef_b, &coef_c))
            return 1;
    }
    else if (askt_result == CONSOLE_INPUT) {
        int req_result = RequestCoefficients(&coef_a, &coef_b, &coef_c);
        if (req_result == 1)
            return 0;
        else if (req_result == 2)
            return 2;
    }
    else
        return 3;

    double root1 = 0.0, root2 = 0.0;
    rootsc nroots = SolveQuadratic(coef_a, coef_b, coef_c, &root1, &root2);

    if (PrintRoots(nroots, root1, root2)) {
        fprintf(stderr, Color("PrintRoots: Unexpected value of nroot\n", RED));
        return 4;
    }

    return 0;
}


inptype AskInputType() {
    printf("Quadratic equations solver by " Color("Zotov Anton", YEL) "\n");
    printf("Would you like to use file input from file " Color("coefficients.txt", YEL) "? (write " Color("Y", MAG) " or " Color("N", MAG) ")\n");
    printf("If you write N it will be console input\n");

    answ ask_result = AskYesOrNo();
    if (ask_result == YES)
        return FILE_INPUT;
    else if (ask_result == NO)
        return CONSOLE_INPUT;
    else
        return ERR;
}


int FileRequestCoefficients(double* const p_coef_a, double* const p_coef_b, double* const p_coef_c) {
    FILE* coefficients_file = fopen("coefficients.txt", "r");

    if (coefficients_file == NULL) {
        printf("file " Color("coefficients.txt", YEL) "doesn't exist or you don't have \"r\" permission\n");
        return 1;
    }

    if (fscanf(coefficients_file, "%lf %lf %lf", p_coef_a, p_coef_b, p_coef_c) != 3) {
        printf(Color("FileRequestCoefficients:", YEL) "There must be three coefficients like: " Color("1 2 1", MAG) "\n");
        return 2;
    }

    fclose(coefficients_file);
    return 0;
}


int RequestCoefficients(double* const p_coef_a, double* const p_coef_b, double* const p_coef_c) {    //TODO any parameters count
    assert(p_coef_a != NULL);
    assert(p_coef_b != NULL);
    assert(p_coef_c != NULL);
    assert(p_coef_a != p_coef_b && p_coef_a != p_coef_c && p_coef_b != p_coef_c);

    while(1) {                          // request the introduction of coefficients unless input is correct or program is closed
        printf("Enter the coefficients coef_a, coef_b, coef_c like: " Color("1 2 1", MAG) "\n");

        if (scanf("%lf %lf %lf", p_coef_a, p_coef_b, p_coef_c) != 3) {
            ClearInputBuf();        // clear the input buffer from excess symbols

            printf(Color("RequestCoefficients:", YEL) "There must be three coefficients like: " Color("1 2 1", MAG) "\n");
            printf("Do you want to try again? (write " Color("Y", MAG) " if you do or " Color("N", MAG) " otherwise)\n");

            answ ask_result = AskYesOrNo();
            if (ask_result == YES)
                continue;
            else if (ask_result == NO)
                return 1;
            else
                return 2;

        }
        else /* number of arguments is 3 => input is correct */
            break;
    }

    return 0;
}


answ AskYesOrNo() {
    char c;
    answ result = NONE;
    while(1) {                      // request answer unless it is correct
        result = NONE;
        bool clear = true;
        c = getchar();
        if (c == 'Y')
            result = YES;
        else if (c  == 'N')
            result = NO;
        else if ((c == '\n') || (c == EOF))
            clear = false;          // buffer is empty
        if (result != NONE) {
            c = getchar();
            if((c == '\n') || (c == EOF))
                return result;
        }

        if (clear)
            ClearInputBuf();

        printf("PLease write " Color("Y", MAG) " or " Color("N", MAG) "\n");
    }
}


void ClearInputBuf(){
    char c;
    while(((c = getchar()) != EOF) && (c != '\n'));
}


rootsc SolveQuadratic(const double coef_a, const double coef_b, const double coef_c,
                       double* const p_root1, double* const p_root2) {
    assert(p_root1 != NULL);
    assert(p_root2 != NULL);
    assert(p_root1 != p_root2);

    if (CmpEpsPrec(coef_a, 0)) {
        return SolveLinear(coef_b, coef_c, p_root1);
    }
    else { /* (coef_a != 0) => it's quadric education */
        double discr = coef_b*coef_b - 4*coef_a*coef_c;
        if (CmpEpsPrec(discr, 0)) {
            *p_root1 = DivideSmart(-coef_b, 2*coef_a);
            return ONEROOT;
        }
        else if (discr > 0) {
            double sq_discr = sqrt(discr);
            *p_root1 = DivideSmart(-coef_b + sq_discr, 2*coef_a);
            *p_root2 = DivideSmart(-coef_b - sq_discr, 2*coef_a);
            return TWOROOTS;
        }
        else { /* (discr < 0) => there is no solutions */
            return NOROOTS;
        }
    }
}


rootsc SolveLinear(const double coef_a, const double coef_b, double* const p_root) {
    assert(p_root != NULL);

    if (CmpEpsPrec(coef_a, 0)) {
        if (CmpEpsPrec(coef_b, 0))
            return INFROOTS;
        else /* (coef_c != 0) */
            return NOROOTS;
    }
    else { /* (coef_b != 0) */
        *p_root = DivideSmart(-coef_b, coef_a);
        return ONEROOT;
    }
}


double DivideSmart(const double dividend, const double divider) {
    assert(divider != 0);

    if (!CmpEpsPrec(dividend, 0))
        return dividend / divider;
    else
        return 0;
}


bool CmpEpsPrec(const double a, const double b) {
    if ((a <= b + EPSILON) && (a >= b - EPSILON))
        return 1;
    else
        return 0;
}


int PrintRoots(rootsc nroots, const double root1, const double root2) {
    if(CmpEpsPrec(nroots, TWOROOTS) && CmpEpsPrec(root1, root2))
        nroots = ONEROOT;

    switch(nroots) {
        case INFROOTS:
            printf("Infinitely many solutions\n");
            break;
        case NOROOTS:
            printf("There isn't any solution\n");
            break;
        case ONEROOT:
            printf("There is only solution x1: " Color("%.5lf", MAG) "\n", root1);
            break;
        case TWOROOTS:
            printf("There are two solutions x1: " Color("%.5lf", MAG) "; x2: " Color("%.5lf", MAG) "\n", root1, root2);
            break;
        default:
            return 1;
    }

    return 0;
}
