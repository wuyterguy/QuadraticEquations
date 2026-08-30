#include "title.c"
#include "UnitTest_for_SolveQuadratic.c"
#include "input_functions.c"
#include "calculation_functions.c"
#include "draw_functions.c"

int main(const int argc, const char* argv[]) {
    Flags command_line_flags = {};
    HandleArg(argc, argv, &command_line_flags);

    char output_str[MAX_OUTOUT_LENGTH];

    if (command_line_flags.show_helplist_flag) {
        (void)ShowHelplist(&command_line_flags);
        return MAIN_NORMAL;
    }

    if (command_line_flags.preliminary_testing_flag)
        (void)RunFileTests(&command_line_flags);

    Quadratic_coefficients coef = {NAN, NAN, NAN};
    (void)GetCoefficients(&coef, &command_line_flags);

    double root_1 = NAN, root_2 = NAN;
    Roots_c_t n_roots = SolveQuadratic(&coef, &root_1, &root_2);

    (void)PrintRoots(n_roots, root_1, root_2, &command_line_flags);

    if (command_line_flags.draw_graphic_flag) {
        sprintf(output_str, "Press " Color("Enter", MAG) " to see graphic\n");
        PrintAnimated(output_str, &command_line_flags);
        getchar();

        DrawParabola(&coef);
    }

    return MAIN_NORMAL;
}


void HandleArg(const int argc, const char* argv[], Flags* command_line_flags) {
    for (int arg_index = 1; arg_index < argc; arg_index++) {
        const char* arg_position = argv[arg_index];
        if (*arg_position++ != '-')
            continue;

        while (isalpha(*arg_position)) {
            switch (*arg_position) {
                case 'f': command_line_flags->file_input_flag = 1;          break;
                case 'c': command_line_flags->console_input_flag = 1;       break;
                case 't': command_line_flags->preliminary_testing_flag = 1; break;
                case 'h': command_line_flags->show_helplist_flag = 1;       break;
                case 'g': command_line_flags->draw_graphic_flag = 1;        break;
                case 'a': command_line_flags->animated_output_flag = 1;     break;
                default: break;
            }

            arg_position++;
        }
    }
}


Help_err_t ShowHelplist(const Flags* command_line_flags) {
    char output_str[MAX_OUTOUT_LENGTH];

    FILE* help_file = fopen(HELPFILE, "r");

    if (help_file == NULL){
        sprintf(output_str, "File " Color(HELPFILE, YEL) " doesn't exist or you don't have \"r\" permission\n");
        PrintAnimated(output_str, command_line_flags);
        exit(HELP_FOPEN_ERROR);
    }

    int current_symbol = 0;
    while (((current_symbol = getc(help_file)) != '\0') && (current_symbol != EOF)) {
        putchar(current_symbol);
        if (command_line_flags->animated_output_flag)
            txSleep(OUTPUT_FAST_DELAY);
    }

    if (ferror(help_file)){
        sprintf(output_str, "File " Color(HELPFILE, YEL) " - error during the reading\n");
        PrintAnimated(output_str, command_line_flags);
        fclose(help_file);
        exit(HELP_READ_ERROR);
    }

    return HELP_NORMAL;
}


Print_err_t PrintRoots(Roots_c_t n_roots, const double root_1, const double root_2,
                       const Flags* command_line_flags) {
    assert(command_line_flags != NULL);

    char output_str[MAX_OUTOUT_LENGTH];

    if (CmpEpsPrec(n_roots, TWO_ROOTS) && CmpEpsPrec(root_1, root_2))
        n_roots = ONE_ROOT;

    switch (n_roots) {
        case INF_ROOTS:
            sprintf(output_str, "Infinitely many solutions\n");
            PrintAnimated(output_str, command_line_flags);
            break;
        case NO_ROOTS:
            sprintf(output_str, "There isn't any solution\n");
            PrintAnimated(output_str, command_line_flags);
            break;
        case ONE_ROOT:
            sprintf(output_str, "There is only solution x1: " Color("%.5lf", MAG) "\n", root_1);
            PrintAnimated(output_str, command_line_flags);
            break;
        case TWO_ROOTS:
            sprintf(output_str, "There are two solutions x1: " Color("%.5lf", MAG) "; x2: " Color("%.5lf", MAG) "\n", root_1, root_2);
            PrintAnimated(output_str, command_line_flags);
            break;
        default:
            sprintf(output_str, "PrintRoots: Unexpected value of n_roots\n");
            PrintAnimated(output_str, command_line_flags);
            exit(PRINT_NROOT_ERROR);
            break;
    }

    return PRINT_NORMAL;
}


void PrintAnimated(const char* output_str, const Flags* command_line_flags) {
    const char* current_symbol = output_str;
    while ((*current_symbol != '\0') && (*current_symbol != EOF)) {
        if (*current_symbol == '\x1b') {
            do {
                putchar(*current_symbol);
            } while (*current_symbol++ != 'm');
            continue;
        }

        putchar(*current_symbol++);
        if (command_line_flags->animated_output_flag)
            txSleep(OUTPUT_FAST_DELAY);
    }
}
