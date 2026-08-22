Roots_c SolveQuadratic(const double coef_a, const double coef_b, const double coef_c,
                       double* const p_root1, double* const p_root2) {
    assert(p_root1 != NULL);
    assert(p_root2 != NULL);
    assert(p_root1 != p_root2);

    if (CmpEpsPrec(coef_a, 0.0)) {
        return SolveLinear(coef_b, coef_c, p_root1);
    }
    else { /* (coef_a != 0.0) => it's quadric education */
        double discr = coef_b*coef_b - 4*coef_a*coef_c;
        if (CmpEpsPrec(discr, 0.0)) {
            *p_root1 = DivideSmart(-coef_b, 2*coef_a);
            return ONE_ROOT;
        }
        else if (discr > 0.0) {
            double sq_discr = sqrt(discr);
            *p_root1 = DivideSmart(-coef_b + sq_discr, 2*coef_a);
            *p_root2 = DivideSmart(-coef_b - sq_discr, 2*coef_a);
            return TWO_ROOTS;
        }
        else { /* (discr < 0.0) => there is no solutions */
            return NO_ROOTS;
        }
    }
}


Roots_c SolveLinear(const double coef_a, const double coef_b, double* const p_root) {
    assert(p_root != NULL);

    if (CmpEpsPrec(coef_a, 0.0)) {
        if (CmpEpsPrec(coef_b, 0.0))
            return INF_ROOTS;
        else /* (coef_c != 0.0) */
            return NO_ROOTS;
    }
    else { /* (coef_b != 0.0) */
        *p_root = DivideSmart(-coef_b, coef_a);
        return ONE_ROOT;
    }
}


double DivideSmart(const double dividend, const double divider) {
    assert(!CmpEpsPrec(divider, 0.0));

    if (!CmpEpsPrec(dividend, 0.0))
        return dividend / divider;
    else
        return 0.0;
}


bool CmpEpsPrec(const double a, const double b) {
    if (((a <= b + EPSILON) && (a >= b - EPSILON)) || (isnan(a) && isnan(b)))
        return true;
    else
        return false;
}
