#include "title.c"
#include "UnitTest_for_SolveQuadratic.c"
#include "input_functions.c"
#include "calculation_functions.c"

int main(const int argc, const char* argv[]) {
    Flags command_line_flags = HandleArg(argc, argv);

    if (command_line_flags.preliminary_testing_flag)
        (void)RunFileTests();

    Quadratic_coefficients coef = {NAN, NAN, NAN};
    (void)GetCoefficients(&coef, command_line_flags);

    double root_1 = NAN, root_2 = NAN;
    Roots_c_t n_roots = SolveQuadratic(coef, &root_1, &root_2);

    (void)PrintRoots(n_roots, root_1, root_2);

    return MAIN_NORMAL;
}


Flags HandleArg(const int argc, const char* argv[]) {
    Flags command_line_flags = {};

    for (int arg_index = 1; arg_index < argc; arg_index++) {
        const char* arg_position = argv[arg_index];
        if (*arg_position++ != '-')
            continue;

        while (isalpha(*arg_position)) {
            if (*arg_position == 'f')
                command_line_flags.file_input_flag = 1;
            if (*arg_position == 'c')
                command_line_flags.console_input_flag = 1;
            if (*arg_position == 't')
                command_line_flags.preliminary_testing_flag = 1;

            arg_position++;
        }
    }

    return command_line_flags;
}


Print_err_t PrintRoots(Roots_c_t n_roots, const double root_1, const double root_2) {
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
            printf("PrintRoots: Unexpected value of n_roots\n");
            exit(PRINT_NROOT_ERROR);
    }

    return PRINT_NORMAL;
}
