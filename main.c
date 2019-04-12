#include "interpret.h"
#include "environment.h"
#include "numeric.h"
#include "closure.h"
#include "scientific.h"
#include <stdio.h>
#include <stdlib.h>

#define QUIT_SIGN "\\q"
static char input[65536];
void Jack(char *input, Environment *env);
//Numeric result;
int main(int argc, char **argv)
{
    
    global_table_init();
    Numeric_global_init();
    Environment env0;
    new_env(&env0);
    //Numeric_new(&result);

    if (argc == 1)
    {
        puts("EJack Calstack Version 0.1");
        puts("Enjoy Programmable, Functional, High-precision Scientific Calculator!");
        puts("Type " QUIT_SIGN " to quit");
        while (1)
        {
            printf(">>> ");
            gets(input);
            if (input[0] != '\0')
            {
                if (!strcmp(input, QUIT_SIGN))
                {
                    break;
                }
                Jack(input, &env0);
            }
        }
    }
    else
    {
        FILE *source;
        char *pos = strchr(argv[1], '.');
        if (!pos)
        {
            char *fnm = malloc(strlen(argv[1]) + 3);
            strcpy(fnm, argv[1]);
            strcat(fnm, ".ej");
            source = fopen(fnm, "r");
        }
        else
        {
            source = fopen(argv[1], "r");
        }
        while (!feof(source))
        {
            fgets(input, 65536, source);
            if (input[0] != '\0')
            {
                if (!strcmp(input, QUIT_SIGN))
                {
                    break;
                }
                Jack(input, &env0);
            }
        }
    }
}

void Jack(char *input, Environment *env)
{
    void *ptr = NULL;

    ptr = interpret(input, env);

    if (*(int *)ptr == 2)
    {
        printf("-> Closure: $%s{%s}\n", ((Closure *)ptr)->varName, ((Closure *)ptr)->exp);
    }
    else
    {
        printf("-> ");
        puts(Numerictostr(ptr));
    }
}