#include "title.c"
#include "UnitTest_for_SolveQuadratic.c"
#include "input_functions.c"
#include "calculation_functions.c"

int main(const int argc, const char* argv[]) {
    HandleArg(argc, argv);

    if (COMMAND_LINE_FLAGS.PRELIMINARY_TESTING_FLAG) {
        int ftest_res = RunFileTests();
        if (ftest_res < 0)
            return MAIN_TESTING_ERROR;

        if (ftest_res < 100) { //TODO mac
            printf(Color("%d%%", MAG) " successful file tests\n", ftest_res);
            printf("SolveQuadratic: test failed\n");
            return MAIN_NORMAL;
        }
    }

    printf("Quadratic equations solver by " Color("Zotov Anton", YEL) "\n");

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


void HandleArg(const int argc, const char* argv[]) {
    for (int arg_index = 1; arg_index < argc; arg_index++) {
        const char* arg_position = argv[arg_index];
        if (*arg_position++ != '-')
            continue;

        while (isalpha(*arg_position)) {
            if (*arg_position == 'f')
                COMMAND_LINE_FLAGS.FILE_INPUT_FLAG = 1;
            if (*arg_position == 'c')
                COMMAND_LINE_FLAGS.CONSOLE_INPUT_FLAG = 1;
            if (*arg_position == 't')
                COMMAND_LINE_FLAGS.PRELIMINARY_TESTING_FLAG = 1;

            arg_position++;
        }
    }
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
