#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdalign.h>

struct typedata{size_t size; size_t align;};

/* tdataof(type): инициализатор для typedata, содержит размер и выравнивание типа данных type */
#define tdataof(type) {sizeof(type), alignof(type)}

/* printstructp: рассчитывает адреса всех членов struct по их типам данных */
/* n - количество членов, *p - адрес первого члена */
void printstructp(int n, void *p, ...){
    size_t sizeLast, alignNow;
    va_list ap;
    struct typedata td;
    va_start(ap, p);
    td = va_arg(ap, struct typedata);
    sizeLast = td.size;
    printf("%p", p);
    while(--n){
        p += sizeLast;
        /* p теперь адрес сразу после конца прошлой ячейки*/
        td = va_arg(ap, struct typedata);
        alignNow = td.align;
        p = (void *) (((uintptr_t)p + alignNow - 1) & ~(alignNow - 1));
        /* p теперь ближайий после конца прдыдущей ячейки адрес, выравненный по требованию текущей ячейки */
        printf(" %p", p);
        sizeLast = td.size;
    }
    va_end(ap);
}

int main() {
    // Разнообразные типы днных:
    typedef struct gg1 {float a; int *b; int c;} GG1;
    typedef union gg2 {int a; float b; char *c;} GG2;
    //<

    // Данные для тестирования:
    struct test{int a; long double b; GG1 c; char *d; GG2 e; GG1 *f;} Test;
    struct typedata a = tdataof(typeof(Test.a)), b = tdataof(typeof(Test.b)), c = tdataof(typeof(Test.c));
    struct typedata d = tdataof(typeof(Test.d)), e = tdataof(typeof(Test.e)), f = tdataof(typeof(Test.f));
    //<

    printf("%p %p %p %p %p %p\n", &Test.a, &Test.b, &Test.c, &Test.d, &Test.e, &Test.f);
    printstructp(6, &Test, a, b, c, d, e, f);
};
