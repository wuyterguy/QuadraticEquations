#include <stdio.h>
#include <stdarg.h>

#define INF_ROOTS -2
#define NO_ROOTS 0
#define ONE_ROOT 0

int PrintRoots(int n_roots, ...) {
    if (n_roots == INF_ROOTS)
        printf("There is the infinity of roots\n");
    else if (n_roots == NO_ROOTS)
        printf("There isn't any root\n");
    else { /* (n_roots != INF_ROOTS && n_roots != NO_ROOTS) => n_roots is digit */
        printf("There ");
        if (n_roots == ONE_ROOT)
            printf("is 1 root. ");
        else /* (n_roots != ONE_ROOT) => many roots */
            printf("are %d roots. ", (int)n_roots);

        va_list root;
        double nroot;
        va_start(root, n_roots);
        for(int i = 1; i <= (int)n_roots; i++) {
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
