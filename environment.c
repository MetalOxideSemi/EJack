#include "environment.h"

void new_env(Environment *env)
{
    env->head = 0;
    for (int i = 0; i < VAR_NUM; i++)
    {
        env->var[i].value = NULL;
        memset(env->var[i].name, 0, sizeof(env->var[i].name));
    }
    env->var[env->head].value =&PI;
    strcpy(env->var[env->head].name,"PI");
    ++env->head;


}

Environment *ext_env_letrec(char *name, void *value, Environment *env)
 {
     strcpy(env->var[env->head].name, name);
     env->var[env->head].value = value;
     ++env->head;
     return env;
 }
Environment* ext_env(char *name, void *value, Environment *env)
{
    Environment* lenv = malloc(sizeof(Environment));
    memcpy(lenv, env, sizeof(Environment));

    strcpy(lenv->var[lenv->head].name, name);
    lenv->var[lenv->head].value = value;
    ++lenv->head;
    return lenv;
}

void *lookup_env(char *name, Environment *env)
{

    for (int i = env->head - 1; i >= 0; i--)
    {
        if (!strcmp(env->var[i].name, name))
        {
            return env->var[i].value;
        }
    }
    return NULL;
}