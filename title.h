#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>

#define EPSILON 0.00001                     //comparison accuracy
typedef enum {NOROOTS, ONEROOT, TWOROOTS, INFROOTS = -2} rootsc;
typedef enum {FILE_INPUT, CONSOLE_INPUT, ERR} inptype;
typedef enum {NO, YES, NONE} answ;

#define ON  1
#define OFF 0

#define COLORSWITCH ON                  // switch color output

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

/* AskInputType: asks the user to choose input type.
Returns FILE_INPUT or CONSOLE_INPUT or ERR if there is error. */
inptype AskInputType(void);

/* FileRequestCoefficients: transmit three values from coefficients.txt to addresses pcoef_a, pcoef_b, pcoef_c.
Returns 0 if everything is correct or 1 if there is file read error or 2 if there is fscanf error. */
int FileRequestCoefficients(double* const p_coef_a, double* const p_coef_b, double* const p_coef_c);

/* RequestCoefficients: transmit three values from input to addresses pcoef_a, pcoef_b, pcoef_c.
Returns 1 if the user wants to close the program or 0 otherwise or 2 if there is error. */
int RequestCoefficients(double* const p_coef_a, double* const p_coef_b, double* const p_coef_c);

/* SolveQuadratic: finds the solution of quadratic equation with coefficients coef_a, coef_b, coef_c
and writes them to the address proot1, proot2.
If equation has only one solution, it will be written to addres proot1.
Returns the number of solutions or INFROOTS if there are infinite. */
rootsc SolveQuadratic(const double coef_a, const double coef_b, const double coef_c, double* const p_root1, double* const p_root2);

/* PrintRoots: print roots depending on the value of nroots.
Returns 1 if everything is correct or 0 otherwise. */
int PrintRoots(rootsc nroots, const double root1, const double root2);

/* AskYesOrNo: asks the user to write Y or N unless he do it.
Returns 1 if answer is Y or 0 if answer is N. */
answ AskYesOrNo(void);

/* ClearInputBuf: clears the input buffer from excess symbols.
Buffer mustn't be empty. */
void ClearInputBuf(void);

/* CmpEpsPrec: compares a and b taking EPSILON as accuracy into account.
Returns 1 if a = b or 0 otherwise. */
bool CmpEpsPrec(const double a, const double b);

/* SolveLinear: finds the solution of linear equation with coefficients coef_a, coef_b
and writes them to the address proot.
Returns the number of solutions or INFROOTS if there are infinite. */
rootsc SolveLinear(const double coef_a, const double coef_b, double* const p_root);

/* DivideSmart: returns the quotient of dividend/divider preventing the return of -0.0 */
double DivideSmart(const double dividend, const double divider);
