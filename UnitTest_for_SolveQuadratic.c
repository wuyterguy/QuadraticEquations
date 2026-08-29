int RunFileTests(const Flags* command_line_flags) {
    assert(command_line_flags != NULL);

    char output_str[MAX_OUTOUT_LENGTH];

    FILE* tests_file = fopen(TESTFILE, "r");

    if (tests_file == NULL) {
        fclose(tests_file);
        PrintErrorMessage(FTEST_FOPEN_ERROR, 0, command_line_flags);
        exit(FTEST_FOPEN_ERROR);
    }

    Ftest_err_t error_type = FTEST_NORMAL;

    int successful_tests = 0, tests_count = 0;

    Quadratic_coefficients coef = {NAN, NAN, NAN};
    Expected_sq_result exp = {(int)NO_ROOTS, NAN, NAN};

    char line[MAX_TEST_LINE_LENGTH] = {};
    int symbols_read = 0;

    while (fgets(line, sizeof(line), tests_file) != NULL) {
        tests_count++;

        exp.root_1 = NAN;
        exp.root_2 = NAN;
        symbols_read = 0;

        if (sscanf(line, "%lf %lf %lf %d%n", &coef.a, &coef.b, &coef.c, &exp.n_roots, &symbols_read) != 4) {
            error_type = FTEST_INPUT_ERROR;
            break;
        }

        if (exp.n_roots == (int)ONE_ROOT) {
            if (sscanf(line + symbols_read, "%lf", &exp.root_1) != 1) {
                error_type = FTEST_INPUT_ERROR;
                break;
            }
        }
        else if (exp.n_roots == (int)TWO_ROOTS) {
            if (sscanf(line + symbols_read, "%lf %lf", &exp.root_1, &exp.root_2) != 2) {
                error_type = FTEST_INPUT_ERROR;
                break;
            }
        }
        else if ((exp.n_roots != (int)NO_ROOTS) && (exp.n_roots != (int)INF_ROOTS)) {
            error_type = FTEST_EXP_NROOT_ERROR;
            break;
        }

        successful_tests += RunTest(&coef, exp, tests_count);
    }

    if (ferror(tests_file))
        error_type = FTEST_READ_ERROR;

    fclose(tests_file);
    if (error_type != FTEST_NORMAL) {
        PrintErrorMessage(error_type, tests_count, command_line_flags);
        exit(error_type);
    }

    int ftest_result = successful_tests * HUNDRED_PERCENT / tests_count;
    if (ftest_result < HUNDRED_PERCENT) {
        sprintf(output_str, "SolveQuadratic: testing failed\n"
               Color("%d%%", MAG) " successful file tests\n", ftest_result);
        PrintAnimated(output_str, command_line_flags);
        exit(MAIN_NORMAL);
    }

    return FTEST_NORMAL;
}


void PrintErrorMessage(const Ftest_err_t error_type, const int test_number,
                       const Flags* command_line_flags) {
    assert(command_line_flags != NULL);

    char output_str[MAX_OUTOUT_LENGTH];

    if (error_type == FTEST_FOPEN_ERROR) {
        sprintf(output_str, "File " Color(TESTFILE, YEL) " doesn't exist or you don't have \"r\" permission\n");
        PrintAnimated(output_str, command_line_flags);
    }
    if (error_type == FTEST_INPUT_ERROR) {
        sprintf(output_str, Color("RunFileTest", YEL) ": " Color(TESTFILE, YEL) ": test "
               Color("%d", MAG) ": There must be four - six numbers like: " Color("1 2 1 1 -1", MAG) "\n", test_number);
        PrintAnimated(output_str, command_line_flags);
    }
    if (error_type == FTEST_EXP_NROOT_ERROR) {
        sprintf(output_str, Color("RunFileTest", YEL) ": " Color(TESTFILE, YEL) ": test "
               Color("%d", MAG) ": There must be value of type Roots_c_t like: " Color("1 2 1 >1< -1", MAG) "\n", test_number);
        PrintAnimated(output_str, command_line_flags);
    }
    if (error_type == FTEST_READ_ERROR) {
        sprintf(output_str, "File " Color(TESTFILE, YEL) " - error during the reading\n");
        PrintAnimated(output_str, command_line_flags);
    }
}


int RunTest(const Quadratic_coefficients* coef, const Expected_sq_result exp, const int test_number) {
    assert(coef != NULL);

    double root_1 = NAN, root_2 = NAN;
    Roots_c_t n_roots = SolveQuadratic(coef, &root_1, &root_2);

    Conformity_sq_result conf {false, false, false};

    if ((int)n_roots == exp.n_roots)
        conf.n_roots = true;

    if (CmpEpsPrec(root_1, exp.root_1))
        conf.root_1 = true;
    else if (!CmpEpsPrec(root_2, exp.root_2)){        // if root_2 == exp.root_2 there is no swap needed
        Swap(&root_1, &root_2);                       // check second root it,s necessary to correct print FAILED
        if (CmpEpsPrec(root_1, exp.root_1))
            conf.root_1 = true;
    }

    if (CmpEpsPrec(root_2, exp.root_2))
        conf.root_2 = true;

    if (conf.n_roots && conf.root_1 && conf.root_2)
        return 1;
    else { /* discrepancy between exp and got */
        PrintFailed(coef, n_roots, root_1, root_2, exp, conf, test_number);
        return 0;
    }
}


void Swap(double* const p_a, double* const p_b) {
    assert(p_a != NULL);
    assert(p_b != NULL);
    assert(p_a != p_b);

    double mem = *p_a;
    *p_a = *p_b;
    *p_b = mem;
}


void PrintFailed(const Quadratic_coefficients* coef, const int n_roots, const double root_1, const double root_2,
                 const Expected_sq_result exp, const Conformity_sq_result conf, const int test_number) {
    assert(coef != NULL);

    printf("Test " Color("%d", MAG) " " Color("FAILED", RED) "\n", test_number);

    printf("coefficients: a = " Color("%.5lf", MAG) "; b = " Color("%.5lf", MAG) "; c = " Color("%.5lf", MAG) "\n",
           coef->a, coef->b, coef->c);

    printf("expected: n_roots code = " Color("%-2d", MAG) "; ", exp.n_roots);

    printf("x1 = ");
    if (IsNAN(exp.root_1))
        printf(Color("NAN        ", MAG));
    else
        printf(Color("%-11.5lf", MAG), exp.root_1);
    printf("; ");

    printf("x2 = ");
    if (IsNAN(exp.root_2))
        printf(Color("NAN        ", MAG));
    else
        printf(Color("%-11.5lf", MAG), exp.root_2);
    printf("\n");

    printf("got     : n_roots code = ");
    printf("%s", conf.n_roots?MAG:RED);    // select color
    printf("%-2d", (int)n_roots);
    printf(END);                           // reset color
    printf("; ");

    printf("x1 = ");
    printf("%s", conf.root_1?MAG:RED);    // select color
    if (IsNAN(root_1))
        printf("NAN        ");
    else
        printf("%-11.5lf", root_1);
    printf(END);                          // reset color
    printf("; ");

    printf("x2 = ");
    printf("%s", conf.root_2?MAG:RED);    // select color
    if (IsNAN(root_2))
        printf("NAN        ");
    else
        printf("%-11.5lf", root_2);
    printf(END);                          // reset color
    printf("\n");

    printf("\n");
}
