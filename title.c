#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>

#define MAX_FILENAME_LONG 100
#define EPSILON 0.00001                                                       //comparison accuracy
typedef enum {NO_ROOTS = 0, ONE_ROOT = 1, TWO_ROOTS = 2, INF_ROOTS = -2} Roots_c;
typedef enum {FILE_INPUT, CONSOLE_INPUT, ERROR} Input_type;
typedef enum {NO, YES, NONE} Answer;

enum {MAIN_NORMAL, MAIN_FILE_ERROR,
      MAIN_INPUT_ERROR, MAIN_NROOT_ERROR};                                    // return of main()
typedef enum {FREQ_NORMAL, FREQ_STOP_PROGRAM,
              FREQ_INPUT_ERROR, FREQ_FOPEN_ERROR, FREQ_READ_ERROR} Freq_err;  // return of FileRequestCoefficients()
typedef enum {REQ_NORMAL, REQ_STOP_PROGRAM,
              REQ_INPUT_ERROR} Req_err;                                       // return of RequestCoefficients()
typedef enum {PRINT_NORMAL, PRINT_NROOT_ERROR} Print_err;                     // return of PrintRoots()

#define ON  1
#define OFF 0

#define COLORSWITCH ON                                                        // switch color output

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

/* SolveQuadratic: find the solution of quadratic equation with coefficients coef_a, coef_b, coef_c
and write them to the address proot1, proot2.
If equation has only one solution, it will be written to addres proot1.
Return the number of solutions or INF_ROOTS if there are infinite. */
Roots_c SolveQuadratic(const double coef_a, const double coef_b, const double coef_c, double* const p_root1, double* const p_root2);

/* PrintRoots: print roots depending on the value of n_roots.
Return 1 if everything is correct or 0 otherwise. */
Print_err PrintRoots(Roots_c n_roots, const double root_1, const double root_2);

/* AskYesOrNo: ask the user to write Y or N unless he do it.
Return 1 if answer is Y or 0 if answer is N. */
Answer AskYesOrNo(void);

/* ClearInputBuf: clear the input buffer from excess symbols.
Buffer mustn't be empty. */
void ClearInputBuf(void);

/* CmpEpsPrec: compares roots a and b taking EPSILON as accuracy into account.
Return 1 if (a = b) || (a = NAN && b = NAN) or 0 otherwise. */
bool CmpEpsPrec(const double a, const double b);

/* SolveLinear: find the solution of linear equation with coefficients coef_a, coef_b
and write them to the address proot.
Return the number of solutions or INF_ROOTS if there are infinite. */
Roots_c SolveLinear(const double coef_a, const double coef_b, double* const p_root);

/* DivideSmart: return the quotient of dividend/divider preventing the return of -0.0 */
double DivideSmart(const double dividend, const double divider);
