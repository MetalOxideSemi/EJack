#include "interpretOther.h"

Numeric *handleArithmeticExp(char *input, Environment *env, Numeric *num);

void calculate(Stack *numericStack, SStack *symbolStack);

void pushOperation(Stack *numericStack, SStack *symbolStack);

int parser(char *input, int now, Environment *env, Stack *numericStack, SStack *symbolStack);

static Operation operation;

//extern Numeric result;

void *interpret(char *input, Environment *env)
{
#ifdef DEBUG
    static int ct = 0;
    printf("%dth call interpret(%s)\n", ++ct, input);
#endif
    Numeric num;
    Numeric_new(&num);
    if (isLetrec(input))
    {
        return handleLetrec(input, env);
    }
    else if (isLet(input)) //let                     //void*
    {
        return handleLet(input, env);
    }
    else if (isIf(input))
    {
        return handleIf(input, env);
    }
    else if (isNum(input)) //number             //void*
    {
        return handleNum(input);
    }
    else if (isClosure(input)) // Closure
    {
        return handleClosure(input, env); //void*
    }
    else if (isAlphaExp(input)) // handle alphaExp (only alpha)
    {
        return handleAlphaExp(input, env); //void*
    }
    else if (isCall(input)) //Call function
    {
        return handleCall(input, env); //void*
    }
    else // Arithmetic expression
    {
        Numeric *result = malloc(sizeof(Numeric));
        Numeric_new(result);
        Numeric_assign(result, handleArithmeticExp(input, env, &num));
        //        return handleArithmeticExp(input, env); //numeric*
        return result;
    }
}

Numeric *handleArithmeticExp(char *input, Environment *env, Numeric *num)
{
    Stack numericStack;
    SStack symbolStack;

    Stack_new(&numericStack);
    SStack_new(&symbolStack);

    int now = 0;
    int parserValue = 0;
    while (1)
    {
        parserValue = parser(input + now, now, env, &numericStack, &symbolStack);
        if (parserValue == -1)
        {
            break;
        }
        now += parserValue;
    }
    while (SStack_empty(&symbolStack) != 1)
    {
        calculate(&numericStack, &symbolStack);
    }

    Stack_pop(&numericStack, num);
    Stack_free(&numericStack);
    SStack_free(&symbolStack);
    return num;
}

int parser(char *input, int now, Environment *env, Stack *numericStack, SStack *symbolStack)
{
    int i = 0, j = 0;
    Numeric num;
    char buf[BUFFER_LENTH] = "";
    Numeric_new(&num);
    while (input[j] == ' ')
    {
        ++j;
    }

    if (input[j] == '-')
    {
        if (now == 0 || input[j - 1] == '(')
        {

            operation = CALSTACK_NEGATIVE;
            ++j;
        }
        else
        {
            operation = CALSTACK_SUB;
            ++j;
        }
        pushOperation(numericStack, symbolStack);
    }
    else if (isdigit(input[j]))
    {
        while (isdigit(input[j]) || input[j] == '.')
        {
            buf[i++] = input[j++];
        }
        buf[i] = '\0';
        strtoNumeric(buf, &num);
        Stack_push(numericStack, &num);
    }
    else if (input[j] == '(' || input[j] == ')' || input[j] == '+' || input[j] == '*' || input[j] == '/' ||
             input[j] == '^' || input[j] == '$')
    {
        switch (input[j])
        {
        case '(':
            operation = CALSTACK_LBRACKET;
            ++j;
            pushOperation(numericStack, symbolStack);
            break;
        case ')':
            operation = CALSTACK_RBRACKET;
            ++j;
            pushOperation(numericStack, symbolStack);
            break;
        case '+':
            operation = CALSTACK_ADD;
            ++j;
            pushOperation(numericStack, symbolStack);
            break;
        case '*':
            operation = CALSTACK_MULT;
            ++j;
            pushOperation(numericStack, symbolStack);
            break;
        case '/':
            operation = CALSTACK_DIV;
            ++j;
            pushOperation(numericStack, symbolStack);
            break;
        case '^':
            operation = CALSTACK_POW;
            pushOperation(numericStack, symbolStack);
            ++j;
            break;
        case '$': //call lambda function
            while (1)
            {
                buf[i++] = input[j++];
                if (input[j] == '{')
                {
                    int endFlag = 1;
                    while (1)
                    {
                        buf[i++] = input[j++];
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
                            buf[i++] = input[j++];
                            if (input[j] == '[')
                            {
                                endFlag = 1;
                                while (1)
                                {
                                    buf[i++] = input[j++];
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
                                        buf[i++] = input[j++];
                                        break;
                                    }
                                }
                            }
                            break;
                        }
                    }
                    break;
                }
            }
            buf[i] = '\0';
            Stack_push(numericStack, interpret(buf, env));
            break;
        default:
            printf("Error: not found in '+ - * / ^ $ ( )'");
        }
    }
    else if (isalpha(input[j]))
    {
        if (strlen(input + j) >= 3 && !strncmp(input + j, "ln(", 3))
        {
            operation = CALSTACK_LN;
            pushOperation(numericStack, symbolStack);
            j += 3;
        }
        else if (strlen(input + j) >= 3 && !strncmp(input + j, "lg(", 3))
        {
            operation = CALSTACK_LG;
            pushOperation(numericStack, symbolStack);
            j += 3;
        }
        else if (strlen(input + j) >= 4 && !strncmp(input + j, "sin(", 4))
        {
            operation = CALSTACK_SIN;
            pushOperation(numericStack, symbolStack);
            j += 4;
        }
        else if (strlen(input + j) >= 4 && !strncmp(input + j, "cos(", 4))
        {
            operation = CALSTACK_COS;
            pushOperation(numericStack, symbolStack);
            j += 4;
        }
        else if (strlen(input + j) >= 4 && !strncmp(input + j, "tan(", 4))
        {
            operation = CALSTACK_TAN;
            pushOperation(numericStack, symbolStack);
            j += 4;
        }
        else if (strlen(input + j) >= 4 && !strncmp(input + j, "sec(", 4))
        {
            operation = CALSTACK_SEC;
            pushOperation(numericStack, symbolStack);
            j += 4;
        }
        else if (strlen(input + j) >= 4 && !strncmp(input + j, "csc(", 4))
        {
            operation = CALSTACK_CSC;
            pushOperation(numericStack, symbolStack);
            j += 4;
        }
        else if (strlen(input + j) >= 4 && !strncmp(input + j, "cot(", 4))
        {
            operation = CALSTACK_COT;
            pushOperation(numericStack, symbolStack);
            j += 4;
        }
        else if (strlen(input + j) >= 4 && !strncmp(input + j, "log(", 4))
        {
            operation = CALSTACK_LOG;
            pushOperation(numericStack, symbolStack);
            j += 4;
        }
        else if (strlen(input + j) >= 4 && !strncmp(input + j, "exp(", 4))
        {
            operation = CALSTACK_EXP;
            pushOperation(numericStack, symbolStack);
            j += 4;
        }
        else if (strlen(input + j) >= 4 && !strncmp(input + j, "abs(", 4))
        {
            operation = CALSTACK_ABS;
            pushOperation(numericStack, symbolStack);
            j += 4;
        }
        else if (strlen(input + j) >= 5 && !strncmp(input + j, "log2(", 5))
        {
            operation = CALSTACK_LOG2;
            pushOperation(numericStack, symbolStack);
            j += 5;
        }
        else if (strlen(input + j) >= 5 && !strncmp(input + j, "sqrt(", 5))
        {
            operation = CALSTACK_SQRT;
            pushOperation(numericStack, symbolStack);
            j += 5;
        }
        else if (strlen(input + j) >= 5 && !strncmp(input + j, "fact(", 5))
        {
            operation = CALSTACK_FACT;
            pushOperation(numericStack, symbolStack);
            j += 5;
        }
        else if (strlen(input + j) >= 7 && !strncmp(input + j, "arcsin(", 7))
        {
            operation = CALSTACK_ARCSIN;
            pushOperation(numericStack, symbolStack);
            j += 7;
        }
        else if (strlen(input + j) >= 7 && !strncmp(input + j, "arccos(", 7))
        {
            operation = CALSTACK_ARCCOS;
            pushOperation(numericStack, symbolStack);
            j += 7;
        }
        else if (strlen(input + j) >= 7 && !strncmp(input + j, "arctan(", 7))
        {
            operation = CALSTACK_ARCTAN;
            pushOperation(numericStack, symbolStack);
            j += 7;
        }
        else if (strlen(input + j) >= 7 && !strncmp(input + j, "arcsec(", 7))
        {
            operation = CALSTACK_ARCSEC;
            pushOperation(numericStack, symbolStack);
            j += 7;
        }
        else if (strlen(input + j) >= 7 && !strncmp(input + j, "arccsc(", 7))
        {
            operation = CALSTACK_ARCCSC;
            pushOperation(numericStack, symbolStack);
            j += 7;
        }
        else if (strlen(input + j) >= 7 && !strncmp(input + j, "arccot(", 7))
        {
            operation = CALSTACK_ARCCOT;
            pushOperation(numericStack, symbolStack);
            j += 7;
        }
        //        else if (strlen(input + j) >= 8 && !strncmp(input + j, "integral", 8))
        //        {
        //            j += 8;
        //            char name[NAME_LENTH] = "";
        //            char num1[BUFFER_LENTH] = "";
        //            char num2[BUFFER_LENTH] = "";
        //            Numeric left, right;
        //            Numeric_new(&left);
        //            Numeric_new(&right);
        //            Closure *function;
        //            while (input[j] == ' ')
        //            {
        //                ++j;
        //            }
        //            while (isalpha(input[j]))
        //            {
        //                name[i++] = input[j++];
        //            }
        //            name[i] = '\0';
        //            i = 0;
        //            while (input[j] == ' ')
        //            {
        //                ++j;
        //            }
        //            while (isdigit(input[j]) || input[j] == '.')
        //            {
        //                num1[i++] = input[j++];
        //            }
        //            num1[i] = '\0';
        //            i = 0;
        //            while (input[j] == ' ')
        //            {
        //                ++j;
        //            }
        //            while (isdigit(input[j]) || input[j] == '.')
        //            {
        //                num2[i++] = input[j++];
        //            }
        //            strtoNumeric(num1, &left);
        //            strtoNumeric(num2, &right);
        //            function = interpret(name, env);
        //
        //            integral(function, &left, &right);
        //        }

        //handle diff f value
        else if (strlen(input + j) >= 4 && !strncmp(input + j, "diff", 4))
        {
            prec_opt = false;
            char name[NAME_LENTH] = "";
            Closure *f = NULL;
            const Numeric *dx = Numeric_calculus();
            Numeric *num = NULL;
            Numeric value1, value2;
            Numeric_new(&value1);
            Numeric_new(&value2);
            void *localEnv = NULL;
            j += 4;
            while (input[j] == ' ')
            {
                ++j;
            }
            while (isalpha(input[j]))
            {
                name[i++] = input[j++];
            }
            name[i] = '\0';
            while (input[j] == ' ')
            {
                ++j;
            }
            i = 0;
            if (input[j] == '-' || input[j] == '+')
            {
                buf[i++] = input[j++];
            }
            while ( input[j] == '.'||isalnum(input[j]))
            {
                buf[i++] = input[j++];
            }
            buf[i] = '\0';

            num = interpret(buf, env);
            f = interpret(name, env);

            localEnv = ext_env(f->varName, Numeric_add(num, dx), &f->lenv);
            Numeric_assign(&value1, interpret(f->exp, localEnv));
            localEnv = ext_env(f->varName, Numeric_sub(num, dx), &f->lenv);
            Numeric_assign(&value2, interpret(f->exp, localEnv));
            //function diff(Clouse* f,Numeric* num,Numeric* dx)
            Numeric_assign(num, Numeric_div(Numeric_sub(&value1, &value2), Numeric_add(dx, dx)));
            //function over
            Stack_push(numericStack, num);

            Numeric_free(&value1);
            Numeric_free(&value2);

            prec_opt = true;
        }
        else //variable or function
        {
            while (1)
            {
                buf[i++] = input[j++];
                if (!isalpha(input[j]))
                {
                    if (input[j] == '[')
                    {
                        int endFlag = 1;
                        while (1)
                        {
                            buf[i++] = input[j++];
                            if (input[j] == '[')
                            {
                                ++endFlag;
                            }
                            else if (input[j] == ']')
                            {
                                --endFlag;
                            }
                            if (endFlag == 0 && input[j + 1] != '[')
                            {
                                buf[i++] = input[j++];
                                break;
                            }
                        }
                    }

                    break;
                }
            }
            buf[i] = '\0';
            Stack_push(numericStack, interpret(buf, env));
        }
    }

    if (input[j] == '\0' || input[j] == ';' || input[j] == '\n')
    {
        Numeric_free(&num);
        return -1;
    }
    else
    {
        Numeric_free(&num);
        return j;
    }
}

void pushOperation(Stack *numericStack, SStack *symbolStack)
{
    if (operation == CALSTACK_NEGATIVE)
    {
        Stack_push(numericStack,Numeric_negative_one());
        operation = CALSTACK_MULT;
        pushOperation(numericStack, symbolStack);
    }
    else if (SStack_empty(symbolStack) == 1)
    {
        SStack_push(symbolStack, operation);
    }
    else if (operation == CALSTACK_LBRACKET)
    {
        SStack_push(symbolStack, operation);
    }
    else if (operation == CALSTACK_RBRACKET)
    {
        while (SStack_top(symbolStack) < CALSTACK_SIN)
        {
            calculate(numericStack, symbolStack);
        }
        calculate(numericStack, symbolStack);
    }
    else if (operation >= CALSTACK_SIN && operation <= CALSTACK_FACT)
    {
        SStack_push(symbolStack, operation);
    }
    else if (operation >= CALSTACK_MULT && operation <= CALSTACK_POW)
    {
        if (SStack_top(symbolStack) < CALSTACK_MULT ||
            (SStack_top(symbolStack) >= CALSTACK_SIN && SStack_top(symbolStack) <= CALSTACK_LBRACKET))
        {
            SStack_push(symbolStack, operation);
        }
        else
        {
            while (SStack_top(symbolStack) >= CALSTACK_MULT && SStack_top(symbolStack) <= CALSTACK_POW)
            {
                calculate(numericStack, symbolStack);
            }
            SStack_push(symbolStack, operation);
        }
    }
    else if (operation >= CALSTACK_ADD && operation <= CALSTACK_SUB)
    {

        while (SStack_top(symbolStack) >= CALSTACK_ADD && SStack_top(symbolStack) <= CALSTACK_POW)
        {
            calculate(numericStack, symbolStack);
        }
        SStack_push(symbolStack, operation);
    }
}

void calculate(Stack *numericStack, SStack *symbolStack)
{
    Numeric a, b, n;
    Numeric_new(&a);
    Numeric_new(&b);
    Numeric_new(&n);
    switch (SStack_pop(symbolStack))
    {
    case CALSTACK_LBRACKET:
        break;
    case CALSTACK_FACT:
        Stack_pop(numericStack, &n);
        Stack_push(numericStack, Numeric_fact(&n));
        break;
    case CALSTACK_ABS:
        Stack_pop(numericStack, &n);
        Stack_push(numericStack, Numeric_abs(&n));
        break;
    case CALSTACK_EXP:
        Stack_pop(numericStack, &n);
        Stack_push(numericStack, Numeric_exp(&n));
        break;
    case CALSTACK_SQRT:
        Stack_pop(numericStack, &n);
        Stack_push(numericStack, Numeric_sqrt(&n));
        break;
    case CALSTACK_LN:
        Stack_pop(numericStack, &n);
        Stack_push(numericStack, Numeric_ln(&n));
        break;
    case CALSTACK_LG:
        Stack_pop(numericStack, &n);
        Stack_push(numericStack, Numeric_lg(&n));
        break;
    case CALSTACK_LOG2:
        Stack_pop(numericStack, &n);
        Stack_push(numericStack, Numeric_log2(&n));
        break;
    case CALSTACK_LOG:
        Stack_pop(numericStack, &a);
        Stack_pop(numericStack, &b);
        Stack_push(numericStack, Numeric_log(&b, &a));
        break;
    case CALSTACK_ARCCOT:
        Stack_pop(numericStack, &n);
        Stack_push(numericStack, Numeric_arccot(&n));
        break;
    case CALSTACK_ARCCSC:
        Stack_pop(numericStack, &n);
        Stack_push(numericStack, Numeric_arccot(&n));
        break;
    case CALSTACK_ARCSEC:
        Stack_pop(numericStack, &n);
        Stack_push(numericStack, Numeric_arcsec(&n));
        break;
    case CALSTACK_ARCTAN:
        Stack_pop(numericStack, &n);
        Stack_push(numericStack, Numeric_arctan(&n));
        break;
    case CALSTACK_ARCCOS:
        Stack_pop(numericStack, &n);
        Stack_push(numericStack, Numeric_arccos(&n));
        break;
    case CALSTACK_ARCSIN:
        Stack_pop(numericStack, &n);
        Stack_push(numericStack, Numeric_arcsin(&n));
        break;
    case CALSTACK_COT:
        Stack_pop(numericStack, &n);
        Stack_push(numericStack, Numeric_cot(&n));
        break;
    case CALSTACK_CSC:
        Stack_pop(numericStack, &n);
        Stack_push(numericStack, Numeric_csc(&n));
        break;
    case CALSTACK_SEC:
        Stack_pop(numericStack, &n);
        Stack_push(numericStack, Numeric_sec(&n));
        break;
    case CALSTACK_TAN:
        Stack_pop(numericStack, &n);
        Stack_push(numericStack, Numeric_tan(&n));
        break;
    case CALSTACK_COS:
        Stack_pop(numericStack, &n);
        Stack_push(numericStack, Numeric_cos(&n));
        break;
    case CALSTACK_SIN:
        Stack_pop(numericStack, &n);
        Stack_push(numericStack, Numeric_sin(&n));
        break;
    case CALSTACK_POW:
        Stack_pop(numericStack, &a);
        Stack_pop(numericStack, &b);
        Stack_push(numericStack, Numeric_pow(&b, &a));
        break;
    case CALSTACK_DIV:
        Stack_pop(numericStack, &a);
        Stack_pop(numericStack, &b);
        Stack_push(numericStack, Numeric_div(&b, &a));
        break;
    case CALSTACK_MULT:
        Stack_pop(numericStack, &a);
        Stack_pop(numericStack, &b);
        Stack_push(numericStack, Numeric_mult(&b, &a));
        break;
    case CALSTACK_SUB:
        Stack_pop(numericStack, &a);
        Stack_pop(numericStack, &b);
        Stack_push(numericStack, Numeric_sub(&b, &a));
        break;
    case CALSTACK_ADD:
        Stack_pop(numericStack, &a);
        Stack_pop(numericStack, &b);
        Stack_push(numericStack, Numeric_add(&b, &a));
        break;

    default:
        printf("Error: not found operation");
    }
    Numeric_free(&a);
    Numeric_free(&b);
    Numeric_free(&n);
}