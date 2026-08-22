#define MAX_TEST_LINE_LONG 200

typedef enum {FTEST_EXP_NROOT_ERROR = -1, FTEST_INPUT_ERROR = -2,
              FTEST_FOPEN_ERROR = -3, FTEST_READ_ERROR = -4} Ftest_err;  // return of RunFileTests()

/* PrintErrorMessage: print a message depending on error_type.
Return error_type. */
int PrintErrorMessage(const Ftest_err error_type, const int test_number);

/* RunTest: do test SolveQuadratic() with values from parameters.
Return 1 if test is successful or 0 otherwise. */
int RunTest(const double coef_a, const double coef_b, const double coef_c, const int exp_n_roots,
            const double exp_root_1, const double exp_root_2, const int test_number);

/* Swap: swap a from p_a and b from_p_b. */
void Swap(double* const p_a, double* const p_b);

/* PrintFailed: print field report. */
void PrintFailed(const double coef_a, const double coef_b, const double coef_c,
                 const int n_roots, const double root_1, const double root_2,
                 const int exp_n_roots, const double exp_root_1, const double exp_root_2,
                 const bool n_roots_conf, const bool root_1_conf, const bool root_2_conf, const int test_number);

/* RunFileTests: open file and do tests from it.
Return percent of successful tests or negative code of error if it have occurred. */
int RunFileTests() {
    FILE* tests_file = fopen("tests_for_SolveQuadratic.txt", "r");

    if (tests_file == NULL) {
        fclose(tests_file);
        return PrintErrorMessage(FTEST_FOPEN_ERROR, 0);
    }

    int successful_tests = 0, tests_count = 0;

    double coef_a = NAN, coef_b = NAN, coef_c = NAN;
    double exp_root_1 = NAN, exp_root_2 = NAN;
    int exp_n_roots = (int)NO_ROOTS;

    char line[MAX_TEST_LINE_LONG];
    int symbols_read = 0;

    while (fgets(line, sizeof(line), tests_file) != NULL) {
        tests_count++;

        exp_root_1 = NAN;
        exp_root_2 = NAN;
        symbols_read = 0;

        if (sscanf(line, "%lf %lf %lf %d%n", &coef_a, &coef_b, &coef_c, &exp_n_roots, &symbols_read) != 4) {
            fclose(tests_file);
            return PrintErrorMessage(FTEST_INPUT_ERROR, tests_count);
        }

        if (exp_n_roots == (int)ONE_ROOT) {
            if (sscanf(line + symbols_read, "%lf", &exp_root_1) != 1) {
                fclose(tests_file);
                return PrintErrorMessage(FTEST_INPUT_ERROR, tests_count);
            }
        }
        else if (exp_n_roots == (int)TWO_ROOTS) {
            if (sscanf(line + symbols_read, "%lf %lf", &exp_root_1, &exp_root_2) != 2) {
                fclose(tests_file);
                return PrintErrorMessage(FTEST_INPUT_ERROR, tests_count);
            }
        }
        else if ((exp_n_roots != (int)NO_ROOTS) && (exp_n_roots != (int)INF_ROOTS)) {
            fclose(tests_file);
            return PrintErrorMessage(FTEST_EXP_NROOT_ERROR, tests_count);
        }

        successful_tests += RunTest(coef_a, coef_b, coef_c, exp_n_roots, exp_root_1, exp_root_2, tests_count);
    }

    if (ferror(tests_file)) {
        fclose(tests_file);
        return PrintErrorMessage(FTEST_FOPEN_ERROR, 0);
    }

    fclose(tests_file);
    return successful_tests * 100 / tests_count;
}


int PrintErrorMessage(const Ftest_err error_type, const int test_number) {
    if (error_type == FTEST_FOPEN_ERROR) {
        printf("File " Color("tests_for_SolveQuadratic.txt", YEL) " doesn't exist or you don't have \"r\" permission\n");
    }
    if (error_type == FTEST_INPUT_ERROR) {
        printf(Color("RunFileTest", YEL) ": " Color("%tests_for_SolveQuadratic.txt", YEL) ": test "
               Color("%d", MAG) ": There must be four - six numbers like: " Color("1 2 1 1 -1", MAG) "\n", test_number);
    }
    if (error_type == FTEST_EXP_NROOT_ERROR) {
        printf(Color("RunFileTest", YEL) ": " Color("%tests_for_SolveQuadratic.txt", YEL) ": test "
               Color("%d", MAG) ": There must be value of type Roots_c like: " Color("1 2 1 >1< -1", MAG) "\n", test_number);
    }
    if (error_type == FTEST_READ_ERROR) {
        printf("File " Color("tests_for_SolveQuadratic.txt", YEL) " - error during the reading\n");
    }

    return (int)error_type;
}


int RunTest(const double coef_a, const double coef_b, const double coef_c, const int exp_n_roots,
            const double exp_root_1, const double exp_root_2, const int test_number) {
    double root_1 = NAN, root_2 = NAN;
    Roots_c n_roots = SolveQuadratic(coef_a, coef_b, coef_c, &root_1, &root_2);
    bool n_roots_conf = false, root_1_conf = false, root_2_conf = false;

    if ((int)n_roots == exp_n_roots)
        n_roots_conf = true;

    if (CmpEpsPrec(root_1, exp_root_1))
        root_1_conf = true;
    else if (!CmpEpsPrec(root_2, exp_root_2)){        // if root_2 == exp_root_2 there is no swap needed
        Swap(&root_1, &root_2);                       //check second root it,s necessary to correct print FAILED
        if (CmpEpsPrec(root_1, exp_root_1))
            root_1_conf = true;
    }

    if (CmpEpsPrec(root_2, exp_root_2))
        root_2_conf = true;

    if (n_roots_conf && root_1_conf && root_2_conf)
        return 1;
    else { /* discrepancy between exp and got */
        PrintFailed(coef_a, coef_b, coef_c, n_roots, root_1, root_2, exp_n_roots, exp_root_1, exp_root_2,
                    n_roots_conf, root_1_conf, root_2_conf, test_number);
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


void PrintFailed(const double coef_a, const double coef_b, const double coef_c,
                 const int n_roots, const double root_1, const double root_2,
                 const int exp_n_roots, const double exp_root_1, const double exp_root_2,
                 const bool n_roots_conf, const bool root_1_conf, const bool root_2_conf, const int test_number) {
    printf("Test " Color("%d", MAG) " " Color("FAILED", RED) "\n", test_number);

    printf("coefficients: a = " Color("%.5lf", MAG) "; b = " Color("%.5lf", MAG) "; c = " Color("%.5lf", MAG) "\n",
           coef_a, coef_b, coef_c);

    printf("expected: n_roots code = " Color("%-2d", MAG) "; ", exp_n_roots);

    printf("x1 = ");
    if (isnan(exp_root_1))
        printf(Color("NAN        ", MAG));
    else
        printf(Color("%-11.5lf", MAG), exp_root_1);
    printf("; ");

    printf("x2 = ");
    if (isnan(exp_root_2))
        printf(Color("NAN        ", MAG));
    else
        printf(Color("%-11.5lf", MAG), exp_root_2);
    printf("\n");

    printf("got     : n_roots code = ");
    printf("%s", n_roots_conf?MAG:RED);    // select color
    printf("%-2d", (int)n_roots);
    printf(END);                           // reset color
    printf("; ");

    printf("x1 = ");
    printf("%s", root_1_conf?MAG:RED);    // select color
    if (isnan(root_1))
        printf("NAN        ");
    else
        printf("%-11.5lf", root_1);
    printf(END);                          // reset color
    printf("; ");

    printf("x2 = ");
    printf("%s", root_2_conf?MAG:RED);    // select color
    if (isnan(root_2))
        printf("NAN        ");
    else
        printf("%-11.5lf", root_2);
    printf(END);                          // reset color
    printf("\n");
}
