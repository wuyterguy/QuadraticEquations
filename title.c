#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define MAX_TEST_LINE_LONG 200
#define HUNDRED_PERCENT 100
#define TESTFILE "tests_for_SolveQuadratic.txt"
#define MAX_FILENAME_LONG 100
#define EPSILON 0.00001                                                           // comparison accuracy

enum {MAIN_NORMAL, MAIN_FILE_ERROR, MAIN_INPUT_ERROR,
      MAIN_NROOT_ERROR, MAIN_TESTING_ERROR};                                      // return of main()
typedef enum {FTEST_NORMAL = 0, FTEST_EXP_NROOT_ERROR = -1, FTEST_INPUT_ERROR = -2,
              FTEST_FOPEN_ERROR = -3, FTEST_READ_ERROR = -4}           Ftest_err; // return of RunFileTests()
typedef enum {FILE_INPUT, CONSOLE_INPUT, TYPE_ERROR}                  Input_type; // return of AskInputType()
typedef enum {NO = 0, YES = 1, NONE = 2}                                  Answer; // return of AskYesOrNo())
typedef enum {FREQ_NORMAL, FREQ_STOP_PROGRAM, FREQ_INPUT_ERROR,
              FREQ_FOPEN_ERROR, FREQ_READ_ERROR}                        Freq_err; // return of FileRequestCoefficients()
typedef enum {REQ_NORMAL, REQ_STOP_PROGRAM,
              REQ_INPUT_ERROR}                                           Req_err; // return of RequestCoefficients()
typedef enum {NO_ROOTS = 0, ONE_ROOT = 1, TWO_ROOTS = 2, INF_ROOTS = -2} Roots_c; // return od Solve___()
typedef enum {PRINT_NORMAL, PRINT_NROOT_ERROR}                         Print_err; // return of PrintRoots()


#define ON  1
#define OFF 0

#define COLOR_SWITCH ON                                                            // switch color output

#define SCREEN_CLEAR_SWITCH ON                                                     // switch clearing screen

#if COLOR_SWITCH == ON
    #define RED "\x1b[31m"
    #define BLU "\x1b[34m"
    #define GRE "\x1b[32m"
    #define YEL "\x1b[33m"
    #define MAG "\x1b[35m"
    #define END "\x1b[0m"

    /* Color: is replaced by s colored in color */
    #define Color(s, color) color s END
#else
    #define RED ""
    #define BLU ""
    #define GRE ""
    #define YEL ""
    #define MAG ""
    #define END ""

    #define Color(s, color) s
#endif

#if SCREEN_CLEAR_SWITCH == ON
    #define clear_to_line(line) printf("\e[;1H\e[2J");
#else
    #define clear_to_line(line)
#endif

#undef assert

#define NDEBUG                                                                // switch assert off

#ifndef NDEBUG
    #define assert(condition)                                                                          \
    if(!(condition)) {                                                                                 \
        fprintf(stderr, "Assertion failed: " #condition ", file %s, line %d", __FILE__, __LINE__);     \
        abort();                                                                                       \
    }
#else
    #define assert(condition)
#endif

struct {
    unsigned int FILE_INPUT_FLAG : 1;
    unsigned int CONSOLE_INPUT_FLAG : 1;
    unsigned int PRELIMINARY_TESTING_FLAG : 1;
} COMMAND_LINE_FLAGS = {};
//char COMMAND_LINE_FLAGS_LIST[] = "fct";

struct Expected_sq_result {int n_roots; double root_1; double root_2;};
struct Conformity_sq_result {bool n_roots, root_1, root_2;};


/**
 * @brief get command line flags from command line arguments
 *
 * @param[in] argc argc from command line
 * @param[in] agrv argv from command line
 */
void HandleArg(const int argc, const char* argv[]);


/**
 * @brief open file and do tests from it
 *
 * @return percent of successful tests or negative code of error if it have occurred
 */
int RunFileTests(void);

/**
 * @brief print a message depending on error_type
 *
 * @param[in] error_type  error number
 * @param[in] test_number number of failed test
 *
 * @return error_type converted to int
 */
int PrintErrorMessage(const Ftest_err error_type, const int test_number);

/**
 * @brief do test of SolveQuadratic() using values from parameters
 *
 * @param[in] coef_a      coefficient a of quadratic equation
 * @param[in] coef_b      coefficient b of quadratic equation
 * @param[in] coef_c      coefficient c of quadratic equation
 * @param[in] exp         structure containing expected values
 * @param[in] test_number number of current test
 *
 * @return 1 if test is successful or 0 otherwise
 */
int RunTest(const double coef_a, const double coef_b, const double coef_c,
            const Expected_sq_result exp, const int test_number);

/**
 * @brief swap a from p_a and b from_p_b
 *
 * @param[in, out] p_a pointer to a
 * @param[in, out] p_b pointer to b
 */
void Swap(double* const p_a, double* const p_b);

/**
 * @brief print failed report
 *
 * @param[in] coef_a  coefficient a of quadratic equation
 * @param[in] coef_b  coefficient b of quadratic equation
 * @param[in] coef_c  coefficient c of quadratic equation
 * @param[in] n_roots number of roots
 * @param[in] roor_1  first root of quadratic equation
 * @param[in] roor_2  second root of quadratic equation
 * @param[in] exp     structure containing expected values
 * @param[in] conf    structure containing information about conformity between got and expected values
 */
void PrintFailed(const double coef_a, const double coef_b, const double coef_c,
                 const int n_roots, const double root_1, const double root_2,
                 const Expected_sq_result exp, const Conformity_sq_result conf, const int test_number);


/**
 * @brief ask the user to choose input type
 *
 * @return FILE_INPUT or CONSOLE_INPUT or TYPE_ERROR if there is error
 */
Input_type AskInputType(void);

/**
 * @brief transmit three values from coefficients.txt to addresses pcoef_a, pcoef_b, pcoef_c
 *
 * @param[out] p_coef_a pointer to coefficient a of quadratic equation
 * @param[out] p_coef_b pointer to coefficient b of quadratic equation
 * @param[out] p_coef_c pointer to coefficient c of quadratic equation
 *
 * @return FREQ_NORMAL or error number
 */
Freq_err FileRequestCoefficients(double* const p_coef_a, double* const p_coef_b, double* const p_coef_c);

/**
 * @brief Ask the user to write name of file until this is in correct form.
 *
 * @return filename
 */
char* GetFilename(void);

/**
 * @brief transmit three values from input to addresses pcoef_a, pcoef_b, pcoef_c
 *
 * @param[out] p_coef_a pointer to coefficient a of quadratic equation
 * @param[out] p_coef_b pointer to coefficient b of quadratic equation
 * @param[out] p_coef_c pointer to coefficient c of quadratic equation
 *
 * @return REQ_NORMAL or error number
 */
Req_err RequestCoefficients(double* const p_coef_a, double* const p_coef_b, double* const p_coef_c);

/**
 * @brief ask the user to write Y or N unless he do it
 *
 * @return YES if answer is Y or  if answer is N
 */
Answer AskYesOrNo(void);

/**
 * @brief clear the input buffer from excess symbols
 *
 * @note Buffer mustn't be empty
 */
void ClearInputBuf(void);


/**
 * @brief find the solution of quadratic equation with coefficients coef_a, coef_b, coef_c
 *        and write them to the address proot1, proot2
 * @param[in] coef_a   coefficient a of quadratic equation
 * @param[in] coef_b   coefficient b of quadratic equation
 * @param[in] coef_c   coefficient c of quadratic equation
 * @param[out] p_roor_1 pointer to first root of quadratic equation
 * @param[out] p_roor_2 pointer to second root of quadratic equation
 *
 * @return the number of solutions or INF_ROOTS if there are infinite
 *
 * @note If equation has only one solution, it will be written to addres proot1
 */
Roots_c SolveQuadratic(const double coef_a, const double coef_b, const double coef_c, double* const p_root1, double* const p_root2);

/**
 * @brief find the solution of linear equation with coefficients coef_a, coef_b
 *        and write them to the address proot
 * @param[in] coef_a   coefficient a of linear equation
 * @param[in] coef_b   coefficient b of linear equation
 * @param[out] p_root   pointer to root of linear equation
 *
 * @return the number of solutions or INF_ROOTS if there are infinite
 */
Roots_c SolveLinear(const double coef_a, const double coef_b, double* const p_root);

/**
 * @brief divide dividend/divider preventing the return of -0.0
 *
 * @param[in] dividend dividend
 * @param[in] divider  divider
 *
 * @return the quotient of dividend/divider
 */
double DivideSmart(const double dividend, const double divider);

/**
 * @brief compares roots a and b taking EPSILON as accuracy into account
 *
 * @param[in] a first being compared number
 * @param[in] b second being compared number
 *
 * @return 1 if (a = b) || (a = NAN && b = NAN) or 0 otherwise
 */
bool CmpEpsPrec(const double a, const double b);

/**
 * @brief find out whether lf is NAN
 *
 * @param[in] lf number under investigation
 *
 * @return 1 if lf is NAN or 0 if lf isn't
 */
bool IsNAN(double lf);


/**
 * @brief print roots depending on the value of n_roots
 *
 * @param[in] n_roots  number of roots
 * @param[in] p_roor_1 first root of quadratic equation
 * @param[in] p_roor_2 second root of quadratic equation
 *
 * @return PRINT_NORMAL or error number
 */
Print_err PrintRoots(Roots_c n_roots, const double root_1, const double root_2);
