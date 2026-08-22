#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <math.h>
#include <ctype.h>

#define MAX_FILENAME_LONG 100
#define EPSILON 0.00001                                                           // comparison accuracy
#define MAX_TEST_LINE_LONG 200
#define PERCENT 100

enum {MAIN_NORMAL, MAIN_FILE_ERROR, MAIN_INPUT_ERROR,
      MAIN_NROOT_ERROR, MAIN_TESTING_ERROR};                                      // return of main()
typedef enum {FTEST_EXP_NROOT_ERROR = -1, FTEST_INPUT_ERROR = -2,
              FTEST_FOPEN_ERROR = -3, FTEST_READ_ERROR = -4}           Ftest_err; // return of RunFileTests()
typedef enum {FILE_INPUT, CONSOLE_INPUT, ERROR}                       Input_type; // return of AskInputType()
typedef enum {NO, YES, NONE}                                              Answer; // return of AskYesOrNo())
typedef enum {FREQ_NORMAL, FREQ_STOP_PROGRAM, FREQ_INPUT_ERROR,
              FREQ_FOPEN_ERROR, FREQ_READ_ERROR}                        Freq_err; // return of FileRequestCoefficients()
typedef enum {REQ_NORMAL, REQ_STOP_PROGRAM,
              REQ_INPUT_ERROR}                                           Req_err; // return of RequestCoefficients()
typedef enum {NO_ROOTS = 0, ONE_ROOT = 1, TWO_ROOTS = 2, INF_ROOTS = -2} Roots_c; // return od Solve___()
typedef enum {PRINT_NORMAL, PRINT_NROOT_ERROR}                         Print_err; // return of PrintRoots()

#define ON  1
#define OFF 0

#define COLORSWITCH ON                                                            // switch color output

#if COLORSWITCH == ON
    #define RED "\x1b[31m"
    #define BLU "\x1b[34m"
    #define GRE "\x1b[32m"
    #define YEL "\x1b[33m"
    #define MAG "\x1b[35m"
    #define END "\x1b[0m"

    /* Color: is replaced by s colored in color */
    #define Color(s, color) color s END
#else
    #define Color(s, color) s
#endif

#undef assert

//#define NDEBUG                                                                // switch assert off

#ifndef NDEBUG
    #define assert(condition)                                                                          \
    if(!(condition)) {                                                                                 \
        fprintf(stderr, "Assertion failed: " #condition ", file %s, line %d", __FILE__, __LINE__);     \
        abort();                                                                                       \
    }
#else
    #define assert(condition)
#endif


/* RunFileTests: open file and do tests from it.
Return percent of successful tests or negative code of error if it have occurred. */
int RunFileTests(void);

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


/* AskInputType: ask the user to choose input type.
Return FILE_INPUT or CONSOLE_INPUT or ERROR if there is error. */
Input_type AskInputType(void);

/* FileRequestCoefficients: transmit three values from coefficients.txt to addresses pcoef_a, pcoef_b, pcoef_c.
Return 0 if everything is correct or 1 if there is file read error or 2 if there is fscanf error. */
Freq_err FileRequestCoefficients(double* const p_coef_a, double* const p_coef_b, double* const p_coef_c);

/* Ask the user to write name of file until this is in correct form.
Return pointer to begining of string with filename. */
char* GetFilename(void);

/* RequestCoefficients: transmit three values from input to addresses pcoef_a, pcoef_b, pcoef_c.
Return 1 if the user wants to close the program or 0 otherwise or 2 if there is error. */
Req_err RequestCoefficients(double* const p_coef_a, double* const p_coef_b, double* const p_coef_c);

/* AskYesOrNo: ask the user to write Y or N unless he do it.
Return 1 if answer is Y or 0 if answer is N. */
Answer AskYesOrNo(void);

/* ClearInputBuf: clear the input buffer from excess symbols.
Buffer mustn't be empty. */
void ClearInputBuf(void);


/* SolveQuadratic: find the solution of quadratic equation with coefficients coef_a, coef_b, coef_c
and write them to the address proot1, proot2.
If equation has only one solution, it will be written to addres proot1.
Return the number of solutions or INF_ROOTS if there are infinite. */
Roots_c SolveQuadratic(const double coef_a, const double coef_b, const double coef_c, double* const p_root1, double* const p_root2);

/* SolveLinear: find the solution of linear equation with coefficients coef_a, coef_b
and write them to the address proot.
Return the number of solutions or INF_ROOTS if there are infinite. */
Roots_c SolveLinear(const double coef_a, const double coef_b, double* const p_root);

/* DivideSmart: return the quotient of dividend/divider preventing the return of -0.0 */
double DivideSmart(const double dividend, const double divider);

/* CmpEpsPrec: compares roots a and b taking EPSILON as accuracy into account.
Return 1 if (a = b) || (a = NAN && b = NAN) or 0 otherwise. */
bool CmpEpsPrec(const double a, const double b);


/* PrintRoots: print roots depending on the value of n_roots.
Return 1 if everything is correct or 0 otherwise. */
Print_err PrintRoots(Roots_c n_roots, const double root_1, const double root_2);
