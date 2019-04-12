#ifndef _ENVIRONMENT_H_
#define _ENVIRONMENT_H_
#define VAR_NUM 128
#define NAME_LENTH 16
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "numeric.h"
#include "scientific.h"
extern Numeric PI;
typedef struct
{
    char name[NAME_LENTH];
    void *value;
} item;

typedef struct
{
    item var[VAR_NUM];
    int head;
} Environment;

void new_env(Environment *env);
Environment *ext_env(char *name, void *value, Environment *env);//new
Environment *ext_env_letrec(char *name, void *value, Environment *env);//old
void *lookup_env(char *name, Environment *env);

#endif