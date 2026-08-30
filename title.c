#include "TXLib.h"   /// TODO README
#include <stdio.h>   // TODO pass test file
#include <stdlib.h>  // TODO clear output
#include <string.h>  // TODO arguments of flags
#include <math.h>    // TODO collect warnings
#include <ctype.h>   /// TODO if click enter or other key it will be skiped for current print

#define HELPFILE "help.txt"
#define OUTPUT_SLOW_DELAY 25.0       // milliseconds
#define OUTPUT_FAST_DELAY 15.0       // milliseconds
#define MAX_OUTOUT_LENGTH 200
#define MAX_TEST_LINE_LENGTH 200
#define HUNDRED_PERCENT 100
#define TESTFILE "tests_for_SolveQuadratic.txt"
#define FAILED_TESTS_FILE "failed_tests.txt"
#define DEFAULT_FILE "coefficients.txt"
#define MAX_FILENAME_LENGTH 100
#define EPSILON 0.00001                                                         // comparison accuracy

#define WINDOW_SIZE_X 1000.0
#define WINDOW_SIZE_Y 800.0
#define WINDOW_HALF_X (WINDOW_SIZE_X / 2)
#define WINDOW_HALF_Y (WINDOW_SIZE_Y / 2)
#define WINDOW_QUARTER_X (WINDOW_SIZE_X / 4)
#define WINDOW_QUARTER_Y (WINDOW_SIZE_Y / 4)
#define SCALE_DIVISION_COUNT_X 10
#define SCALE_DIVISION_COUNT_Y 8
#define MAX_NUMBER_LENGTH 50
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
#define STEPS_COUNT (int)(WINDOW_HALF_X / DELTA_X)//((int)((WINDOW_HALF_X - DEFAULT_ORDINATE_POSITION) / DELTA_X))
#define DEFAULT_ORDINATE_POSITION 30 // pixels
#define DEFAULT_SCALE_X (WINDOW_SIZE_Y / 10.0)
#define DEFAULT_SCALE_Y (WINDOW_SIZE_Y / 10.0)

#define MAX_ERROR_COUNT 10
enum {HELP_ORD = 1, FREQ_ORD, REQ_ORD, PRINT_ORD, GET_ORD};

enum {MAIN_NORMAL, MAIN_FILE_ERROR, MAIN_INPUT_ERROR,
      MAIN_NROOT_ERROR, MAIN_TESTING_ERROR};                                     // return of main()
typedef enum {FTEST_NORMAL = 0, FTEST_EXP_NROOT_ERROR = -1, FTEST_INPUT_ERROR = -2,
              FTEST_FOPEN_ERROR = -3, FTEST_READ_ERROR = -4}                     Ftest_err_t; // return of RunFileTests()
typedef enum {FILE_INPUT, CONSOLE_INPUT, TYPE_ERROR}                             Input_type_t; // return of AskInputType()
typedef enum {NO = 0, YES = 1, NONE = 2}                                         Answer_t; // return of AskYesOrNo())
typedef enum {NO_ROOTS = 0, ONE_ROOT = 1, TWO_ROOTS = 2, INF_ROOTS = -2}         Roots_c_t; // return od Solve___()
typedef enum {HELP_NORMAL = 0, HELP_FOPEN_ERROR = HELP_ORD * MAX_ERROR_COUNT,
              HELP_READ_ERROR}                                                   Help_err_t; // return of ShowHelplist()
typedef enum {FREQ_NORMAL = 0, FREQ_STOP_PROGRAM = FREQ_ORD * MAX_ERROR_COUNT,
              FREQ_INPUT_ERROR, FREQ_FOPEN_ERROR, FREQ_READ_ERROR}               Freq_err_t; // return of FileRequestCoefficients()
typedef enum {REQ_NORMAL = 0, REQ_STOP_PROGRAM = REQ_ORD * MAX_ERROR_COUNT,
              REQ_INPUT_ERROR}                                                   Req_err_t; // return of RequestCoefficients()
typedef enum {PRINT_NORMAL = 0, PRINT_NROOT_ERROR = PRINT_ORD * MAX_ERROR_COUNT} Print_err_t; // return of PrintRoots()
typedef enum {GET_NORMAL = 0, GET_INPUT_ERROR = GET_ORD * MAX_ERROR_COUNT}       get_err_t; // return of GetCoefficients()

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
    unsigned int show_helplist_flag : 1;
    unsigned int draw_graphic_flag : 1;
    unsigned int animated_output_flag : 1;
};
//char COMMAND_LINE_FLAGS_LIST[] = "fct";

struct Expected_sq_result {int n_roots; double root_1; double root_2;};
struct Conformity_sq_result {bool n_roots, root_1, root_2;};
struct Quadratic_coefficients {double a; double b; double c;};
struct Position_parameters {pixel pxl_vertex_x; pixel pxl_vertex_y;
                            double x_vertex_value; double y_vertex_value; pixel scale_x; pixel scale_y;
                            pixel abscissa_position; pixel ordinate_position;};
struct Axis_scale {double dgt_step_x; pixel pxl_step_x; double anchor_value_x; pixel anchor_point_x;
                   double dgt_step_y; pixel pxl_step_y; double anchor_value_y; pixel anchor_point_y;};
struct Derivative_parameters {const Quadratic_coefficients* coef; pixel current_x;
                              pixel pxl_vertex_x; pixel scale_x;};


//__________________________main_functions___________________________
/**
 * @brief get command line flags from command line arguments
 *
 * @param[in]  argc argc from command line
 * @param[in]  agrv argv from command line
 * @param[out] command_line_flags pointer to stract with flags
 */
void HandleArg(const int argc, const char* argv[], Flags* command_line_flags);

/**
 * @brief open file and print helplist from it
 *
 * @param[in] command_line_flags pointer to stract with flags
 *
 * @return HELP_NORMAL or exit with error number
 */
Help_err_t ShowHelplist(const Flags* command_line_flags);

/**
 * @brief print roots depending on the value of n_roots
 *
 * @param[in] n_roots            number of roots
 * @param[in] p_roor_1           first root of quadratic equation
 * @param[in] p_roor_2           second root of quadratic equation
 * @param[in] command_line_flags pointer to stract with flags
 *
 * @return PRINT_NORMAL or exit with error number
 */
Print_err_t PrintRoots(Roots_c_t n_roots, const double root_1, const double root_2,
                       const Flags* command_line_flags);

/**
 * @brief do animated print if there is animated_output_flag or do simple print
 *
 * @param[in] output_str         line for output
 * @param[in] command_line_flags pointer to stract with flags
 */
void PrintAnimated(const char* output_str, const Flags* command_line_flags);

//____________________UnitTest_for_SolveQuadratic____________________
/**
 * @brief open file and do tests from it
 *
 * @param[in] command_line_flags pointer to stract with flags
 *
 * @return percent of successful tests or exit negative error number if it have occurred
 */
int RunFileTests(const Flags* command_line_flags);

/**
 * @brief print a message depending on error_type
 *
 * @param[in] error_type         error number
 * @param[in] test_number        number of failed test
 * @param[in] command_line_flags pointer to stract with flags
 */
void PrintErrorMessage(const Ftest_err_t error_type, const int test_number,
                       const Flags* command_line_flags);

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
 * @param[out] coef               pointer to structure containing coefficients of quadratic equation
 * @param[in]  command_line_flags pointer to stract with flags
 *
 * @return GET_NORMAL or exit with error number
 */
get_err_t GetCoefficients(Quadratic_coefficients* coef, const Flags* command_line_flags);

/**
 * @brief ask the user to choose input type
 *
 * @param[in] command_line_flags pointer to stract with flags
 *
 * @return FILE_INPUT or CONSOLE_INPUT or TYPE_ERROR if there is error
 */
Input_type_t AskInputType(const Flags* command_line_flags);

/**
 * @brief transmit three values from coefficients.txt to addresses coef
 *
 * @param[out] coef               pointer to structure containing coefficients of quadratic equation
 * @param[in]  command_line_flags pointer to stract with flags
 *
 * @return FREQ_NORMAL or exit with error number
 */
Freq_err_t FileRequestCoefficients(Quadratic_coefficients* coef, const Flags* command_line_flags);

/**
 * @brief Ask the user to write name of file until this is in correct form.
 *
 * @param[in] command_line_flags pointer to stract with flags
 *
 * @return filename
 */
char* GetFilename(const Flags* command_line_flags);

/**
 * @brief transmit three values from input to addresses coef
 *
 * @param[out] coef               pointer to structure containing coefficients of quadratic equation
 * @param[in]  command_line_flags pointer to stract with flags
 *
 * @return REQ_NORMAL or exit with error number
 */
Req_err_t RequestCoefficients(Quadratic_coefficients* coef, const Flags* command_line_flags);

/**
 * @brief ask the user to write Y or N unless he do it
 *
 * @param[in] command_line_flags pointer to stract with flags
 *
 * @return YES if answer is Y or  if answer is N
 */
Answer_t AskYesOrNo(const Flags* command_line_flags);

/**
 * @brief clear the input buffer from excess symbols
 *
 * @note buffer mustn't be empty
 */
void ClearInputBuf(void);

//_______________________calculation_functions_______________________
/**
 * @brief find the solution of quadratic equation with coefficients coef_a, coef_b, coef_c
 *        and write them to the address proot1, proot2
 * @param[in]  coef     pointer to structure containing coefficients of quadratic equation
 * @param[out] p_roor_1 pointer to first root of quadratic equation
 * @param[out] p_roor_2 pointer to second root of quadratic equation
 *
 * @return the number of solutions or INF_ROOTS if there are infinite
 *
 * @note if equation has only one solution, it will be written to addres proot1
 */
Roots_c_t SolveQuadratic(const Quadratic_coefficients* coef, double* const p_root1, double* const p_root2);

/**
 * @brief find the solution of linear equation with coefficients coef_a, coef_b
 *        and write them to the address proot
 * @param[in]  coef_a   coefficient a of linear equation
 * @param[in]  coef_b   coefficient b of linear equation
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
 * @return true if (a = b) || (a = NAN && b = NAN) or false otherwise
 */
bool CmpEpsPrec(const double a, const double b);

/**
 * @brief find out whether lf is NAN
 *
 * @param[in] lf number under investigation
 *
 * @return true if lf is NAN or false if lf isn't
 *
 * @note IEEE 754
 */
bool IsNAN(const double lf);

/**
 * @brief get sign of x
 *
 * @param[in] x number under investigation
 *
 * @return 1 0 or -1 depending on sign of x
 */
int Sign(const double x);

//__________________________draw_functions___________________________
/**
 * @brief draw beautiful graphic of parabola or linear with coordinate system
 *
 * @param[in] coef pointer to structure containing coefficients of quadratic equation
 */
void DrawParabola(const Quadratic_coefficients* coef);

/**
 * @brief get information about position and scale of graphic
 *
 * @param[in]  coef     pointer to structure containing coefficients of quadratic equation
 * @param[out] parabola pointer to structure containing information about position and scale of graphic
 */
void GetParabolaPosition(const Quadratic_coefficients* coef, Position_parameters* parabola);

/**
 * @brief draw beautiful graphic of linear with coordinate system
 *
 * @param[in]  coef_a coefficient a of linear equal
 * @param[in]  coef_b coefficient b of linear equal
 */
void DrawLinear(double coef_a, double coef_b);

/**
 * @brief get information about position and scale of graphic
 *
 * @param[in]  coef_a coefficient a of linear equal
 * @param[in]  coef_b coefficient b of linear equal
 * @param[out] linear pointer to structure containing information about position and scale of graphic
 */
void GetLinearPosition(double coef_a, double coef_b, Position_parameters* linear);

/**
 * @brief draw beautiful graphic of some function with coordinate system
 *
 * @param[in] coef       pointer to structure containing coefficients of quadratic equation
 * @param[in] position   pointer to structure containing information about position and scale of graphic
 * @param[in] Derivative function calculating derivative of function for DrawGraphic()
 */
void DrawGraphic(const Quadratic_coefficients* coef, const Position_parameters* position,
                 double Derivative(const Derivative_parameters*));

/**
 * @brief get information about digitalization of axes
 *
 * @param[in]  position pointer to structure containing information about position and scale of graphic
 * @param[out] axes_sc  pointer to structure containing information about digitalization of axes
 */
void GetAxisScale(const Position_parameters* position, Axis_scale* axes_sc);

/**
 * @brief make black window
 */
void MakeBlackWindow(void);

/**
 * @brief cancel console printing
 */
void CancelConsolePrinting(void);

/**
 * @brief draw coordinate grid
 *
 * @param[in] axes_sc  pointer to structure containing information about digitalization of axes
 */
void DrawGrid(const Axis_scale* axes_sc);

/**
 * @brief draw digitalized coordinate axes
 *
 * @param[in]  coef     pointer to structure containing coefficients of quadratic equation
 * @param[in]  position pointer to structure containing information about position and scale of graphic
 */
void DrawAxis(const Axis_scale* axes_sc, const Position_parameters* position);

/**
 * @brief draw curve of some function
 *
 * @param[in] coef       pointer to structure containing coefficients of quadratic equation
 * @param[in] position   pointer to structure containing information about position and scale of graphic
 * @param[in] Derivative function calculating derivative of function for DrawGraphic()
 */
void DrawCurve(const Quadratic_coefficients* coef, const Position_parameters* position,
               double Derivative(const Derivative_parameters*));

/**
 * @brief get derivative of quadratic equal for current position_x
 *
 * @param[in] der_par pointer to structure containing parameters for derivative
 *
 * @return derivative of quadratic equal
 */
double QuadraticEqualDerivative(const Derivative_parameters* der_par);

/**
 * @brief get derivative of linear equal for current position_x
 *
 * @param[in] der_par pointer to structure containing parameters for derivative
 *
 * @return derivative of linear equal
 */
double LinearEqualDerivative (const Derivative_parameters* der_par);

/**
 * @brief find closest number among 0.5 1 2 for number 0.32 - 3.2
 *
 * @param[in] x number 0.32 - 3.2
 *
 * @return closest number among 0.5 1 2
 */
double Closest05_1_2(const double x);

/**
 * @brief get decimal place of full and write mantissa redused to form 0.5 1 2 to p_mantissa
 *
 * @param[in]  full       full number
 * @param[out] p_mantissa pointer to mantissa redused to form 0.5 1 2
 *
 * @return decimal place of full
 */
int Place(double full, double* p_mantissa);
