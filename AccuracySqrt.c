#include <stdio.h>
#include <assert.h>

/* AccuracyComparison: comperes a and b.
Return -1 if a < b, 1 if a > b, 0 if a = b */
int AccuracyComparison(double a, double b, double accuracy);

double AccuracySqrt(double radicand, double accuracy) {
    assert(radicand>=0);

    double low, high;
    int cmp_result = AccuracyComparison(radicand, 1.0, accuracy);
    switch(cmp_result) {
        case 0:
            return 1;
            break;
        case -1:
            low = radicand, high = 1;
            break;
        case 1:
            low = 1, high = radicand;
            break;
    }

    double lmiddle = 0, middle;
    //int k = 0;                            counter of steps
    while (AccuracyComparison(low, high, accuracy)) {
        //k++;                              counter of steps
        middle = (high + low) / 2;
        if (middle == lmiddle) {
            fprintf(stderr, "AccuracySqrt: Warning: too high accuracy\n");
            break;
        }
        if (middle * middle <= radicand)
            low = middle;
        else /* (middle * middle > radicant) */
            high = middle;

        lmiddle = middle;                  // remember last value of middle
    }

    //printf("%d\n", k);                    counter of steps
    return low;
}


int AccuracyComparison(double a, double b, double accuracy) {
    if (a <= b + accuracy && a >= b - accuracy)
        return 0;
    else if (a < b)
        return -1;
    else /* a > b */
        return 1;
}

int main() {
    printf("%lf", AccuracySqrt(2.0, 0.05));
}

// ?static into a recursion

// ?must library function consist of basic functions

// ?is initialization necessary

// ?space of vision in case
