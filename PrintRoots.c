#include <stdio.h>
#include <stdarg.h>

#define INFROOTS -2
#define NOROOTS 0
#define ONEROOT 0

int PrintRoots(int nroots, ...) {
    if (nroots == INFROOTS)
        printf("There is the infinity of roots\n");
    else if (nroots == NOROOTS)
        printf("There isn't any root\n");
    else { /* (nroots != INFROOTS && nroots != NOROOTS) => nroots is digit */
        printf("There ");
        if (nroots == ONEROOT)
            printf("is 1 root. ");
        else /* (nroots != ONEROOT) => many roots */
            printf("are %d roots. ", (int)nroots);

        va_list root;
        double nroot;
        va_start(root, nroots);
        for(int i = 1; i <= (int)nroots; i++) {
            nroot = va_arg(root, double);
            printf("x%d: %.4lf; ", i, nroot);
        }
        va_end(root);
    }

    return 0;
}

/*int main(){
    PrintRoots(6, 23.23432, (double)56, (double)34, 64.34, 10.34, 93.5);
}*/
