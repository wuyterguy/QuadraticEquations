//IEEE754
#include <stdio.h>
#include <math.h>

bool IsNAN(const double lf) {
    bool result = false;
    unsigned char* pointer_to_start = (unsigned char*)&lf + sizeof(double);

    for (unsigned char* pointer_to_bite = pointer_to_start - 3;
         pointer_to_bite >= pointer_to_start - 8; pointer_to_bite--) {
        if (*pointer_to_bite != 0b00000000){                  // 13rd - 64th bites must contain
            result = true;                                    // at least one bit with 1 value
            break;
        }
    }

    if ((unsigned char)(*(pointer_to_start - 2) << 5) != 0)                  // 13rd - 64th bites must contain
        result = true;                                                       // at least one bit with 1 value

    printf("%d ", *(pointer_to_start - 2));
    printf("%d ", (*(pointer_to_start - 1)) << 1);
    if (((unsigned char)(*(pointer_to_start - 1) << 1) != 0b11111110) ||     // 1st bite must be ?1111111
        ((unsigned char)(*(pointer_to_start - 2) >> 4) != 0b00001111))       // 2nd bite must be 1111????
        result = false;

    return result;
}

int main() {
    double x = 2;
    if (isnan(NAN))
        printf("Is NAN\n");
    else
        printf("Isn't NAN\n");
}
