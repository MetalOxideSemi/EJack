#ifndef CALSTACK_OPERATION_H_
#define CALSTACK_OPERATION_H_

#include <string.h>

typedef enum
{

    CALSTACK_FALSE, // FALSE 0
    CALSTACK_TURE,  // TRUE 1

    CALSTACK_GREATER,       // >
    CALSTACK_LESS,          // <
    CALSTACK_EQUAL,         // == (equal)
    CALSTACK_GREATER_EQUAL, // >=
    CALSTACK_LESS_EQUAL,    // <=

    CALSTACK_NEGATIVE, // -1 *

    CALSTACK_ADD,    // +
    CALSTACK_SUB,    // -
                     //3
    CALSTACK_MULT,   // *
    CALSTACK_DIV,    // /
    CALSTACK_POW,    // ^
                     //4
    CALSTACK_SIN,    // sin(
    CALSTACK_COS,    // cos(
    CALSTACK_TAN,    // tan(
    CALSTACK_SEC,    // sec(
    CALSTACK_CSC,    // csc(
    CALSTACK_COT,    // cot(
    CALSTACK_ARCSIN, // arcsin(
    CALSTACK_ARCCOS, // arccos(
    CALSTACK_ARCTAN, // arctan(
    CALSTACK_ARCSEC, // arcsec(
    CALSTACK_ARCCSC, // arccsc(
    CALSTACK_ARCCOT, // arccot(
    // CALSTACK_SINH,          // sinh(
    // CALSTACK_COSH,          // cosh(
    // CALSTACK_TANH,          // tanh(
    // CALSTACK_SECH,          // sech(
    // CALSTACK_CSCH,          // csch(
    // CALSTACK_COTH,          // coth(
    // CALSTACK_ARCSINH,       // arcsinh(
    // CALSTACK_ARCCOSH,       // arccosh(
    // CALSTACK_ARCTANH,       // arctanh(
    // CALSTACK_ARCSECH,       // arcsech(
    // CALSTACK_ARCCSCH,       // arccsch(
    // CALSTACK_ARCCOTH,       // arccoth(
    CALSTACK_LOG,  // log(
    CALSTACK_LOG2, // log2(
    CALSTACK_LG,   // lg(
    CALSTACK_LN,   // ln(
    CALSTACK_SQRT, // sqrt(
    CALSTACK_EXP,  // exp(
    CALSTACK_ABS,  // abs(
    CALSTACK_FACT, // fact( i.e. x!

    CALSTACK_LBRACKET, // (

    CALSTACK_RBRACKET, // )
} Operation;

#endif //CALSTACK_OPERATION_H_
