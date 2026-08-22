#include "title.c"

int main() {
    double coef_a = NAN, coef_b = NAN, coef_c = NAN;

    Input_type askt_result = AskInputType();
    if (askt_result == FILE_INPUT) {
        Freq_err freq_result = FileRequestCoefficients(&coef_a, &coef_b, &coef_c);
        if (freq_result == FREQ_STOP_PROGRAM)
            return MAIN_NORMAL;
        else if (freq_result != FREQ_NORMAL)
            return MAIN_INPUT_ERROR;
    }
    else if (askt_result == CONSOLE_INPUT) {
        Req_err req_result = RequestCoefficients(&coef_a, &coef_b, &coef_c);
        if (req_result == REQ_STOP_PROGRAM)
            return MAIN_NORMAL;
        else if (req_result == REQ_INPUT_ERROR)
            return MAIN_INPUT_ERROR;
    }
    else
        return MAIN_INPUT_ERROR;

    double root_1 = NAN, root_2 = NAN;
    Roots_c n_roots = SolveQuadratic(coef_a, coef_b, coef_c, &root_1, &root_2);

    if (PrintRoots(n_roots, root_1, root_2) != PRINT_NORMAL) {
        fprintf(stderr, Color("PrintRoots: Unexpected value of n_roots\n", RED));
        return MAIN_NROOT_ERROR;
    }

    return MAIN_NORMAL;
}


Input_type AskInputType() {
    printf("Quadratic equations solver by " Color("Zotov Anton", YEL) "\n");
    printf("Would you like to use file input from file? (write "
           Color("Y", MAG) " or " Color("N", MAG) ")\n");
    printf("If you write N it will be console input\n");

    Answer ask_result = AskYesOrNo();
    if (ask_result == YES)
        return FILE_INPUT;
    else if (ask_result == NO)
        return CONSOLE_INPUT;
    else
        return ERROR;
}


Freq_err FileRequestCoefficients(double* const p_coef_a, double* const p_coef_b, double* const p_coef_c) {
    assert(p_coef_a != NULL);
    assert(p_coef_b != NULL);
    assert(p_coef_c != NULL);
    assert(p_coef_a != p_coef_b && p_coef_a != p_coef_c && p_coef_b != p_coef_c);

    Freq_err error_identification = FREQ_NORMAL;

    while(1) {                                          // request the correct file unless input is possible to read 3 coefficients or program is closed
        printf("Enter a filename like: " Color("coefficients.txt", YEL) "\n");

        char* filename = GetFilename();

        FILE* coefficients_file = fopen(filename, "r");

        error_identification = FREQ_NORMAL;

        if (coefficients_file == NULL)
            error_identification = FREQ_FOPEN_ERROR;
        else { /* (coefficients_file != NULL) */
            if (fscanf(coefficients_file, "%lf %lf %lf", p_coef_a, p_coef_b, p_coef_c) != 3)
                error_identification = FREQ_INPUT_ERROR;
            if (ferror(coefficients_file)) {
                printf("File " Color("%s", YEL) " - error during the reading\n", filename);
                fclose(coefficients_file);
                free(filename);
                return FREQ_READ_ERROR;
            }
        }

        if (error_identification != FREQ_NORMAL) {      // any error
            if (error_identification == FREQ_FOPEN_ERROR)
                printf("File " Color("%s", YEL) " doesn't exist or you don't have \"r\" permission\n", filename);
            if (error_identification == FREQ_INPUT_ERROR)
                printf(Color("FileRequestCoefficients", YEL) ": " Color("%s", YEL)
                       ": There must be three coefficients like: " Color("1 2 1", MAG) "\n", filename);
            printf("Do you want to try again? (write " Color("Y", MAG) " if you do or "
                   Color("N", MAG) " otherwise)\n");

            fclose(coefficients_file);
            free(filename);

            Answer ask_result = AskYesOrNo();
            if (ask_result == YES)
                continue;
            else if (ask_result == NO)
                return FREQ_STOP_PROGRAM;
            else
                return FREQ_INPUT_ERROR;
        }
        else /* (error_identification == FREQ_NORMAL) => everything is correct */
            break;

        fclose(coefficients_file);
        free(filename);
    }

    return FREQ_NORMAL;
}


char* GetFilename() {
    char c;
    char* filename = (char*)calloc(MAX_FILENAME_LONG, sizeof(char));
    char* name_end = filename;
    bool clear = true;

    while (1) {                                                    // request answer unless it is correct
        name_end = filename;
        clear = true;
        while (((c = getchar()) == ' ') || (c == '\t'));           // skip spaces

        if (!isalpha(c)) {                                         // first symbol is not letter
            if ((c == '\n') || (c == EOF))
                clear = false;
            goto CLEAR_BUF_AND_REPRINT; /* continue with clear and reprint */
        }

        *name_end++ = c;                                           // read first symbol which is letter
        while(isalnum(c = getchar()) || (c == '.') || (c == '_'))  // read symbols until there is space symbol or end of buffer
            *name_end++ = c;

        while ((c == ' ') || (c == '\t'))                          // skip spaces
            c = getchar();

        if ((c == '\n') || (c == EOF)) {                           // this is correct name => return it
            *name_end++ = '\0';
            return filename;
        }

        CLEAR_BUF_AND_REPRINT: /* from goto */

        if (clear)
            ClearInputBuf();
        printf("Enter a filename like: " Color("coefficients.txt", YEL) "\n");
    }
}


Req_err RequestCoefficients(double* const p_coef_a, double* const p_coef_b, double* const p_coef_c) {    //TODO any parameters count
    assert(p_coef_a != NULL);
    assert(p_coef_b != NULL);
    assert(p_coef_c != NULL);
    assert(p_coef_a != p_coef_b && p_coef_a != p_coef_c && p_coef_b != p_coef_c);

    while (1) {                          // request the introduction of coefficients unless input is correct or program is closed
        printf("Enter the coefficients coef_a, coef_b, coef_c like: " Color("1 2 1", MAG) "\n");

        if (scanf("%lf %lf %lf", p_coef_a, p_coef_b, p_coef_c) != 3) {
            ClearInputBuf();        // clear the input buffer from excess symbols

            printf(Color("RequestCoefficients:", YEL) "There must be three coefficients like: "
                   Color("1 2 1", MAG) "\n");
            printf("Do you want to try again? (write " Color("Y", MAG) " if you do or "
                   Color("N", MAG) " otherwise)\n");

            Answer ask_result = AskYesOrNo();
            if (ask_result == YES)
                continue;
            else if (ask_result == NO)
                return REQ_STOP_PROGRAM;
            else
                return REQ_INPUT_ERROR;

        }
        else /* number of arguments is 3 => input is correct */
            break;
    }

    return REQ_NORMAL;
}


Answer AskYesOrNo() {
    char c;
    Answer result = NONE;
    bool clear = true;

    while (1) {                                                 // request answer unless it is correct
        result = NONE;
        clear = true;
        while (((c = getchar()) == ' ') || (c == '\t'));         // skip spaces

        if (c == 'Y')
            result = YES;
        else if (c  == 'N')
            result = NO;
        else if ((c == '\n') || (c == EOF))
            clear = false;                                      // buffer is empty
        if (result != NONE) {
            while (((c = getchar()) == ' ') || (c == '\t'));     // skip spaces
            if ((c == '\n') || (c == EOF))
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


Roots_c SolveQuadratic(const double coef_a, const double coef_b, const double coef_c,
                       double* const p_root1, double* const p_root2) {
    assert(p_root1 != NULL);
    assert(p_root2 != NULL);
    assert(p_root1 != p_root2);

    if (CmpEpsPrec(coef_a, 0.0)) {
        return SolveLinear(coef_b, coef_c, p_root1);
    }
    else { /* (coef_a != 0.0) => it's quadric education */
        double discr = coef_b*coef_b - 4*coef_a*coef_c;
        if (CmpEpsPrec(discr, 0.0)) {
            *p_root1 = DivideSmart(-coef_b, 2*coef_a);
            return ONE_ROOT;
        }
        else if (discr > 0.0) {
            double sq_discr = sqrt(discr);
            *p_root1 = DivideSmart(-coef_b + sq_discr, 2*coef_a);
            *p_root2 = DivideSmart(-coef_b - sq_discr, 2*coef_a);
            return TWO_ROOTS;
        }
        else { /* (discr < 0.0) => there is no solutions */
            return NO_ROOTS;
        }
    }
}


Roots_c SolveLinear(const double coef_a, const double coef_b, double* const p_root) {
    assert(p_root != NULL);

    if (CmpEpsPrec(coef_a, 0.0)) {
        if (CmpEpsPrec(coef_b, 0.0))
            return INF_ROOTS;
        else /* (coef_c != 0.0) */
            return NO_ROOTS;
    }
    else { /* (coef_b != 0.0) */
        *p_root = DivideSmart(-coef_b, coef_a);
        return ONE_ROOT;
    }
}


double DivideSmart(const double dividend, const double divider) {
    assert(divider != 0.0);

    if (!CmpEpsPrec(dividend, 0.0))
        return dividend / divider;
    else
        return 0.0;
}


bool CmpEpsPrec(const double a, const double b) {
    if (((a <= b + EPSILON) && (a >= b - EPSILON)) || (isnan(a) && isnan(b)))
        return true;
    else
        return false;
}


Print_err PrintRoots(Roots_c n_roots, const double root_1, const double root_2) {
    if(CmpEpsPrec(n_roots, TWO_ROOTS) && CmpEpsPrec(root_1, root_2))
        n_roots = ONE_ROOT;

    switch(n_roots) {
        case INF_ROOTS:
            printf("Infinitely many solutions\n");
            break;
        case NO_ROOTS:
            printf("There isn't any solution\n");
            break;
        case ONE_ROOT:
            printf("There is only solution x1: " Color("%.5lf", MAG) "\n", root_1);
            break;
        case TWO_ROOTS:
            printf("There are two solutions x1: " Color("%.5lf", MAG) "; x2: " Color("%.5lf", MAG) "\n", root_1, root_2);
            break;
        default:
            return PRINT_NROOT_ERROR;
    }

    return PRINT_NORMAL;
}
