// in task you need to use point_
#define DoAcrossEntireScreenRange(lo_coord, up_coord, type_start, start, step, task)   \
for (type_start point_##lo_coord = start##lo_coord;                                    \
     point_##lo_coord <= WINDOW_SIZE_##up_coord; point_##lo_coord += step##lo_coord) { \
    task                                                                               \
}                                                                                      \
for (type_start point_##lo_coord = start##lo_coord;                                    \
     point_##lo_coord >= 0.0; point_##lo_coord -= step##lo_coord) {                    \
    task                                                                               \
}

#define DoXYAcrossEntireScreenRange(type_start, start, step, task_x, task_y) \
    DoAcrossEntireScreenRange(x, X, type_start, start, step, task_x)         \
    DoAcrossEntireScreenRange(y, Y, type_start, start, step, task_y)


void DrawParabola(const Quadratic_coefficients* coef) {
    assert(coef != NULL);
    assert(!CmpEpsPrec(coef->a, 0.0));

    if (!CmpEpsPrec(coef->a, 0.0)) {
        Position_parameters parabola = {};
        GetParabolaPosition(coef, &parabola);

        DrawGraphic(coef, &parabola, QuadraticEqualDerivative);
    }
    else
        DrawLinear(coef->b, coef->c);
}


void GetParabolaPosition(const Quadratic_coefficients* coef, Position_parameters* parabola) {
    assert(coef != NULL);
    assert(parabola != NULL);

    double discr = coef->b * coef->b - 4.0 * coef->a * coef->c;

    parabola->y_vertex_value = coef->c - coef->b * coef->b / 4.0 / coef->a;
    parabola->x_vertex_value = -coef->b / 2.0 / coef->a;
    parabola->pxl_vertex_x = WINDOW_HALF_X;

    if (CmpEpsPrec(discr, 0.0)) {
        parabola->abscissa_position = WINDOW_HALF_Y;
        parabola->scale_y = DEFAULT_SCALE_Y;
        parabola->scale_x = sqrt(parabola->scale_y * fabs(coef->a) * WINDOW_QUARTER_Y);
        parabola->pxl_vertex_y = WINDOW_HALF_Y;
    }
    else if (discr > 0.0) {
        parabola->abscissa_position = WINDOW_HALF_Y;                                     // middle
        parabola->scale_x = WINDOW_HALF_X / (sqrt(discr) / fabs(coef->a));               // roots difference
        parabola->scale_y = WINDOW_QUARTER_Y / fabs(parabola->y_vertex_value);                     // height of pxl_vertex
        parabola->pxl_vertex_y = parabola->abscissa_position - parabola->y_vertex_value * parabola->scale_y;
    }
    else { /* (discr < 0) */
        parabola->abscissa_position = fmod(WINDOW_SIZE_Y - Sign(coef->a) * WINDOW_QUARTER_Y, WINDOW_SIZE_Y);
        parabola->scale_y = WINDOW_QUARTER_Y / fabs(parabola->y_vertex_value);                     // height of pxl_vertex
        parabola->scale_x = sqrt(parabola->scale_y * fabs(coef->a) * WINDOW_QUARTER_Y);  // beauty
        parabola->pxl_vertex_y = parabola->abscissa_position - parabola->y_vertex_value * parabola->scale_y;
    }

    if ((WINDOW_HALF_X - DEFAULT_ORDINATE_POSITION) / parabola->scale_x > fabs(parabola->x_vertex_value))
        parabola->ordinate_position = WINDOW_HALF_X - parabola->x_vertex_value * parabola->scale_x;
    else /* ordinate isn't into screen */
        parabola->ordinate_position = fmod(WINDOW_SIZE_X + Sign(parabola->x_vertex_value) * DEFAULT_ORDINATE_POSITION, WINDOW_SIZE_X);
}


void DrawLinear(double coef_a, double coef_b) {
    Position_parameters linear = {};
    GetLinearPosition(coef_a, coef_b, &linear);

    Quadratic_coefficients coef = {coef_a, coef_b, NAN};
    DrawGraphic(&coef, &linear, LinearEqualDerivative);
}


void GetLinearPosition(double coef_a, double coef_b, Position_parameters* linear) {
    linear->pxl_vertex_x = WINDOW_HALF_X;
    linear->pxl_vertex_y = WINDOW_HALF_Y - Sign(coef_b) * WINDOW_QUARTER_Y;
    linear->y_vertex_value = coef_b;
    linear->x_vertex_value = 0.0;
    linear->abscissa_position = WINDOW_HALF_Y;
    linear->ordinate_position = WINDOW_HALF_X;
    if (!CmpEpsPrec(coef_b, 0.0))
        linear->scale_y = WINDOW_QUARTER_Y / fabs(coef_b);
    else
        linear->scale_y = DEFAULT_SCALE_Y;
    if (!CmpEpsPrec(coef_a, 0.0))
        linear->scale_x = WINDOW_QUARTER_X / fabs(coef_b / coef_a);
    else
        linear->scale_x = DEFAULT_SCALE_X;
}


void DrawGraphic(const Quadratic_coefficients* coef, const Position_parameters* position,
                 double Derivative(const Derivative_parameters*)) {
    assert(coef != NULL);
    assert(position != NULL);
    assert(Derivative);

    Axis_scale axes_sc = {};
    GetAxisScale(position, &axes_sc);

    MakeBlackWindow();

    CancelConsolePrinting();

    DrawGrid(&axes_sc);

    DrawAxis(&axes_sc, position);

    DrawCurve(coef, position, Derivative);
}


void GetAxisScale(const Position_parameters* position, Axis_scale* axes_sc) {
    assert(position != NULL);
    assert(axes_sc != NULL);

    double sc_div_x = WINDOW_SIZE_X / SCALE_DIVISION_COUNT_X / position->scale_x;
    double sc_div_y = WINDOW_SIZE_Y / SCALE_DIVISION_COUNT_Y / position->scale_y;
    double processed_sc_div_x = 1.0;
    double processed_sc_div_y = 1.0;
    double decimal_place_x = Place(sc_div_x, &processed_sc_div_x);            //processed_sc_div_x is being changed to 0.5/1.0/2.0
    double decimal_place_y = Place(sc_div_y, &processed_sc_div_y);            //processed_sc_div_y is being changed to 0.5/1.0/2.0

    axes_sc->dgt_step_x = processed_sc_div_x * pow(10.0, decimal_place_x);
    axes_sc->pxl_step_x = axes_sc->dgt_step_x * position->scale_x;
    axes_sc->anchor_value_x = floor(position->x_vertex_value / axes_sc->dgt_step_x) * axes_sc->dgt_step_x;
    axes_sc->anchor_point_x = position->pxl_vertex_x - (position->x_vertex_value - axes_sc->anchor_value_x) * position->scale_x;

    axes_sc->dgt_step_y = processed_sc_div_y * pow(10.0, decimal_place_y);
    axes_sc->pxl_step_y = axes_sc->dgt_step_y * position->scale_y;
    axes_sc->anchor_value_y = 0.0;
    axes_sc->anchor_point_y = position->abscissa_position;
}


void MakeBlackWindow() {
    txCreateWindow(WINDOW_SIZE_X, WINDOW_SIZE_Y);                                    // make black screen;
    txSetFillColor(TX_BLACK);
    txClear();
}


void CancelConsolePrinting() {
    txClearConsole();
    txTextCursor(false);
    txSetConsoleAttr(FOREGROUND_BLACK);
    txSetConsoleAttr(BACKGROUND_BLACK);
}


void DrawGrid(const Axis_scale* axes_sc) {
    assert(axes_sc != NULL);

    double small_square_step_x = axes_sc->pxl_step_x / SHORT_HATCH_PER_LONG;
    double small_square_step_y = axes_sc->pxl_step_y / SHORT_HATCH_PER_LONG;

    txSetColor(RGB(50, 40, 55), GRID_THICKNESS);                                     // small square
    DoXYAcrossEntireScreenRange(pixel, axes_sc->anchor_point_, small_square_step_,
        txLine(point_x, 0.0, point_x, WINDOW_SIZE_Y);,
        txLine(0.0, point_y, WINDOW_SIZE_X, point_y);
    );

    txSetColor(RGB(65, 55, 70), GRID_THICKNESS);                                     // big square
    DoXYAcrossEntireScreenRange(pixel, axes_sc->anchor_point_, axes_sc->pxl_step_,
        txLine(point_x, 0.0, point_x, WINDOW_SIZE_Y);,
        txLine(0.0, point_y, WINDOW_SIZE_X, point_y);
    );
}


#define DrawAbscissaHatches(condition, sign_1, sign_2)                                                                                       \
    value_x = axes_sc->anchor_value_x;                                                                                                       \
    for (pixel point_x = axes_sc->anchor_point_x; condition; point_x sign_1##= axes_sc->pxl_step_x, value_x sign_2##= axes_sc->dgt_step_x) { \
        if (!(CmpEpsPrec(value_x, 0.0))) {                                                                                                   \
            txLine(point_x, position->abscissa_position - LONG_HATCH_SIZE, point_x, position->abscissa_position + LONG_HATCH_SIZE);          \
            sprintf(value_str, "%lg", value_x);                                                                                              \
            txTextOut(point_x, position->abscissa_position - OFFSET, value_str);                                                             \
        }                                                                                                                                    \
        else if (point_x >= DEFAULT_ORDINATE_POSITION - EPSILON &&                                                                           \
                 point_x <= WINDOW_SIZE_X - DEFAULT_ORDINATE_POSITION + EPSILON)                                                             \
            txCircle(point_x, position->abscissa_position, ORIGIN_POINT_SIZE);                                                               \
    }

#define DrawOrdinateHatches(condition, sign_1, sign_2)                                                                                       \
    value_y = axes_sc->anchor_value_y;                                                                                                       \
    for (pixel point_y = axes_sc->anchor_point_y; condition; point_y sign_1##= axes_sc->pxl_step_y, value_y sign_2##= axes_sc->dgt_step_y) { \
        txLine(position->ordinate_position - LONG_HATCH_SIZE, point_y, position->ordinate_position + LONG_HATCH_SIZE, point_y);              \
        sprintf(value_str, "%lg", value_y);                                                                                                  \
        txTextOut(position->ordinate_position + OFFSET * offsetDirection, point_y + TEXT_SIZE / 2, value_str);                               \
    }

void DrawAxis(const Axis_scale* axes_sc, const Position_parameters* position) {
    assert(axes_sc != NULL);
    assert(position != NULL);

    char value_str[MAX_NUMBER_LENGTH] = {};

    txSetColor(TX_YELLOW, AXIS_THICKNESS);                                           // axes
    txLine(0.0, position->abscissa_position, WINDOW_SIZE_X, position->abscissa_position);
    txLine(position->ordinate_position, 0.0, position->ordinate_position, WINDOW_SIZE_Y);

    double small_square_step_x = axes_sc->pxl_step_x / SHORT_HATCH_PER_LONG;
    double small_square_step_y = axes_sc->pxl_step_y / SHORT_HATCH_PER_LONG;

    txSetColor(TX_MAGENTA, SHORT_HATCH_THICKNESS);                                   // short hatches
    DoXYAcrossEntireScreenRange(pixel, axes_sc->anchor_point_, small_square_step_,
        txLine(point_x, position->abscissa_position - SHORT_HATCH_SIZE, point_x, position->abscissa_position + SHORT_HATCH_SIZE);,
        txLine(position->ordinate_position - SHORT_HATCH_SIZE, point_y, position->ordinate_position + SHORT_HATCH_SIZE, point_y);
    );

    txSetFillColor(TX_MAGENTA);                                                      // long hatches and values
    txSetColor(TX_MAGENTA, LONG_HATCH_THICKNESS);
    txSetTextAlign(TA_CENTER|TA_BOTTOM);
    txSelectFont("Arial", TEXT_SIZE);

    double value_x = 0;
    DrawAbscissaHatches(point_x <= WINDOW_SIZE_X, +, +);
    DrawAbscissaHatches(point_x >= 0.0, -, -);

    txSetTextAlign(TA_LEFT|TA_BOTTOM);                                               // ordinate hatches
    double offsetDirection = 1.0;
    if (position->ordinate_position >= WINDOW_SIZE_X - MIN_ORDINATE_DISTANCE) {
        txSetTextAlign(TA_RIGHT|TA_BOTTOM);
        offsetDirection = -1.0;
    }
    double value_y = 0;

    DrawOrdinateHatches(point_y >= 0.0, -, +);
    DrawOrdinateHatches(point_y <= WINDOW_SIZE_Y, +, -);
}


#define DrawBranch(sign_x, sign_y)                                                                   \
x = position->pxl_vertex_x;                                                                          \
y = position->pxl_vertex_y;                                                                          \
for(int step_number = 0; step_number < STEPS_COUNT && y < WINDOW_SIZE_Y && y > 0.0; step_number++) { \
    const Derivative_parameters der_par = {coef, x, position->pxl_vertex_x, position->scale_x};               \
    delta_y = DELTA_X * (position->scale_y / position->scale_x) * Derivative(&der_par);              \
    txLine(x, y, x sign_x DELTA_X, y sign_y delta_y);                                                \
                                                                                                     \
    x sign_x##= DELTA_X;                                                                             \
    y sign_y##= delta_y;                                                                             \
}

void DrawCurve(const Quadratic_coefficients* coef, const Position_parameters* position,
               double Derivative(const Derivative_parameters*)) {
    assert(coef != NULL);
    assert(position != NULL);
    assert(Derivative);

    txSetFillColor(TX_RED);
    txSetColor(TX_RED, CURVE_THICKNESS);
    pixel delta_y = 0.0;

    pixel x = 0.0, y = 0.0;

    DrawBranch(+, -);               // right_branch
    DrawBranch(-, +);               // left_branch

    if (!IsNAN(coef->c)) {
        double discr = coef->b * coef->b - 4.0 * coef->a * coef->c;
        if (CmpEpsPrec(discr, 0.0))     // roots
            txCircle(position->abscissa_position, position->pxl_vertex_y, ROOT_POINT_SIZE);
        else if (discr > 0.0) {
            double ver_root_distance = sqrt(discr) / 2 / coef->a;
            txCircle(position->pxl_vertex_x + ver_root_distance * position->scale_x, position->abscissa_position, ROOT_POINT_SIZE);
            txCircle(position->pxl_vertex_x - ver_root_distance * position->scale_x, position->abscissa_position, ROOT_POINT_SIZE);
        }
    }
    else if (!CmpEpsPrec(coef->a, 0.0))
        txCircle(position->ordinate_position + (-coef->b / coef->a) * position->scale_x,
                 position->abscissa_position, ROOT_POINT_SIZE);
}


double QuadraticEqualDerivative (const Derivative_parameters* der_par) {
    assert(der_par != NULL);
    assert(der_par->coef != NULL);

    double real_x = (der_par->current_x - der_par->pxl_vertex_x) / der_par->scale_x;

    return 2.0 * der_par->coef->a * real_x;
}


double LinearEqualDerivative (const Derivative_parameters* der_par) {
    assert(der_par != NULL);
    assert(der_par->coef != NULL);

    return der_par->coef->a;
}


int Place(double full, double* p_mantissa) {
    assert(p_mantissa != NULL);

    int place = 0;

    if (full < 0.32) {
        while (full < 0.32) {
            full *= 10.0;
            place--;
        }
    }
    else if (full > 3.2) {
        while (full > 3.2) {
            full /= 10.0;
            place++;
        }
    }

    *p_mantissa = Closest05_1_2(full); // TODO print x, unit and y, unit on graphic
    return place;
}


double Closest05_1_2(const double x) {
    if (x >= 0.71 && x <= 1.41)
        return 1.0;
    else if (x < 0.71)
        return 0.5;
    else /* (x > 1.41) */
        return 2.0;
}
