// in task you need to use point_
#define DoAcrossEntireScreenRange(lo_coord, up_coord, type_start, start, step,         \
                                  preliminary_task, intermediate_task, task)           \
preliminary_task                                                                       \
for (type_start point_##lo_coord = start##lo_coord;                                    \
     point_##lo_coord <= WINDOW_SIZE_##up_coord; point_##lo_coord += step##lo_coord) { \
    task                                                                               \
}                                                                                      \
intermediate_task                                                                      \
for (type_start point_##lo_coord = start##lo_coord;                                    \
     point_##lo_coord >= 0.0; point_##lo_coord -= step##lo_coord) {                    \
    task                                                                               \
}

#define DoXYAcrossEntireScreenRange(type_start, start, step,                         \
                                    preliminary_task_x, intermediate_task_x, task_x, \
                                    preliminary_task_y, intermediate_task_y, task_y) \
    DoAcrossEntireScreenRange(x, X, type_start, start, step,                         \
                              preliminary_task_x, intermediate_task_x, task_x)       \
    DoAcrossEntireScreenRange(y, Y, type_start, start, step,                         \
                              preliminary_task_y, intermediate_task_y, task_y)


void DrawParabola(const Quadratic_coefficients* coef) {
    assert(coef != NULL);
    assert(!CmpEpsPrec(coef->a, 0.0));

    Position_parameters parabola = {};
    GetParabolaPosition(coef, &parabola);

    DrawGraphic(coef, &parabola, QuadraticEqualDerivative);
}


void GetParabolaPosition(const Quadratic_coefficients* coef, Position_parameters* parabola) {
    assert(coef != NULL);
    assert(parabola != NULL);

    double discr = coef->b * coef->b - 4.0 * coef->a * coef->c;
    double y_vertex_value = coef->c - coef->b * coef->b / 4.0 / coef->a;
    double x_vertex_value = -coef->b / 2.0 / coef->a;

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
        parabola->scale_y = WINDOW_QUARTER_Y / fabs(y_vertex_value);                     // height of pxl_vertex
        parabola->pxl_vertex_y = parabola->abscissa_position - y_vertex_value * parabola->scale_y;
    }
    else { /* (discr < 0) */
        parabola->abscissa_position = fmod(WINDOW_SIZE_Y - Sign(coef->a) * WINDOW_QUARTER_Y, WINDOW_SIZE_Y);
        parabola->scale_y = WINDOW_QUARTER_Y / fabs(y_vertex_value);                     // height of pxl_vertex
        parabola->scale_x = sqrt(parabola->scale_y * fabs(coef->a) * WINDOW_QUARTER_Y);  // beauty
        parabola->pxl_vertex_y = parabola->abscissa_position - y_vertex_value * parabola->scale_y;
    }

    if ((WINDOW_HALF_X - DEFAULT_ORDINATE_POSITION) / parabola->scale_x > fabs(x_vertex_value))
        parabola->ordinate_position = WINDOW_HALF_X - x_vertex_value * parabola->scale_x;
    else /* ordinate isn't into screen */
        parabola->ordinate_position = fmod(WINDOW_SIZE_X + Sign(x_vertex_value) * DEFAULT_ORDINATE_POSITION,
                                 WINDOW_SIZE_X);
}


void DrawGraphic(const Quadratic_coefficients* coef, const Position_parameters* position,
                 double Derivative(const Quadratic_coefficients*, const pixel, const pixel, const pixel)) {
    assert(coef != NULL);
    assert(position != NULL);
    assert(Derivative);

    Axis_scale axis_sc = {};
    GetAxisScale(coef, position, &axis_sc);

    MakeBlackWindow();

    CancelConsolePrinting();

    DrawGrid(&axis_sc);

    DrawAxis(&axis_sc, position);

    DrawCurve(coef, position, Derivative);
}


void GetAxisScale(const Quadratic_coefficients* coef, const Position_parameters* position,
                  Axis_scale* axis_sc) {
    assert(coef != NULL);
    assert(position != NULL);
    assert(axis_sc != NULL);

    double x_vertex_value = -coef->b / 2.0 / coef->a;
    double sc_div_x = WINDOW_SIZE_X / SCALE_DIVISION_COUNT_X / position->scale_x;
    double sc_div_y = WINDOW_SIZE_Y / SCALE_DIVISION_COUNT_Y / position->scale_y;
    double processed_sc_div_x = 1.0;
    double processed_sc_div_y = 1.0;
    double decimal_place_x = GetPlace(sc_div_x, &processed_sc_div_x);            //processed_sc_div_x is being changed to 0.5/1.0/2.0
    double decimal_place_y = GetPlace(sc_div_y, &processed_sc_div_y);            //processed_sc_div_y is being changed to 0.5/1.0/2.0

    axis_sc->dgt_step_x = processed_sc_div_x * pow(10.0, decimal_place_x);
    axis_sc->pxl_step_x = axis_sc->dgt_step_x * position->scale_x;
    axis_sc->anchor_value_x = floor(x_vertex_value / axis_sc->dgt_step_x) * axis_sc->dgt_step_x;
    axis_sc->anchor_point_x = position->pxl_vertex_x - (x_vertex_value - axis_sc->anchor_value_x) * position->scale_x;

    axis_sc->dgt_step_y = processed_sc_div_y * pow(10.0, decimal_place_y);
    axis_sc->pxl_step_y = axis_sc->dgt_step_y * position->scale_y;
    axis_sc->anchor_value_y = 0.0;
    axis_sc->anchor_point_y = position->abscissa_position;
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


void DrawGrid(const Axis_scale* axis_sc) {
    assert(axis_sc != NULL);

    double small_square_step_x = axis_sc->pxl_step_x / SHORT_HATCH_PER_LONG;
    double small_square_step_y = axis_sc->pxl_step_y / SHORT_HATCH_PER_LONG;

    txSetColor(RGB(50, 40, 55), GRID_THICKNESS);                                     // small square
    DoXYAcrossEntireScreenRange(pixel, axis_sc->anchor_point_, small_square_step_, , ,
        txLine(point_x, 0.0, point_x, WINDOW_SIZE_Y);, , ,
        txLine(0.0, point_y, WINDOW_SIZE_X, point_y);
    );

    txSetColor(RGB(65, 55, 70), GRID_THICKNESS);                                     // big square
    DoXYAcrossEntireScreenRange(pixel, axis_sc->anchor_point_, axis_sc->pxl_step_, , ,
        txLine(point_x, 0.0, point_x, WINDOW_SIZE_Y);, , ,
        txLine(0.0, point_y, WINDOW_SIZE_X, point_y);
    );
}


void DrawAxis(const Axis_scale* axis_sc, const Position_parameters* position) {
    assert(axis_sc != NULL);
    assert(position != NULL);

    char value_str[MAX_NUMBER_SIZE] = {};

    txSetColor(TX_YELLOW, AXIS_THICKNESS);                                           // axis
    txLine(0.0, position->abscissa_position, WINDOW_SIZE_X, position->abscissa_position);
    txLine(position->ordinate_position, 0.0, position->ordinate_position, WINDOW_SIZE_Y);

    double small_square_step_x = axis_sc->pxl_step_x / SHORT_HATCH_PER_LONG;
    double small_square_step_y = axis_sc->pxl_step_y / SHORT_HATCH_PER_LONG;

    txSetColor(TX_MAGENTA, SHORT_HATCH_THICKNESS);                                   // short hatches
    DoXYAcrossEntireScreenRange(pixel, axis_sc->anchor_point_, small_square_step_, , ,
        txLine(point_x, position->abscissa_position - SHORT_HATCH_SIZE, point_x, position->abscissa_position + SHORT_HATCH_SIZE);, , ,
        txLine(position->ordinate_position - SHORT_HATCH_SIZE, point_y, position->ordinate_position + SHORT_HATCH_SIZE, point_y);
    );

    txSetFillColor(TX_MAGENTA);                                                      // long hatches and values
    txSetColor(TX_MAGENTA, LONG_HATCH_THICKNESS);
    txSelectFont("Arial", TEXT_SIZE);

    DoXYAcrossEntireScreenRange(pixel, axis_sc->anchor_point_, axis_sc->pxl_step_,
        txSetTextAlign(TA_CENTER|TA_BOTTOM);
        double value_x = axis_sc->anchor_value_x;
        int bypass = 1;,

        value_x = axis_sc->anchor_value_x;
        bypass = -1;,

        if (!(CmpEpsPrec(value_x, 0.0))) {
            txLine(point_x, position->abscissa_position - LONG_HATCH_SIZE, point_x, position->abscissa_position + LONG_HATCH_SIZE);
            sprintf(value_str, "%lg", value_x);
            txTextOut(point_x, position->abscissa_position - OFFSET, value_str);
        }
        else if (point_x >= DEFAULT_ORDINATE_POSITION - EPSILON &&
                 point_x <= WINDOW_SIZE_X - DEFAULT_ORDINATE_POSITION + EPSILON)
            txCircle(point_x, position->abscissa_position, ORIGIN_POINT_SIZE);
        value_x += (axis_sc->dgt_step_x * bypass);,

        txSetTextAlign(TA_LEFT|TA_BOTTOM);
        double offsetDirection = 1.0;
        if (position->ordinate_position >= WINDOW_SIZE_X - MIN_ORDINATE_DISTANCE) {
            txSetTextAlign(TA_RIGHT|TA_BOTTOM);
            offsetDirection = -1.0;
        }
        double value_y = axis_sc->anchor_value_y;
        bypass = 1;,

        value_y = axis_sc->anchor_value_y;
        bypass = -1;,

        txLine(position->ordinate_position - LONG_HATCH_SIZE, point_y, position->ordinate_position + LONG_HATCH_SIZE, point_y);
        sprintf(value_str, "%lg", value_y);
        txTextOut(position->ordinate_position + OFFSET * offsetDirection, point_y + TEXT_SIZE / 2, value_str);
        value_y -= (axis_sc->dgt_step_y * bypass);
    );
}


#define DrawBranch(sign_x, sign_y)                                                                                                \
x = position->pxl_vertex_x;                                                                                                       \
y = position->pxl_vertex_y;                                                                                                       \
for(int step_number = 0; step_number < STEPS_COUNT && y < WINDOW_SIZE_Y && y > 0.0; step_number++) {                              \
    delta_y = DELTA_X * (position->scale_y / position->scale_x) * Derivative(coef, x, position->pxl_vertex_x, position->scale_x); \
    txLine(x, y, x sign_x DELTA_X, y sign_y delta_y);                                                                             \
                                                                                                                                  \
    x sign_x##= DELTA_X;                                                                                                        \
    y sign_y##= delta_y;                                                                                                        \
}

void DrawCurve(const Quadratic_coefficients* coef, const Position_parameters* position,
               double Derivative(const Quadratic_coefficients*, const pixel, const pixel, const pixel)) {
    assert(coef != NULL);
    assert(position != NULL);
    assert(Derivative);

    txSetFillColor(TX_RED);
    txSetColor(TX_RED, CURVE_THICKNESS);
    double discr = coef->b * coef->b - 4.0 * coef->a * coef->c;
    pixel delta_y = 0.0;

    pixel x = 0.0, y = 0.0;

    DrawBranch(+, -);               // right_branch
    DrawBranch(-, +);               // left_branch

    if (CmpEpsPrec(discr, 0.0))     // roots
        txCircle(position->abscissa_position, position->pxl_vertex_y, ROOT_POINT_SIZE);
    else if (discr > 0.0) {
        double ver_root_distance = sqrt(discr) / 2 / coef->a;
        txCircle(position->pxl_vertex_x + ver_root_distance * position->scale_x, position->abscissa_position, ROOT_POINT_SIZE);
        txCircle(position->pxl_vertex_x - ver_root_distance * position->scale_x, position->abscissa_position, ROOT_POINT_SIZE);
    }
}


double QuadraticEqualDerivative (const Quadratic_coefficients* coef, const pixel position_x,
                                 const pixel pxl_vertex_x, const pixel scale_x) {
    assert(coef != NULL);

    double real_x = (position_x - pxl_vertex_x) / scale_x;

    return 2.0 * coef->a * real_x;
}


double GetPlace(double full, double* p_mantissa) {
    assert(p_mantissa != NULL);

    double place = 0.0;

    if (full < 0.32) {
        while (full < 0.32) {
            full *= 10.0;
            place -= 1.0;
        }
    }
    else if (full > 3.2) {
        while (full > 3.2) {
            full /= 10.0;
            place += 1.0;
        }
    }

    *p_mantissa = GetClosest05_1_2(full);     // TODO print x, unit and y, unit on graphic
    return place;
}


double GetClosest05_1_2(const double x) {
    if (x >= 0.71 && x <= 1.41)
        return 1.0;
    else if (x < 0.71)
        return 0.5;
    else /* (x > 1.41) */
        return 2.0;
}
