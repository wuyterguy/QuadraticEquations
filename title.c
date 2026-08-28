#include "TXLib.h"
#include <stdio.h>
#include <stdlib.h>  // TODO clear output
#include <string.h>
#include <math.h>
#include <ctype.h>

#define MAX_TEST_LINE_LENGTH 200
#define HUNDRED_PERCENT 100
#define TESTFILE "tests_for_SolveQuadratic.txt"
#define DEFAULT_FILE "coefficients.txt"
#define MAX_FILENAME_LENGTH 100
#define EPSILON 0.00001                                                         // comparison accuracy

#define WINDOW_SIZE_X 800.0
#define WINDOW_SIZE_Y 800.0
#define WINDOW_HALF_X (WINDOW_SIZE_X / 2)
#define WINDOW_HALF_Y (WINDOW_SIZE_X / 2)
#define WINDOW_QUARTER_X (WINDOW_SIZE_X / 4)
#define WINDOW_QUARTER_Y (WINDOW_SIZE_X / 4)
#define SCALE_DIVISION_COUNT_X 10
#define SCALE_DIVISION_COUNT_Y 8
#define MAX_NUMBER_SIZE 50
#define GRID_THICKNESS 1.0           // it's all pixels
#define CURVE_THICKNESS 3.0
#define AXIS_THICKNESS 1.0
#define SHORT_HATCH_THICKNESS 2.0
#define LONG_HATCH_THICKNESS 3.0
#define SHORT_HATCH_SIZE 5.0
#define LONG_HATCH_SIZE 7.0
#define ROOT_POINT_SIZE 5.0
#define ORIGIN_POINT_SIZE 5.0
#define TEXT_SIZE 20.0
#define SHORT_HATCH_PER_LONG 5.0     // must be natural
#define OFFSET 8.0
#define MIN_ORDINATE_DISTANCE 50.0
#define DELTA_X 1.0                  // pixels
#define STEPS_COUNT ((int)((WINDOW_SIZE_X / 2 - DEFAULT_ORDINATE_POSITION) / DELTA_X))
#define DEFAULT_ORDINATE_POSITION 30 // pixels
#define DEFAULT_SCALE_Y (WINDOW_SIZE_Y / 10.0)

#define MAX_ERROR_COUNT 10
enum {MAIN_NORMAL, MAIN_FILE_ERROR, MAIN_INPUT_ERROR,
      MAIN_NROOT_ERROR, MAIN_TESTING_ERROR};                                          // return of main()
typedef enum {FTEST_NORMAL = 0, FTEST_EXP_NROOT_ERROR = -1, FTEST_INPUT_ERROR = -2,
              FTEST_FOPEN_ERROR = -3, FTEST_READ_ERROR = -4}             Ftest_err_t; // return of RunFileTests()
typedef enum {FILE_INPUT, CONSOLE_INPUT, TYPE_ERROR}                    Input_type_t; // return of AskInputType()
typedef enum {NO = 0, YES = 1, NONE = 2}                                    Answer_t; // return of AskYesOrNo())
typedef enum {FREQ_NORMAL = 0, FREQ_STOP_PROGRAM = MAX_ERROR_COUNT * 1,
              FREQ_INPUT_ERROR, FREQ_FOPEN_ERROR, FREQ_READ_ERROR}        Freq_err_t; // return of FileRequestCoefficients()
typedef enum {REQ_NORMAL = 0, REQ_STOP_PROGRAM = MAX_ERROR_COUNT * 2,
              REQ_INPUT_ERROR}                                             Req_err_t; // return of RequestCoefficients()
typedef enum {PRINT_NORMAL = 0, PRINT_NROOT_ERROR = MAX_ERROR_COUNT * 3} Print_err_t; // return of PrintRoots()
typedef enum {GET_NORMAL = 0, GET_INPUT_ERROR = MAX_ERROR_COUNT * 4}       get_err_t; // return of GetCoefficients()
typedef enum {NO_ROOTS = 0, ONE_ROOT = 1, TWO_ROOTS = 2, INF_ROOTS = -2}   Roots_c_t; // return od Solve___()

typedef double pixel;


#define ON  1
#define OFF 0

#define COLOR_SWITCH ON                                                         // switch color output

#define SCREEN_CLEAR_SWITCH ON                                                  // switch clearing screen

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
    do {                                                                                               \
        if(!(condition)) {                                                                             \
            fprintf(stderr, "Assertion failed: " #condition ", file %s, line %d", __FILE__, __LINE__); \
            abort();                                                                                   \
        }                                                                                              \
    } while (0);
#else
    #define assert(condition)
#endif

struct Flags {
    unsigned int file_input_flag : 1;
    unsigned int console_input_flag : 1;
    unsigned int preliminary_testing_flag : 1;
};
//char COMMAND_LINE_FLAGS_LIST[] = "fct";

struct Expected_sq_result {int n_roots; double root_1; double root_2;};
struct Conformity_sq_result {bool n_roots, root_1, root_2;};
struct Quadratic_coefficients {double a; double b; double c;};
struct Position_parameters {pixel pxl_vertex_x; pixel pxl_vertex_y; pixel scale_x; pixel scale_y;
                            pixel abscissa_position; pixel ordinate_position;};
struct Axis_scale {double dgt_step_x; pixel pxl_step_x; double anchor_value_x; pixel anchor_point_x;
                   double dgt_step_y; pixel pxl_step_y; double anchor_value_y; pixel anchor_point_y;};


/**
 * @brief get command line flags from command line arguments
 *
 * @param[in] argc argc from command line
 * @param[in] agrv argv from command line
 *
 * @return stract with got flags
 */
Flags HandleArg(const int argc, const char* argv[]);

//____________________UnitTest_for_SolveQuadratic____________________
/**
 * @brief open file and do tests from it
 *
 * @return percent of successful tests or exit negative error number if it have occurred
 */
int RunFileTests(void);

/**
 * @brief print a message depending on error_type
 *
 * @param[in] error_type  error number
 * @param[in] test_number number of failed test
 */
void PrintErrorMessage(const Ftest_err_t error_type, const int test_number);

/**
 * @brief do test of SolveQuadratic() using values from parameters
 *
 * @param[in] coef        pointer to structure containing coefficients of quadratic equation
 * @param[in] exp         structure containing expected values
 * @param[in] test_number number of current test
 *
 * @return 1 if test is successful or 0 otherwise
 */
int RunTest(const Quadratic_coefficients* coef, const Expected_sq_result exp, const int test_number);

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
 * @param[in] coef    pointer to structure containing coefficients of quadratic equation
 * @param[in] n_roots number of roots
 * @param[in] roor_1  first root of quadratic equation
 * @param[in] roor_2  second root of quadratic equation
 * @param[in] exp     structure containing expected values
 * @param[in] conf    structure containing information about conformity between got and expected values
 */
void PrintFailed(const Quadratic_coefficients* coef, const int n_roots, const double root_1, const double root_2,
                 const Expected_sq_result exp, const Conformity_sq_result conf, const int test_number);

//__________________________input_functions__________________________
/**
 * @brief get coefficients to addres coef by console or console depending on command_line_flags
 *
 * @param[out] coef pointer to structure containing coefficients of quadratic equation
 *
 * @return GET_NORMAL or exit with error number
 */
get_err_t GetCoefficients(Quadratic_coefficients* coef,Flags command_line_flags);

/**
 * @brief ask the user to choose input type
 *
 * @return FILE_INPUT or CONSOLE_INPUT or TYPE_ERROR if there is error
 */
Input_type_t AskInputType(void);

/**
 * @brief transmit three values from coefficients.txt to addresses coef
 *
 * @param[out] coef pointer to structure containing coefficients of quadratic equation
 *
 * @return FREQ_NORMAL or exit with error number
 */
Freq_err_t FileRequestCoefficients(Quadratic_coefficients* coef);

/**
 * @brief Ask the user to write name of file until this is in correct form.
 *
 * @return filename
 */
char* GetFilename(void);

/**
 * @brief transmit three values from input to addresses coef
 *
 * @param[out] coef pointer to structure containing coefficients of quadratic equation
 *
 * @return REQ_NORMAL or exit with error number
 */
Req_err_t RequestCoefficients(Quadratic_coefficients* coef);

/**
 * @brief ask the user to write Y or N unless he do it
 *
 * @return YES if answer is Y or  if answer is N
 */
Answer_t AskYesOrNo(void);

/**
 * @brief clear the input buffer from excess symbols
 *
 * @note Buffer mustn't be empty
 */
void ClearInputBuf(void);

//_______________________calculation_functions_______________________
/**
 * @brief find the solution of quadratic equation with coefficients coef_a, coef_b, coef_c
 *        and write them to the address proot1, proot2
 * @param[in] coef      pointer to structure containing coefficients of quadratic equation
 * @param[out] p_roor_1 pointer to first root of quadratic equation
 * @param[out] p_roor_2 pointer to second root of quadratic equation
 *
 * @return the number of solutions or INF_ROOTS if there are infinite
 *
 * @note If equation has only one solution, it will be written to addres proot1
 */
Roots_c_t SolveQuadratic(const Quadratic_coefficients* coef, double* const p_root1, double* const p_root2);

/**
 * @brief find the solution of linear equation with coefficients coef_a, coef_b
 *        and write them to the address proot
 * @param[in] coef_a   coefficient a of linear equation
 * @param[in] coef_b   coefficient b of linear equation
 * @param[out] p_root   pointer to root of linear equation
 *
 * @return the number of solutions or INF_ROOTS if there are infinite
 */
Roots_c_t SolveLinear(const double coef_a, const double coef_b, double* const p_root);

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
bool IsNAN(const double lf);

int Sign(const double x);  // TODO doc

//__________________________draw_functions___________________________
void DrawParabola(const Quadratic_coefficients* coef);

void GetParabolaPosition(const Quadratic_coefficients* coef, Position_parameters* parabola);

void DrawGraphic(const Quadratic_coefficients* coef, const Position_parameters* position,
                 double Derivative(const Quadratic_coefficients*, const pixel, const pixel, const pixel));

void GetAxisScale(const Quadratic_coefficients* coef, const Position_parameters* position,
                         Axis_scale* axis_sc);

void MakeBlackWindow(void);

void CancelConsolePrinting(void);

void DrawGrid(const Axis_scale* axis_sc);

void DrawAxis(const Axis_scale* axis_sc, const Position_parameters* position);

void DrawCurve(const Quadratic_coefficients* coef, const Position_parameters* position,
               double Derivative(const Quadratic_coefficients*, const pixel, const pixel, const pixel));

double QuadraticEqualDerivative(const Quadratic_coefficients* coef, const pixel position_x,
                                 const pixel pxl_vertex_x, const pixel scale_x);

double GetClosest05_1_2(const double x);

double GetPlace(double full, double* p_mantissa);


/**
 * @brief print roots depending on the value of n_roots
 *
 * @param[in] n_roots  number of roots
 * @param[in] p_roor_1 first root of quadratic equation
 * @param[in] p_roor_2 second root of quadratic equation
 *
 * @return PRINT_NORMAL or exit with error number
 *
 * @note IEEE 754
 */
Print_err_t PrintRoots(Roots_c_t n_roots, const double root_1, const double root_2);
