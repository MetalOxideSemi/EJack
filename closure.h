#include "environment.h"
#ifndef BUFFER_LENTH
#define BUFFER_LENTH 32768
#endif
typedef struct Closure {
    int sign;
    char varName[NAME_LENTH];
    char exp[BUFFER_LENTH];
    Environment lenv;
} Closure;

Closure *new_closure(char *name, char *exp, Environment *env);
