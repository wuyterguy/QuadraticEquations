#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>

#define EPSILON 0.00001                     //comparison accuracy
typedef enum {NOROOTS, ONEROOT, TWOROOTS, INFROOTS = -2} rootsc;

/* CoefficientsRequest: transmit three values from input to addresses pcoef_a, pcoef_b, pcoef_c.
Returns 1 if the user wants to close the program or 0 otherwise. */
int CoefficientsRequest(double* const p_coef_a, double* const p_coef_b, double* const p_coef_c);

/* QuadraticSolver: finds the solution of quadratic equation with coefficients coef_a, coef_b, coef_c
and writes them to the address proot1, proot2.
If equation has only one solution, it will be written to addres proot1.
Returns the number of solutions or INFROOTS if there are infinite. */
rootsc QuadraticSolver(const double coef_a, double const coef_b, double const coef_c, double* const p_root1, double* const p_root2);

/* PrintRoots: print roots depending on the value of nroots.
Returns 1 if everything is correct or 0 otherwise. */
int PrintRoots(rootsc nroots, const double root1, const double root2);

/* AskYesOrNo: asks the user to write Y or N unless he do it.
Returns 1 if answer is Y or 0 if answer is N. */
int AskYesOrNo(void);

/* ClearInputBuf: clears the input buffer from excess symbols */
void ClearInputBuf(void);

/* EpsPrecComparison: compares a and b taking EPSILON as accuracy into account.
Returns 1 if a = b or 0 otherwise. */
int EpsPrecComparison(const double a, const double b);

/* LinearSolver: finds the solution of linear equation with coefficients coef_a, coef_b
and writes them to the address proot.
Returns the number of solutions or INFROOTS if there are infinite. */
rootsc LinearSolver(const double coef_a, const double coef_b, double* const p_root);

/* SmartDivision: returns the quotient of dividend/divider preventing the return of -0.0 */
double SmartDivision(const double dividend, const double divider);
