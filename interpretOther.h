#ifndef _INTERPRETOTHER_H_
#define _INTERPRETOTHER_H_
#include "interpret.h"
#include "calstack.h"
#include "closure.h"
#include "environment.h"
#include "numeric.h"
#include "operation.h"
#include "scientific.h"
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_LENTH 32768

bool isLetrec(char *input);
bool isLet(char *input);
bool isNum(char *input);
bool isAlphaExp(char *input);
bool isClosure(char *input);
bool isCall(char *input);
bool isIf(char *input);

Numeric *handleNum(char *input);
Closure *handleClosure(char *input, Environment *env);
void *handleAlphaExp(char *input, Environment *env);
void *handleLetrec(char *input, Environment *env);
void *handleLet(char *input, Environment *env);
void *handleCall(char *input, Environment *env);
void *handleIf(char *input, Environment *env);

#endif