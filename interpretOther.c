#include "interpretOther.h"
bool isLetrec(char *input)
{
    int j = 0;
    while (input[j] == ' ')
    {
        ++j;
    }

    if (strlen(input + j) >= 6 && !strncmp(input + j, "letrec", 6))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

bool isLet(char *input)
{
    int j = 0;
    while (input[j] == ' ')
    {
        ++j;
    }

    if (strlen(input + j) >= 3 && !strncmp(input + j, "let", 3))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

bool isIf(char *input)
{
    int j = 0;
    while (input[j] == ' ')
    {
        ++j;
    }

    if (strlen(input + j) >= 2 && !strncmp(input + j, "if", 2))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

bool isNum(char *input)
{

    int j = 0;

    while (input[j] == ' ')
    {
        ++j;
    }

    if (input[j] != '-' && input[j] != '+' && !isdigit(input[j]))
    {
        return 0;
    }
    if (input[j] == '-' || input[j] == '+')
        ++j;
    while (input[j] != '\0' && input[j] != ';' && input[j] != '\n')
    {
        if (input[j] == ' ' && input[j + 1] != ' ' && input[j + 1] != '\n' && input[j + 1] != '\0' &&
            input[j + 1] != ';' && input[j + 1] != EOF)
        {
            return 0;
        }

        if (!isdigit(input[j]) && input[j] != '.')
        {
            return 0;
        }

        ++j;
    }
    return 1;
}

bool isAlphaExp(char *input)
{
    int j = 0;

    while (input[j] == ' ')
    {
        j++;
    }

    while (input[j] != '\0' && input[j] != ';' && input[j] != '\n' && input[j] != EOF)
    {
        if (input[j] == ' ' && input[j + 1] != ' ' && input[j + 1] != '\0' && input[j + 1] != ';' &&
            input[j + 1] != '\n' && input[j + 1] != EOF)
        {
            return 0;
        }
        else if (!isalpha(input[j]))
        {
            return 0;
        }
        ++j;
    }
    return 1;
}

bool isClosure(char *input)
{
    int j = 0;
    int endFlag = 0;
    while (input[j] == ' ')
    {
        ++j;
    }

    if (input[j++] != '$')
    {
        return 0;
    }
    while (isalpha(input[j]))
    {
        ++j;
    }

    while (1)
    {
        if (input[j] == '{')
        {
            endFlag++;
        }
        else if (input[j] == '}')
        {
            endFlag--;
        }
        ++j;
        if (endFlag == 0)
        {
            break;
        }
    }

    if (input[j] == '[')
    {
        return 0;
    }

    return 1;
}

bool isCall(char *input)
{
    int j = 0;
    int endFlag = 0;
    while (input[j] == ' ')
    {
        ++j;
    }

    if (input[j] != '$') //common call : f[]
    {
        while (1)
        {
            if (isalpha(input[j]))
            {
                ++j;
            }
            else if (input[j] == '[')
            {
                endFlag = 1;
                while (1)
                {
                    ++j;
                    if (input[j] == '[')
                    {
                        ++endFlag;
                    }
                    else if (input[j] == ']')
                    {
                        --endFlag;
                    }
                    if (endFlag == 0 || input[j + 1] == '[')
                    {
                        break;
                    }
                }
                if (input[j + 1] == '\0' || input[j + 1] == '\n' || input[j + 1] == EOF || input[j + 1] == ';')
                    return 1;
                else
                    return 0;
            }
            else
            {
                return 0;
            }
        }
    }
    else //lambda call : $symbolName{expression1}[expression2]
    {
        ++j;
        while (isalpha(input[j]))
        {
            ++j;
        }

        while (1)
        {
            if (input[j] == '{')
            {
                endFlag++;
            }
            else if (input[j] == '}')
            {
                endFlag--;
            }
            ++j;
            if (endFlag == 0)
            {
                break;
            }
        }

        if (input[j] == '[')
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

void *handleLetrec(char *input, Environment *env)
{
    int i = 0, j = 6;
    char name[NAME_LENTH] = "";
    char exp1[BUFFER_LENTH] = "";
    char exp2[BUFFER_LENTH] = "";
    void *ptr = NULL;
    while (input[j] == ' ')
    {
        ++j;
    }

    //    if (input[j] != '[') //letrec
    //    {
    while (input[j] != '=' && input[j] != ' ')
    {
        name[i++] = input[j++];
    }
    name[i] = '\0';

    while (input[j] == ' ' || input[j] == '=')
    {
        ++j;
    }
    i = 0;
    while (input[j] != '\n' && input[j] != ';' && input[j] != '\0' && input[j] != EOF)
    {
        exp1[i++] = input[j++];
    }
    exp1[i] = '\0';

    if (exp1[i - 1] == ' ') //delete ' ' in the tail of exp1
    {
        int k = i - 1;
        while (exp1[k] == ' ')
        {
            exp1[k--] = '\0';
        }
    }
    ptr = interpret(exp1, env);
    if (*(int *)ptr == 2)
    {
        ext_env_letrec(name, ptr, &((Closure *)ptr)->lenv);
    }
    ext_env_letrec(name, ptr, env);
    return env->var[env->head - 1].value;
    //    }

    // else //letrec in
    // {
    //     int endFlag = 1;
    //     Environment lenv;
    //     memcpy(&lenv, env, sizeof(Environment));
    //     void *ptr = NULL;
    //     void *expPtr = NULL;
    //     ++j;
    //     while (input[j] != '=' && input[j] != ' ')
    //     {
    //         name[i++] = input[j++];
    //     }
    //     name[i] = '\0';

    //     while (input[j] == ' ' || input[j] == '=')
    //     {
    //         ++j;
    //     }

    //     i = 0;
    //     while (1)
    //     {
    //         if (input[j] == '[')
    //         {
    //             ++endFlag;
    //         }
    //         else if (input[j] == ']')
    //         {
    //             --endFlag;
    //         }
    //         if (endFlag == 0)
    //         {
    //             ++j;
    //             break;
    //         }

    //         exp1[i++] = input[j++];
    //     }
    //     exp1[i] = '\0';

    //     while (input[j] == ' ')
    //     {
    //         ++j;
    //     }

    //     if (strncmp(input + j, "in", 2) != 0)
    //     {
    //         printf("Error: not found 'in' ");
    //     }
    //     i = 0;
    //     j += 2;
    //     while (input[j] == ' ')
    //     {
    //         ++j;
    //     }

    //     while (input[j] != '\0' && input[j] != '\n' && input[j] != ';' && input[j] != EOF)
    //     {
    //         exp2[i++] = input[j++];
    //     }

    //     exp2[i] = '\0';
    //     ext_env(name, expPtr = interpret(exp1, &lenv), &lenv);
    //     ptr = interpret(exp2, &lenv);
    //     if (*(int *)ptr == 2)
    //     {

    //         ext_env(name, expPtr, &((Closure *)ptr)->lenv);
    //         ptr = interpret(((Closure *)ptr)->exp, &((Closure *)ptr)->lenv);
    //     }
    //     return ptr;
    // }
}

void *handleLet(char *input, Environment *env)
{
    int i = 0, j = 3;
    char name[NAME_LENTH] = "";
    char exp1[BUFFER_LENTH] = "";
    char exp2[BUFFER_LENTH] = "";
    while (input[j] == ' ')
    {
        ++j;
    }

    if (input[j] != '[') //let
    {
        while (input[j] != '=' && input[j] != ' ')
        {
            name[i++] = input[j++];
        }
        name[i] = '\0';

        while (input[j] == ' ' || input[j] == '=')
        {
            ++j;
        }
        i = 0;
        while (input[j] != '\n' && input[j] != ';' && input[j] != '\0' && input[j] != EOF)
        {
            exp1[i++] = input[j++];
        }
        exp1[i] = '\0';

        if (exp1[i - 1] == ' ') //delete ' ' in the tail of exp1
        {
            int k = i - 1;
            while (exp1[k] == ' ')
            {
                exp1[k--] = '\0';
            }
        }

        ext_env_letrec(name, interpret(exp1, env), env);
        return env->var[env->head - 1].value;
    }
    else //let in
    {
        int endFlag = 1;
        Environment lenv;
        memcpy(&lenv, env, sizeof(Environment));
        void *ptr = NULL;
        void *expPtr = NULL;
        void *localEnv = NULL;
        ++j;
        while (input[j] != '=' && input[j] != ' ')
        {
            name[i++] = input[j++];
        }
        name[i] = '\0';

        while (input[j] == ' ' || input[j] == '=')
        {
            ++j;
        }

        i = 0;
        while (1)
        {
            if (input[j] == '[')
            {
                ++endFlag;
            }
            else if (input[j] == ']')
            {
                --endFlag;
            }
            if (endFlag == 0)
            {
                ++j;
                break;
            }

            exp1[i++] = input[j++];
        }
        exp1[i] = '\0';

        while (input[j] == ' ')
        {
            ++j;
        }

        if (strncmp(input + j, "in", 2) != 0)
        {
            printf("Error: not found 'in' ");
        }
        i = 0;
        j += 2;
        while (input[j] == ' ')
        {
            ++j;
        }

        while (input[j] != '\0' && input[j] != '\n' && input[j] != ';' && input[j] != EOF)
        {
            exp2[i++] = input[j++];
        }

        exp2[i] = '\0';

        ext_env_letrec(name, expPtr = interpret(exp1, &lenv), &lenv);
        ptr = interpret(exp2, &lenv);
        if (*(int *)ptr == 2)
        {

            localEnv = ext_env(name, expPtr, &((Closure *)ptr)->lenv);
            ptr = interpret(((Closure *)ptr)->exp, localEnv);
            free(localEnv);
        }
        return ptr;
    }
}

void *handleIf(char *input, Environment *env) //if exp1 boolSymbol exp2 then exp3 else exp4
{
    int i = 0, j = 0;

    int endFlag = 0;

    void *value1 = NULL;
    void *value2 = NULL;
    Operation boolOperation = CALSTACK_FALSE;

    char exp1[BUFFER_LENTH];
    char exp2[BUFFER_LENTH];
    char exp3[BUFFER_LENTH];
    char exp4[BUFFER_LENTH];

    while (input[j] == ' ')
    {
        ++j;
    }

    if (strncmp(input + j, "if", 2) == 0)
    {

        j += 2;
    }
    else
    {
        printf("Error:bool isIf function is not OK");
    }

    endFlag = 1;
    while (1)
    {
        while (input[j] == ' ')
        {
            ++j;
        }

        if (strncmp(input + j, "if", 2) == 0)
        {

            ++endFlag;
        }
        else if (strncmp(input + j, "then", 4) == 0)
        {

            --endFlag;
        }

        if (endFlag == 1)
        {
            if (input[j] == '>' || input[j] == '<' || !strncmp(input + j, ">=", 2) || !strncmp(input + j, "<=", 2) || !strncmp(input + j, "==", 2))
            {

                if (!strncmp(input + j, ">=", 2))
                {
                    j += 2;
                    boolOperation = CALSTACK_GREATER_EQUAL;
                }
                else if (!strncmp(input + j, "<=", 2))
                {
                    j += 2;
                    boolOperation = CALSTACK_LESS_EQUAL;
                }
                else if (!strncmp(input + j, "==", 2))
                {
                    j += 2;
                    boolOperation = CALSTACK_EQUAL;
                }
                else if (input[j] == '>')
                {
                    ++j;
                    boolOperation = CALSTACK_GREATER;
                }
                else if (input[j] == '<')
                {
                    ++j;
                    boolOperation = CALSTACK_LESS;
                }
                else
                {
                    printf("Error: illegal bool operation");
                }
                break;
            }
            exp1[i++] = input[j++];
        }
    }
    exp1[i] = '\0';

    i = 0;
    while (1) //now endFlag still is 1
    {
        while (input[j] == ' ')
        {
            ++j;
        }
        if (strncmp(input + j, "if", 2) == 0)
        {
            ++endFlag;
        }
        else if (strncmp(input + j, "then", 4) == 0)
        {
            --endFlag;
        }

        if (endFlag == 0)
        {
            j += 4;
            break;
        }
        exp2[i++] = input[j++];
    }
    exp2[i] = '\0';

    endFlag = 1;
    i = 0;
    while (1) //now endFlag still is 1
    {
        while (input[j] == ' ')
        {
            ++j;
        }
        if (strncmp(input + j, "if", 2) == 0)
        {
            ++endFlag;
        }
        else if (strncmp(input + j, "else", 4) == 0)
        {
            --endFlag;
        }

        if (endFlag == 0)
        {
            j += 4;
            break;
        }
        exp3[i++] = input[j++];
    }
    exp3[i] = '\0';
    i = 0;
    while (1)
    {
        if (input[j] == '\n' || input[j] == '\0' || input[j] == ';' || input[j] == EOF)
        {
            break;
        }

        exp4[i++] = input[j++];
    }
    exp4[i] = '\0';

    value1 = interpret(exp1, env);
    value2 = interpret(exp2, env);

    switch (boolOperation)
    {
    case CALSTACK_EQUAL:
        if (Numeric_eq(value1, value2))
        {
            boolOperation = CALSTACK_TURE;
        }
        else
        {
            boolOperation = CALSTACK_FALSE;
        }

        break;
    case CALSTACK_GREATER:
        if (Numeric_greater(value1, value2))
        {
            boolOperation = CALSTACK_TURE;
        }
        else
        {
            boolOperation = CALSTACK_FALSE;
        }
        break;
    case CALSTACK_LESS:
        if (Numeric_less(value1, value2))
        {
            boolOperation = CALSTACK_TURE;
        }
        else
        {
            boolOperation = CALSTACK_FALSE;
        }
        break;
    case CALSTACK_LESS_EQUAL:
        if (Numeric_less_eq(value1, value2))
        {
            boolOperation = CALSTACK_TURE;
        }
        else
        {
            boolOperation = CALSTACK_FALSE;
        }
        break;
    case CALSTACK_GREATER_EQUAL:
        if (Numeric_greater_eq(value1, value2))
        {
            boolOperation = CALSTACK_TURE;
        }
        else
        {
            boolOperation = CALSTACK_FALSE;
        }
        break;
    default:
        printf("Error: in boolOperation Swtich");
        break;
    }

    if (boolOperation == CALSTACK_TURE)
    {
        return interpret(exp3, env);
    }
    else
    {
        return interpret(exp4, env);
    }
}

Numeric *handleNum(char *input)
{
    int i = 0, j = 0;
    char buf[BUFFER_LENTH];
    Numeric *result;
    result = malloc(sizeof(Numeric));
    Numeric_new(result);
    while (input[j] == ' ')
    {
        ++j;
    }
    if (input[j] == '-')
    {
        buf[i++] = input[j++];
    }
    while (isdigit(input[j]) || input[j] == '.')
    {
        buf[i++] = input[j++];
    }
    buf[i] = '\0';
    strtoNumeric(buf, result);
    return result;
}

Closure *handleClosure(char *input, Environment *env)
{
    int i = 0, j = 1; // input[0] is $
    int endFlag = 1;
    char name[NAME_LENTH] = "";
    char exp1[BUFFER_LENTH] = "";
    while (input[j] != '{')
    {
        name[i++] = input[j++];
    }
    name[i] = '\0';
    i = 0;
    ++j;
    while (1)
    {
        if (input[j] == '{')
        {
            ++endFlag;
        }
        else if (input[j] == '}')
        {
            --endFlag;
        }
        if (endFlag == 0)
        {
            break;
        }

        exp1[i++] = input[j++];
    }
    exp1[i] = '\0';

    return new_closure(name, exp1, env);
}

void *handleAlphaExp(char *input, Environment *env)
{
    char name[NAME_LENTH];
    int i = 0, j = 0;
    void *ptr = NULL;

    while (input[j] == ' ')
    {
        ++j;
    }

    while (isalpha(input[j]))
    {
        name[i++] = input[j++];
    }
    name[i] = '\0';

    ptr = lookup_env(name, env);
    if (ptr == NULL)
    {
        printf("Error: not found var in Environment");
    }

    return ptr;
}

void *handleCall(char *input, Environment *env)
{
    int i = 0, j = 0, m = 0, n = 0;
    int endFlag = 1;
    int leftBraket = 0;
    int rightBraket = 0;
    char exp1[BUFFER_LENTH] = "";
    char exp2[BUFFER_LENTH] = "";
    Environment lenv;
    memcpy(&lenv, env, sizeof(Environment));
    Closure *closure = NULL;
    void *localEnv = NULL;
    while (input[j] == ' ')
    {
        ++j;
    }

    while (input[j] != '[')
    {
        exp1[i++] = input[j++];
    }
    // \0 will be add in exp1 in the end of this function
    leftBraket = j; //input[lastBraket] = '['
    n = j + 1;      //input[j] == after '['
    endFlag = 1;

    while (1)
    {
        if (input[n] == '[')
        {
            if (input[n - 1] == ']')
            {
                leftBraket = n;
            }

            ++endFlag;
        }
        else if (input[n] == ']')
        {
            rightBraket = n;
            --endFlag;
        }

        if (endFlag == 0)
        {
            if (input[n + 1] == '[')
            {
                leftBraket = n + 1;
                ++n;
                continue;
            }
            else
            {
                rightBraket = n;
                break;
            }
        }
        ++n;
    }

    while (j < leftBraket)
    {
        exp1[i++] = input[j++];
    }
    exp1[i] = '\0';

    for (int n = leftBraket + 1; n < rightBraket;)
    {
        exp2[m++] = input[n++];
    }

    exp2[m] = '\0';

    closure = interpret(exp1, env);
    localEnv = ext_env(closure->varName, interpret(exp2, env), &closure->lenv);
    return interpret(closure->exp, localEnv);
}
