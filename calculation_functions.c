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
    if ((fabs(a - b) <= EPSILON) ||
        (IsNAN(a) && IsNAN(b)))
        return true;
    else
        return false;
}


bool IsNAN(double lf) {
    bool result = false;
    unsigned char* pointer_to_start = (unsigned char*)&lf + sizeof(double);

    for (unsigned char* pointer_to_bite = pointer_to_start - 3;
         pointer_to_bite >= pointer_to_start - 8; pointer_to_bite--) {
        if (*pointer_to_bite != 0b00000000){                  // 13rd - 64th bites must contain
            result = true;                                    // at least one bit with 1 value
            break;
        }
    }

    if ((unsigned char)(*(pointer_to_start - 2) << 4) != 0)                  // 13rd - 64th bites must contain
        result = true;                                                       // at least one bit with 1 value

    if (((unsigned char)(*(pointer_to_start - 1) << 1) != 0b11111110) ||     // 1st bite must be ?1111111
        ((unsigned char)(*(pointer_to_start - 2) >> 4) != 0b00001111))       // 2nd bite must be 1111????
        result = false;

    return result;
}
