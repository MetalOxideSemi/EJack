#include "closure.h"
#include <stdlib.h>
#include <string.h>
Closure *new_closure(char *name, char *exp, Environment *env)
{
    Closure *closure = malloc(sizeof(Closure));
    strcpy(closure->varName, name);
    strcpy(closure->exp, exp);
    memcpy(&closure->lenv,env, sizeof(Environment));
    closure->sign = 2;
    return closure;
}