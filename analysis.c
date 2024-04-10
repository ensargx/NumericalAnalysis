/*
 * analysis.c
 * Author: Ensar Gök
 *
 * Numerical analysis functions
 */

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define EVALABLE void*

#ifndef MAX_FUNC_ARGS
#define MAX_FUNC_ARGS 16
#endif

#define double long double
#define ABS(x) ((x) < 0 ? -(x) : (x))

double evaluate(EVALABLE *e, double value);
#define EVALTYPE(x) ((EvalAble *)x)->type
#define UNUSED(x) (void)(x)

/* Struct for status codes */ 
typedef struct _StatusCode {
    char code;
    char expected;
    char *pos;
} StatusCode;

/* Enumerations */

typedef enum _EvalAbleType {
    CONSTANT,
    VARIABLE,
    SUM_CHAIN,
    MUL_CHAIN,
    LOGARITHM,
    EXPONENTIAL,
    TRIGONOMETRIC,
    INVERSE_TRIGONOMETRIC,
} EvalAbleType;

/* Struct Prototypes */
typedef struct _Constant Constant;
typedef struct _Variable Variable;
typedef struct _SumChain SumChain;
typedef struct _MulChain MulChain;
typedef struct _Exponential Exponential;
typedef struct _Logarithm Logarithm;
typedef struct _Trigonometric Trigonometric;
typedef enum _TrigonometricType TrigonometricType;
typedef struct _InverseTrigonometric InverseTrigonometric;
typedef enum _InverseTrigonometricType InverseTrigonometricType;

typedef struct _EvalAble {
    EvalAbleType type;
} EvalAble;

typedef struct _Constant {
    EvalAbleType type;
    double value;
} Constant;

typedef struct _Variable {
    EvalAbleType type;
} Variable;

typedef struct _SumChain {
    EvalAbleType type;
    int argCount;
    EVALABLE *args[MAX_FUNC_ARGS];
    int isPositive[MAX_FUNC_ARGS];
} SumChain;

typedef struct _MulChain {
    EvalAbleType type;
    int argCount;
    EVALABLE *args[MAX_FUNC_ARGS];
    int isDivided[MAX_FUNC_ARGS];
} MulChain;

typedef struct _Exponential {
    EvalAbleType type;
    EVALABLE *base;
    EVALABLE *exponent;
} Exponential;

typedef enum _TrigonometricType {
    SIN,
    COS,
    TAN,
    CSC,
    SEC,
    COT,
} TrigonometricType;

typedef struct _Trigonometric {
    EvalAbleType type;
    TrigonometricType trigType;
    EVALABLE *arg;
} Trigonometric;

typedef enum _InverseTrigonometricType {
    ASIN,
    ACOS,
    ATAN,
    ACSC,
    ASEC,
    ACOT,
} InverseTrigonometricType;

typedef struct _InverseTrigonometric {
    EvalAbleType type;
    InverseTrigonometricType trigType;
    EVALABLE *arg;
} InverseTrigonometric;

typedef struct _Logarithm {
    EvalAbleType type;
    EVALABLE *base;
    EVALABLE *value;
} Logarithm;

/* SumChain prototypes */
Variable *createVariable();
void destroyVariable(Variable *v);
double evaluateVariable(Variable *v, double value);
void printVariable(Variable *v);

Constant *createConstant(double value);
void destroyConstant(Constant *c);
double evaluateConstant(Constant *c, double value);
void printConstant(Constant *c);

Exponential *createExponential(EVALABLE *base, EVALABLE *exponent);
void destroyExponential(Exponential *e);
double evaluateExponential(Exponential *e, double value);
void printExponential(Exponential *e);

Logarithm *createLogarithm(EVALABLE *base, EVALABLE *value);
void destroyLogarithm(Logarithm *l);
double evaluateLogarithm(Logarithm *l, double value);
void printLogarithm(Logarithm *l);

SumChain *createSumChain();
void addSumChainArg(SumChain *f, EVALABLE *argType, int sign);
void destroySumChain(SumChain *f);
double evaluateSumChain(SumChain *f, double value);
void printSumChain(SumChain *f);

MulChain *createMulChain();
void addMulChainArg(MulChain *m, EVALABLE *arg, int isDivided);
void destroyMulChain(MulChain *m);
double evaluateMulChain(MulChain *m, double value);
void printMulChain(MulChain *m);

Trigonometric *createTrigonometric(TrigonometricType type, EVALABLE *arg);
void destroyTrigonometric(Trigonometric *t);
double evaluateTrigonometric(Trigonometric *t, double value);
void printTrigonometric(Trigonometric *t);

InverseTrigonometric *createInverseTrigonometric(InverseTrigonometricType type, EVALABLE *arg);
void destroyInverseTrigonometric(InverseTrigonometric *it);
double evaluateInverseTrigonometric(InverseTrigonometric *it, double value);
void printInverseTrigonometric(InverseTrigonometric *it);

double evaluate(EVALABLE *e, double value);
void destroy(EVALABLE *e);
void print(EVALABLE *e);

/* Function implementations */


// Variable functions
Variable *createVariable()
{
    Variable *v = (Variable *)malloc(sizeof(Variable));
    v->type = VARIABLE;
    return v;
}

void destroyVariable(Variable *v)
{
    free(v);
}

double evaluateVariable(Variable *v, double value)
{
    UNUSED(v);
    return value;
}

void printVariable(Variable *v)
{
    UNUSED(v);
    printf("x");
}

// MulChain functions
MulChain *createMulChain()
{
    MulChain *m = (MulChain *)malloc(sizeof(MulChain));
    m->type = MUL_CHAIN;
    m->argCount = 0;
    for (int i = 0; i < MAX_FUNC_ARGS; i++)
    {
        m->args[i] = NULL;
        m->isDivided[i] = 0;
    }
    return m;
}

void addMulChainArg(MulChain *m, EVALABLE *arg, int isDivided)
{
    m->args[m->argCount] = arg;
    m->isDivided[m->argCount] = isDivided;
    m->argCount++;
}

void destroyMulChain(MulChain *m)
{
    for (int i = 0; i < m->argCount && m->args[i] != NULL; i++)
    {
        destroy(m->args[i]);
    }
    free(m);
}

double evaluateMulChain(MulChain *m, double value)
{
    double result = 1;
    for (int i = 0; i < m->argCount && m->args[i] != NULL; i++)
    {
        if (m->isDivided[i] == 1)
        {
            result /= evaluate(m->args[i], value);
        } else {
            result *= evaluate(m->args[i], value);
        }
    }
    return result;
}

void printMulChain(MulChain *m)
{
    for (int i = 0; i < m->argCount && m->args[i] != NULL; i++)
    {
        print(m->args[i]);
        if (i < m->argCount - 1 && m->args[i + 1] != NULL)
        {
            if (m->isDivided[i + 1] == 1)
            {
                printf("/");
            } else {
                printf("*");
            }
        }
    }
}

// Constant functions
Constant *createConstant(double value)
{
    Constant *c = (Constant *)malloc(sizeof(Constant));
    c->type = CONSTANT;
    c->value = value;
    return c;
}

void destroyConstant(Constant *c)
{
    free(c);
}

double evaluateConstant(Constant *c, double value)
{
    UNUSED(value);
    return c->value;
}

void printConstant(Constant *c)
{
    printf("%Lf", c->value);
}

// Exponential functions
Exponential *createExponential(EVALABLE *base, EVALABLE *exponent)
{
    Exponential *e = (Exponential *)malloc(sizeof(Exponential));
    e->type = EXPONENTIAL;
    if (base == NULL)
    {
        base = (EVALABLE *)createConstant(1);
    }
    e->base = base;
    if (exponent == NULL)
    {
        exponent = (EVALABLE *)createConstant(1);
    }
    e->exponent = exponent;
    return e;
}

void destroyExponential(Exponential *e)
{
    destroy(e->base);
    destroy(e->exponent);
    free(e);
}

double evaluateExponential(Exponential *e, double value)
{
    return pow(evaluate(e->base, value), evaluate(e->exponent, value));
}

void printExponential(Exponential *e)
{
    print(e->base);
    printf("^(" );
    print(e->exponent);
    printf(")");
}

// Trigonometric functions

Trigonometric *createTrigonometric(TrigonometricType type, EVALABLE *arg)
{
    Trigonometric *t = (Trigonometric *)malloc(sizeof(Trigonometric));
    t->type = TRIGONOMETRIC;
    if (arg == NULL)
    {
        arg = (EVALABLE *)createConstant(1);
    }
    t->arg = arg;
    t->trigType = type;
    return t;
}

void destroyTrigonometric(Trigonometric *t)
{
    destroy(t->arg);
    free(t);
}

double evaluateTrigonometric(Trigonometric *t, double value)
{
    switch (t->trigType)
    {
        case SIN:
            return sin(evaluate(t->arg, value));
        case COS:
            return cos(evaluate(t->arg, value));
        case TAN:
            return tan(evaluate(t->arg, value));
        case CSC:
            return 1 / sin(evaluate(t->arg, value));
        case SEC:
            return 1 / cos(evaluate(t->arg, value));
        case COT:
            return 1 / tan(evaluate(t->arg, value));
        default:
            return 0;
    }
}

void printTrigonometric(Trigonometric *t)
{
    switch (t->trigType)
    {
        case SIN:
            printf("sin(");
            break;
        case COS:
            printf("cos(");
            break;
        case TAN:
            printf("tan(");
            break;
        case CSC:
            printf("csc(");
            break;
        case SEC:
            printf("sec(");
            break;
        case COT:
            printf("cot(");
            break;
    }
    print(t->arg);
    printf(")");
}

// Inverse Trigonometric functions

InverseTrigonometric *createInverseTrigonometric(InverseTrigonometricType type, EVALABLE *arg)
{
    InverseTrigonometric *it = (InverseTrigonometric *)malloc(sizeof(InverseTrigonometric));
    it->type = INVERSE_TRIGONOMETRIC;
    if (arg == NULL)
    {
        arg = (EVALABLE *)createConstant(1);
    }
    it->arg = arg;
    it->trigType = type;
    return it;
}

void destroyInverseTrigonometric(InverseTrigonometric *it)
{
    destroy(it->arg);
    free(it);
}

double evaluateInverseTrigonometric(InverseTrigonometric *it, double value)
{
    switch (it->trigType)
    {
        case ASIN:
            return asin(evaluate(it->arg, value));
        case ACOS:
            return acos(evaluate(it->arg, value));
        case ATAN:
            return atan(evaluate(it->arg, value));
        case ACSC:
            return asin(1 / evaluate(it->arg, value));
        case ASEC:
            return acos(1 / evaluate(it->arg, value));
        case ACOT:
            return atan(1 / evaluate(it->arg, value));
        default:
            return 0;
    }
}

void printInverseTrigonometric(InverseTrigonometric *it)
{
    switch (it->trigType)
    {
        case ASIN:
            printf("asin(");
            break;
        case ACOS:
            printf("acos(");
            break;
        case ATAN:
            printf("atan(");
            break;
        case ACSC:
            printf("acsc(");
            break;
        case ASEC:
            printf("asec(");
            break;
        case ACOT:
            printf("acot(");
            break;
    }
    print(it->arg);
    printf(")");
}

// Logarithm functions

Logarithm *createLogarithm(EVALABLE *base, EVALABLE *value)
{
    Logarithm *l = (Logarithm *)malloc(sizeof(Logarithm));
    l->type = LOGARITHM;
    if (base == NULL)
    {
        base = (EVALABLE *)createConstant(10);
    }
    l->base = base;
    if (value == NULL)
    {
        value = (EVALABLE *)createConstant(1);
    }
    l->value = value;
    return l;
}

void destroyLogarithm(Logarithm *l)
{
    destroy(l->base);
    destroy(l->value);
    free(l);
}

double evaluateLogarithm(Logarithm *l, double value)
{
    // printf("[DEBUG]: evaluateLogarithm: Check implementation & test\n");
    // log_b(a) = log(a) / log(b) 
    return log(evaluate(l->value, value)) / log(evaluate(l->base, value));
}

void printLogarithm(Logarithm *l)
{
    printf("log_");
    print(l->base);
    printf("(");
    print(l->value);
    printf(")");
}

// SumChain functions

SumChain *createSumChain()
{
    SumChain *f = (SumChain *)malloc(sizeof(SumChain));
    f->type = SUM_CHAIN;
    f->argCount = 0;
    for (int i = 0; i < MAX_FUNC_ARGS; i++)
    {
        f->args[i] = NULL;
    }
    return f;
}

void addSumChainArg(SumChain *f, EVALABLE *arg, int isPositive)
{
    f->args[f->argCount] = arg;
    f->isPositive[f->argCount++] = isPositive;
}

void destroySumChain(SumChain *f)
{
    for (int i = 0; i < f->argCount && f->args[i] != NULL; i++)
    {
        destroy(f->args[i]);
    }
    free(f);
}

double evaluateSumChain(SumChain *f, double value)
{
    double result = 0;
    for (int i = 0; i < f->argCount && f->args[i] != NULL; i++)
    {
        if (f->isPositive[i] == 1)
        {
            result += evaluate((EVALABLE *)f->args[i], value);
        } else {
            result -= evaluate((EVALABLE *)f->args[i], value);
        }
    }
    return result;
}

void printSumChain(SumChain *f)
{
    if (f->argCount == 0)
    {
        return;
    }
    printf("(");
    if (f->isPositive[0] == 0)
    {
        printf("-");
    }
    print(f->args[0]);
    for (int i = 1; i < f->argCount && f->args[i] != NULL; i++)
    {
        if (f->isPositive[i] == 1)
        {
            printf("+");
        } else {
            printf("-");
        }
        print(f->args[i]);
    }
    printf(")");
}

EVALABLE *copyEvalable(EVALABLE *e)
{
    switch (EVALTYPE(e))
    {
        case CONSTANT:
            return (EVALABLE *)createConstant(((Constant *)e)->value);
        case VARIABLE:
            return (EVALABLE *)createVariable();
        case EXPONENTIAL:
            return (EVALABLE *)createExponential(
                copyEvalable(((Exponential *)e)->base),
                copyEvalable(((Exponential *)e)->exponent)
            );
        case TRIGONOMETRIC:
            return (EVALABLE *)createTrigonometric(
                ((Trigonometric *)e)->trigType,
                copyEvalable(((Trigonometric *)e)->arg)
            );
        case INVERSE_TRIGONOMETRIC:
            return (EVALABLE *)createInverseTrigonometric(
                ((InverseTrigonometric *)e)->trigType,
                copyEvalable(((InverseTrigonometric *)e)->arg)
            );
        case LOGARITHM:
            return (EVALABLE *)createLogarithm(
                copyEvalable(((Logarithm *)e)->base),
                copyEvalable(((Logarithm *)e)->value)
            );
        case SUM_CHAIN:
        {
            SumChain *f = createSumChain();
            for (int i = 0; i < ((SumChain *)e)->argCount; i++)
            {
                addSumChainArg(f, copyEvalable(((SumChain *)e)->args[i]), ((SumChain *)e)->isPositive[i]);
            }
            return (EVALABLE *)f;
        }
        case MUL_CHAIN:
        {
            MulChain *m = createMulChain();
            for (int i = 0; i < ((SumChain *)e)->argCount; i++)
            {
                addMulChainArg(m, copyEvalable(((SumChain *)e)->args[i]), ((SumChain *)e)->isPositive[i]);
            }
            return (EVALABLE *)m;
        }
        default:
            return NULL;
    }
}

// Main functions

void destroy(EVALABLE *e)
{
    switch (EVALTYPE(e))
    {
        case CONSTANT:
            destroyConstant((Constant *)e);
            break;
        case VARIABLE:
            destroyVariable((Variable *)e);
            break;
        case EXPONENTIAL:
            destroyExponential((Exponential *)e);
            break;
        case TRIGONOMETRIC:
            destroyTrigonometric((Trigonometric *)e);
            break;
        case INVERSE_TRIGONOMETRIC:
            destroyInverseTrigonometric((InverseTrigonometric *)e);
            break;
        case LOGARITHM:
            destroyLogarithm((Logarithm *)e);
            break;
        case SUM_CHAIN:
            destroySumChain((SumChain *)e);
            break;
        case MUL_CHAIN:
            destroyMulChain((MulChain *)e);
            break;
    }
}

double evaluate(EVALABLE *e, double value)
{
    switch (EVALTYPE(e))
    {
        case CONSTANT:
            return evaluateConstant((Constant *)e, value);
        case VARIABLE:
            return evaluateVariable((Variable *)e, value);
        case EXPONENTIAL:
            return evaluateExponential((Exponential *)e, value);
        case TRIGONOMETRIC:
            return evaluateTrigonometric((Trigonometric *)e, value);
        case INVERSE_TRIGONOMETRIC:
            return evaluateInverseTrigonometric((InverseTrigonometric *)e, value);
        case LOGARITHM:
            return evaluateLogarithm((Logarithm *)e, value);
        case SUM_CHAIN:
            return evaluateSumChain((SumChain *)e, value);
        case MUL_CHAIN:
            return evaluateMulChain((MulChain *)e, value);
        default:
            return 0;
    }
}

void print(EVALABLE *e)
{
    switch (EVALTYPE(e))
    {
        case CONSTANT:
            printConstant((Constant *)e);
            break;
        case VARIABLE:
            printVariable((Variable *)e);
            break;
        case EXPONENTIAL:
            printExponential((Exponential *)e);
            break;
        case TRIGONOMETRIC:
            printTrigonometric((Trigonometric *)e);
            break;
        case INVERSE_TRIGONOMETRIC:
            printInverseTrigonometric((InverseTrigonometric *)e);
            break;
        case LOGARITHM:
            printLogarithm((Logarithm *)e);
            break;
        case SUM_CHAIN:
            printSumChain((SumChain *)e);
            break;
        case MUL_CHAIN:
            printMulChain((MulChain *)e);
            break;
    }
}

void printType(EVALABLE *e)
{
    EvalAbleType type = EVALTYPE(e);
    switch (type)
    {
        case CONSTANT:
            printf("Constant\n");
            break;
        case VARIABLE:
            printf("Variable\n");
            break;
        case EXPONENTIAL:
            printf("Exponential\n");
            break;
        case TRIGONOMETRIC:
            printf("Trigonometric\n");
            break;
        case INVERSE_TRIGONOMETRIC:
            printf("Inverse Trigonometric\n");
            break;
        case LOGARITHM:
            printf("Logarithm\n");
            break;
        case SUM_CHAIN:
            printf("Sum Chain\n");
            break;
        case MUL_CHAIN:
            printf("Mul Chain\n");
            break;
    }
}

/* Optimize functions */ 
EVALABLE *optimize(EVALABLE *e);
EVALABLE *optimizeSumChain(SumChain *f);
EVALABLE *optimizeMulChain(MulChain *m);
EVALABLE *optimizeExponential(Exponential *e);
EVALABLE *optimizeTrigonometric(Trigonometric *t);
EVALABLE *optimizeInverseTrigonometric(InverseTrigonometric *it);
EVALABLE *optimizeLogarithm(Logarithm *l);

/* 
 * Optimize functions for each type of expression
 *
 * The optimization functions are responsible for simplifying the expression
 * by evaluating the constants and optimizing the expressions that can be
 * simplified.
 *
 * Also the optimization functions are responsible for freeing the memory and
 * creating a new expression that is optimized if necessary.
 *
 * Parameters:
 * - e: The expression to be optimized
 *   The expression can be any type of expression
 * Returns:
 * - The optimized expression
*/
EVALABLE *optimize(EVALABLE *e)
{
    switch (EVALTYPE(e))
    {
        case CONSTANT:
        case VARIABLE:
            return e;
        case EXPONENTIAL:
            return optimizeExponential((Exponential *)e);
        case TRIGONOMETRIC:
            return optimizeTrigonometric((Trigonometric *)e);
        case INVERSE_TRIGONOMETRIC:
            return optimizeInverseTrigonometric((InverseTrigonometric *)e);
        case LOGARITHM:
            return optimizeLogarithm((Logarithm *)e);
        case MUL_CHAIN:
            return optimizeMulChain((MulChain *)e);
        case SUM_CHAIN:
            return optimizeSumChain((SumChain *)e);
        default:
            return NULL;
    }
}

EVALABLE *optimizeSumChain(SumChain *f)
{
    for (int i = 0; i < f->argCount; i++)
    {
        f->args[i] = optimize(f->args[i]);
    }
    double constantSum = 0;
    SumChain *optimized = createSumChain();
    for (int i = 0; i < f->argCount; i++)
    {
        if (EVALTYPE(f->args[i]) == CONSTANT)
        {
            if (f->isPositive[i] == 1)
            {
                constantSum += ((Constant *)f->args[i])->value;
            } else
            {
                constantSum -= ((Constant *)f->args[i])->value;
            }
        } else
        {
            EVALABLE *arg = copyEvalable(f->args[i]);
            addSumChainArg(optimized, arg, f->isPositive[i]);
        }
    }
    if (optimized->argCount == 0)
    {
        destroySumChain(optimized);
        destroySumChain(f);
        return (EVALABLE *)createConstant(constantSum);
    }
    if (constantSum != 0)
    {
        addSumChainArg(optimized, (EVALABLE *)createConstant(constantSum), 1);
    }
    destroySumChain(f);
    return (EVALABLE *)optimized;
}

EVALABLE *optimizeMulChain(MulChain *m)
{
    for (int i = 0; i < m->argCount; i++)
    {
        m->args[i] = optimize(m->args[i]);
    }
    double constantMul = 1;
    MulChain *optimized = createMulChain();
    for (int i = 0; i < m->argCount; i++)
    {
        if (EVALTYPE(m->args[i]) == CONSTANT)
        {
            if (m->isDivided[i] == 1)
            {
                constantMul /= ((Constant *)m->args[i])->value;
            } else
            {
                constantMul *= ((Constant *)m->args[i])->value;
            }
        } else
        {
            EVALABLE *arg = copyEvalable(m->args[i]);
            addMulChainArg(optimized, arg, m->isDivided[i]);
        }
    }
    if (optimized->argCount == 0)
    {
        destroyMulChain(optimized);
        destroyMulChain(m);
        return (EVALABLE *)createConstant(constantMul);
    }
    if (constantMul != 1)
    {
        addMulChainArg(optimized, (EVALABLE *)createConstant(constantMul), 0);
    }
    destroyMulChain(m);
    return (EVALABLE *)optimized;
}

EVALABLE *optimizeExponential(Exponential *e)
{
    e->base = optimize(e->base);
    e->exponent = optimize(e->exponent);
    if (EVALTYPE(e->base) == CONSTANT && EVALTYPE(e->exponent) == CONSTANT)
    {
        double base = ((Constant *)e->base)->value;
        double exponent = ((Constant *)e->exponent)->value;
        destroyExponential(e);
        return (EVALABLE *)createConstant(pow(base, exponent));
    }
    return (EVALABLE *)e;
}

EVALABLE *optimizeLogarithm(Logarithm *l)
{
    l->base = optimize(l->base);
    l->value = optimize(l->value);
    if (EVALTYPE(l->base) == CONSTANT && EVALTYPE(l->value) == CONSTANT)
    {
        double base = ((Constant *)l->base)->value;
        double value = ((Constant *)l->value)->value;
        destroyLogarithm(l);
        return (EVALABLE *)createConstant(log(value) / log(base));
    }
    return (EVALABLE *)l;
}

EVALABLE *optimizeTrigonometric(Trigonometric *t)
{
    t->arg = optimize(t->arg);
    if (EVALTYPE(t->arg) == CONSTANT)
    {
        double val = ((Constant *)t->arg)->value;
        double result = 0;
        switch (t->trigType)
        {
            case SIN:
                result = sin(val);
                break;
            case COS:
                result = cos(val);
                break;
            case TAN:
                result = tan(val);
                break;
            case CSC:
                result = 1 / sin(val);
                break;
            case SEC:
                result = 1 / cos(val);
                break;
            case COT:
                result = 1 / tan(val);
                break;
        }
        destroyTrigonometric(t);
        return (EVALABLE *)createConstant(result);
    }
    return (EVALABLE *)t;
}

EVALABLE *optimizeInverseTrigonometric(InverseTrigonometric *it)
{
    it->arg = optimize(it->arg);
    if (EVALTYPE(it->arg) == CONSTANT)
    {
        double val = ((Constant *)it->arg)->value;
        double result = 0;
        switch (it->trigType)
        {
            case ASIN:
                result = asin(val);
                break;
            case ACOS:
                result = acos(val);
                break;
            case ATAN:
                result = atan(val);
                break;
            case ACSC:
                result = asin(1 / val);
                break;
            case ASEC:
                result = acos(1 / val);
                break;
            case ACOT:
                result = atan(1 / val);
                break;
        }
        destroyInverseTrigonometric(it);
        return (EVALABLE *)createConstant(result);
    }
    return (EVALABLE *)it;
}

/* Parser functions */

char *parseExpression(char *input, EVALABLE **e, StatusCode *s);
char *parseLogarithm(char *input, EVALABLE **e, StatusCode *s);
char *parseInsideParantheses(char *input, EVALABLE **e, StatusCode *s);
char *parseTrigonometric(char *input, EVALABLE **e, StatusCode *s);
char *parseInverseTrigonometric(char *input, EVALABLE **e, StatusCode *s);

char *parseTrigonometric(char *input, EVALABLE **e, StatusCode *s)
{
    if (s->code != 0)
        return input;
    TrigonometricType type;
    if (strncmp(input, "sin", 3) == 0)
        type = SIN;
    else if (strncmp(input, "cos", 3) == 0)
        type = COS;
    else if (strncmp(input, "tan", 3) == 0)
        type = TAN;
    else if (strncmp(input, "csc", 3) == 0)
        type = CSC;
    else if (strncmp(input, "sec", 3) == 0)
        type = SEC;
    else if (strncmp(input, "cot", 3) == 0)
        type = COT;
    else
        return input;
    input += 3;
    if (input[0] != '(')
    {
        s->code = 2;
        s->expected = '(';
        s->pos = input;
        return input;
    }
    EVALABLE *arg;
    input = parseInsideParantheses(input, &arg, s);
    *e = (EVALABLE *)createTrigonometric(type, arg);
    return input;
}

char *parseInverseTrigonometric(char *input, EVALABLE **e, StatusCode *s)
{
    if (s->code != 0)
        return input;
    InverseTrigonometricType type;
    if (strncmp(input, "asin", 4) == 0)
        type = ASIN;
    else if (strncmp(input, "acos", 4) == 0)
        type = ACOS;
    else if (strncmp(input, "atan", 4) == 0)
        type = ATAN;
    else if (strncmp(input, "acsc", 4) == 0)
        type = ACSC;
    else if (strncmp(input, "asec", 4) == 0)
        type = ASEC;
    else if (strncmp(input, "acot", 4) == 0)
        type = ACOT;
    else
        return input;
    input += 4;
    if (input[0] != '(')
    {
        s->code = 2;
        s->expected = '(';
        s->pos = input;
        return input;
    }
    EVALABLE *arg;
    input = parseInsideParantheses(input, &arg, s);
    *e = (EVALABLE *)createInverseTrigonometric(type, arg);
    return input;
}

char *parseInsideParantheses(char *input, EVALABLE **e, StatusCode *s)
{
    if (s->code != 0)
        return input;
    if (input[0] != '(')
    {
        s->code = 2;
        s->expected = '(';
        s->pos = input;
        return input;
    }
    int i = 1;
    int paranthesesCount = 1;
    while (paranthesesCount > 0)
    {
        if (input[i] == '(')
        {
            paranthesesCount++;
        }
        else if (input[i] == ')')
        {
            paranthesesCount--;
        }
        i++;
    }
    // char *insideParantheses = (char *)malloc(i);
    // strncpy(insideParantheses, input+1, i-2);
    // insideParantheses[i-2] = '\0';
    input[i - 1] = '\0';
    parseExpression(input + 1, e, s);
    input += i;
    return input;
}

char *parseLogarithm(char *input, EVALABLE **e, StatusCode *s)
{
    if (s->code != 0)
        return input;
    if (strncmp(input, "log", 3) != 0)
    {
        return input;
    }
    input += 3;
    if (input[0] != '_')
    {
        s->code = 2;
        s->expected = '_';
        s->pos = input;
        return input;
    }
    input++;
    EVALABLE *base;
    EVALABLE *value;
    input = parseInsideParantheses(input, &base, s);
    if (input[0] != '(')
    {
        s->code = 2;
        s->expected = '(';
        s->pos = input;
        return input;
    }
    input = parseInsideParantheses(input, &value, s);
    *e = (EVALABLE *)createLogarithm(base, value);
    return input;
}

/* Macro for checking if the argument is available
 * If the argument is available, the input is set to the previous character
 * and the current character is set to '*' to evaluate the expressions 
 * like 2x as 2*x
*/
#define CHECK_ARG_AVAILABLE() \
    if (isArgAvailable) \
    { \
        input--; \
        input[0] = '*'; \
        continue; \
    }

char *parseExpression(char *input, EVALABLE **e, StatusCode *s)
{
    if (s->code != 0)
        return input;
    SumChain *f = createSumChain();
    MulChain *m = createMulChain();
    EVALABLE *arg = NULL;
    int isPositive = 1;
    int isDivided = 0;
    int isArgAvailable = 0;
    while (input[0])
    {
        if (s->code != 0)
            return input;
        if (input[0] == ' ')
        {
            input++;
        }
        else if (strncmp(input, "log", 3) == 0)
        {
            CHECK_ARG_AVAILABLE();
            input = parseLogarithm(input, &arg, s);
            isArgAvailable = 1;
        }
        else if (strncmp(input, "ln", 2) == 0)
        {
            CHECK_ARG_AVAILABLE();
            input += 2;
            if (input[0] != '(')
            {
                s->code = 2;
                s->expected = '(';
                s->pos = input;
                return input;
            }
            input = parseInsideParantheses(input, &arg, s);
            arg = (EVALABLE *)createLogarithm((EVALABLE *)createConstant(M_E), arg);
            isArgAvailable = 1;
        }
        else if (
            strncmp(input, "sin", 3) == 0 || 
            strncmp(input, "cos", 3) == 0 || 
            strncmp(input, "tan", 3) == 0 || 
            strncmp(input, "csc", 3) == 0 || 
            strncmp(input, "sec", 3) == 0 || 
            strncmp(input, "cot", 3) == 0)
        {
            CHECK_ARG_AVAILABLE();
            input = parseTrigonometric(input, &arg, s);
            isArgAvailable = 1;
        }
        else if (
            strncmp(input, "asin", 4) == 0 ||
            strncmp(input, "acos", 4) == 0 ||
            strncmp(input, "atan", 4) == 0 ||
            strncmp(input, "acsc", 4) == 0 ||
            strncmp(input, "asec", 4) == 0 ||
            strncmp(input, "acot", 4) == 0)
        {
            CHECK_ARG_AVAILABLE();
            input = parseInverseTrigonometric(input, &arg, s);
            isArgAvailable = 1;
        }
        else if (input[0] == '(')
        {
            CHECK_ARG_AVAILABLE();
            input = parseInsideParantheses(input, &arg, s);
            isArgAvailable = 1;
        }
        else if (input[0] >= '0' && input[0] <= '9')
        {
            CHECK_ARG_AVAILABLE();
            double value = 0;
            while (input[0] >= '0' && input[0] <= '9')
            {
                value = value * 10 + (input[0] - '0');
                input++;
            }
            if (input[0] == '.')
            {
                input++;
                double decimal = 0;
                double divider = 10;
                while (input[0] >= '0' && input[0] <= '9')
                {
                    decimal += (input[0] - '0') / divider;
                    divider *= 10;
                    input++;
                }
                value += decimal;
            }
            arg = (EVALABLE *)createConstant(value);
            isArgAvailable = 1;
        }
        else if (input[0] == '^')
        {
            input++;
            EVALABLE *exponent;
            input = parseInsideParantheses(input, &exponent, s);
            if (s->code != 0)
                return input;
            arg = (EVALABLE *)createExponential(arg, exponent);
            isArgAvailable = 1;
        }
        else if (input[0] == 'x')
        {
            CHECK_ARG_AVAILABLE();
            input++;
            arg = (EVALABLE *)createVariable();
            isArgAvailable = 1;
        }
        else if (input[0] == 'e')
        {
            CHECK_ARG_AVAILABLE();
            input++;
            arg = (EVALABLE *)createConstant(M_E);
            isArgAvailable = 1;
        }
        else if (strncmp(input, "pi", 2) == 0 || (strncmp(input, "PI", 2) == 0))
        {
            CHECK_ARG_AVAILABLE();
            input += 2;
            arg = (EVALABLE *)createConstant(M_PI);
            isArgAvailable = 1;
        }
        else if (input[0] == '+' || input[0] == '-')
        {
            addMulChainArg(m, arg, isDivided);
            arg = NULL;
            EVALABLE *val;
            if (m->argCount == 1)
            {
                val = copyEvalable(m->args[0]);
                destroyMulChain(m);
            }
            else
            {
                val = (EVALABLE *)m;
            }
            addSumChainArg(f, val, isPositive);
            if (input[0] == '+')
            {
                isPositive = 1;
            }
            else if (input[0] == '-')
            {
                isPositive = 0;
            }
            m = createMulChain();
            input++;
            isArgAvailable = 0;
        }
        else if (input[0] == '*')
        {
            addMulChainArg(m, arg, isDivided);
            arg = NULL;
            isDivided = 0;
            input++;
            isArgAvailable = 0;
        }
        else if (input[0] == '/')
        {
            addMulChainArg(m, arg, isDivided);
            arg = NULL;
            isDivided = 1;
            input++;
            isArgAvailable = 0;
        }
        else 
        {
            s->code = 1;
            s->pos = input;
            return input;
        }
    }
    if (arg != NULL)
    {
        addMulChainArg(m, arg, isDivided);
    }
    EVALABLE *val;
    if (m->argCount == 1)
    {
        val = copyEvalable(m->args[0]);
        destroyMulChain(m);
    }
    else
    {
        val = (EVALABLE *)m;
    }
    addSumChainArg(f, (EVALABLE *)val, isPositive);
    EVALABLE *result;
    if (f->argCount == 1)
    {
        result = copyEvalable(f->args[0]);
        destroySumChain(f);
    }
    else
    {
        result = (EVALABLE *)f;
    }

    // Check if status code is set to 0 
    *e = result;
    return input;
}

/* Prototypes for solvers */

double solveBisection(EVALABLE *e, double a, double b, double epsilon);
double solveRegulaFalsi(EVALABLE *e, double a, double b, double epsilon);
double solveNewtonRaphson(EVALABLE *e, double x0, double epsilon);

/* integral functions */ 
double integrateTrapez(EVALABLE *e, double a, double b, int n);
double integrateSimpson13(EVALABLE *e, double a, double b, int n);
double integrateSimpson38(EVALABLE *e, double a, double b, int n);


/* solver implementations */

double solveBisection(EVALABLE *e, double a, double b, double epsilon)
{
    // TODO: Aralığı tanımsız yapan değer var mı kontrol et.
    double fa = evaluate(e, a);
    double fb = evaluate(e, b);
    double c = 0;
    if (fa * fb > 0)
    {
        return NAN;
    }
    while ((b - a) > epsilon)
    {
        c = (a + b) / 2;
        double fc = evaluate(e, c);
        if (fc == 0)
        {
            return c;
        }
        else if (fa * fc < 0)
        {
            b = c;
            fb = fc;
        } else {
            a = c;
            fa = fc;
        }
    }
    return c;
}

double solveRegulaFalsi(EVALABLE *e, double a, double b, double epsilon)
{
    double fa = evaluate(e, a);
    double fb = evaluate(e, b);
    double c = 0;
    if (fa * fb > 0)
    {
        return NAN;
    }
    while ((b - a) > epsilon)
    {
        c = (a * fb - b * fa) / (fb - fa);
        double fc = evaluate(e, c);
        if (fa * fc < 0)
        {
            b = c;
            fb = fc;
        } else {
            a = c;
            fa = fc;
        }
    }
    return c;
}

double solveNewtonRaphson(EVALABLE *e, double x0, double epsilon)
{
    double x = x0;
    double fx = evaluate(e, x);
    double dfx = (evaluate(e, x + epsilon) - fx) / epsilon;
    while (ABS(fx) > epsilon)
    {
        x = x - fx / dfx;
        fx = evaluate(e, x);
        dfx = (evaluate(e, x + epsilon) - fx) / epsilon;
    }
    return x;
}

double integrateTrapez(EVALABLE *e, double a, double b, int n)
{
    double h = (b - a) / n;
    double sum = 0;
    for (int i = 1; i < n; i++)
    {
        sum += evaluate(e, a + i * h);
    }
    return h * (evaluate(e, a) + evaluate(e, b) + 2 * sum) / 2;
}

double integrateSimpson13(EVALABLE *e, double a, double b, int n)
{
    if (n % 2 != 0)
    {
        printf("n must be even for Simpson's 1/3 rule.\n");
        return NAN;
    }
    double h = (b - a) / n;
    double sum1 = 0;
    double sum2 = 0;
    for (int i = 1; i < n; i++)
    {
        if (i % 2 == 0)
        {
            sum1 += evaluate(e, a + i * h);
        } else
        {
            sum2 += evaluate(e, a + i * h);
        }
    }
    return (h / 3) * (evaluate(e, a) + evaluate(e, b) + 2 * sum1 + 4 * sum2);
}

double integrateSimpson38(EVALABLE *e, double a, double b, int n)
{
    double sum = 0;
    double h = (b - a) / n;
    for (int i = 0; i < n; i++)
    {
        b = a + h;
        sum += (b - a) / 8 * (evaluate(e, a) + 3 * evaluate(e, (a + (b-a)/3)) + 3 * evaluate(e, (a + 2*(b-a)/3)) + evaluate(e, b));
        a = b;
    }
    return sum;
}

/* Matrix implementation */

typedef struct
{
    int rows;
    int cols;
    double **data;
} Matrix;

Matrix *createMatrix(int rows, int cols);
void destroyMatrix(Matrix *m);
void printMatrix(Matrix *m);
Matrix *copyMatrix(Matrix *m);

/* 
 * Add two matrices. m1 + scalar * m2
 *
 * Parameters:
 * - m1: The first matrix
 * - m2: The second matrix
 * - scalar: The scalar to multiply the second matrix
 * Returns:
 * - The result of the addition
*/
Matrix *addMatrix(Matrix *m1, Matrix *m2, double scalar);

/* 
 * Multiply two matrices. m1 * m2
 *
 * Parameters:
 * - m1: The first matrix
 * - m2: The second matrix
 * Returns:
 * - The result of the multiplication
*/

/* 
 * Multiply a matrix with a scalar. scalar * m
 *
 * Parameters:
 * - m: The matrix
 * - scalar: The scalar
 * Returns:
 * - The result of the multiplication
*/
Matrix *multiplyMatrix(Matrix *m1, Matrix *m2);

/* 
 * Transpose a matrix
 *
 * Parameters:
 * - m: The matrix
 * Returns:
 * - The transposed matrix
*/
Matrix *transposeMatrix(Matrix *m);

/* 
 * Inverse of a matrix
 *
 * Parameters:
 * - m: The matrix
 * Returns:
 * - The inverse of the matrix
*/
Matrix *inverseMatrix(Matrix *m);

/* 
 * Solve a linear system of equations
 *
 * Parameters:
 * - A: The coefficient matrix
 * - B: The constant matrix
 * Returns:
 * - The solution matrix
*/
Matrix *solveLinearSystem(Matrix *A, Matrix *B);

/* 
 * Calculate the determinant of a matrix
 *
 * Parameters:
 * - m: The matrix
 * Returns:
 * - The determinant of the matrix
*/
double determinantMatrix(Matrix *m);

/* 
 * Add a row to another row. r1 = r1 + scalar * r2
 *
 * Parameters:
 * - m: The matrix
 * - r1: The row to be added
 * - r2: The row to be added
 * - scalar: The scalar to multiply the row to be added
*/
void addRow(Matrix *m, int r1, int r2, double scalar);

/* 
 * Multiply a row with a scalar. r = scalar * r
 *
 * Parameters:
 * - m: The matrix
 * - r: The row to be multiplied
 * - scalar: The scalar to multiply the row
*/
void multiplyRow(Matrix *m, int r, double scalar); 

/* 
 * Swap two rows. r1 <-> r2
 *
 * Parameters:
 * - m: The matrix
 * - r1: The first row
 * - r2: The second row
*/
void swapRows(Matrix *m, int r1, int r2); 

/* 
 * Add a column to another column. c1 = c1 + scalar * c2
 *
 * Parameters:
 * - m: The matrix
 * - c1: The column to be added
 * - c2: The column to be added
 * - scalar: The scalar to multiply the column to be added
*/
void addColumn(Matrix *m, int c1, int c2, double scalar);

/* 
 * Multiply a column with a scalar. c = scalar * c
 *
 * Parameters:
 * - m: The matrix
 * - c: The column to be multiplied
 * - scalar: The scalar to multiply the column
*/
void multiplyColumn(Matrix *m, int c, double scalar);

/* 
 * Swap two columns. c1 <-> c2
 *
 * Parameters:
 * - m: The matrix
 * - c1: The first column
 * - c2: The second column
*/
void swapColumns(Matrix *m, int c1, int c2);

Matrix *createMatrix(int rows, int cols)
{
    Matrix *m = (Matrix *)malloc(sizeof(Matrix));
    m->rows = rows;
    m->cols = cols;
    m->data = (double **)malloc(rows * sizeof(double *));
    for (int i = 0; i < rows; i++)
    {
        m->data[i] = (double *)malloc(cols * sizeof(double));
        for (int j = 0; j < cols; j++)
        {
            m->data[i][j] = 0;
        }
    }
    return m;
}

void destroyMatrix(Matrix *m)
{
    for (int i = 0; i < m->rows; i++)
    {
        free(m->data[i]);
    }
    free(m->data);
    free(m);
}

void printMatrix(Matrix *m)
{
    for (int i = 0; i < m->rows; i++)
    {
        for (int j = 0; j < m->cols; j++)
        {
            printf("%Lf ", m->data[i][j]);
        }
        printf("\n");
    }
}

Matrix *copyMatrix(Matrix *m)
{
    Matrix *copy = createMatrix(m->rows, m->cols);
    for (int i = 0; i < m->rows; i++)
    {
        for (int j = 0; j < m->cols; j++)
        {
            copy->data[i][j] = m->data[i][j];
        }
    }
    return copy;
}

Matrix *addMatrix(Matrix *m1, Matrix *m2, double scalar)
{
    if (m1->rows != m2->rows || m1->cols != m2->cols)
    {
        return NULL;
    }
    Matrix *result = createMatrix(m1->rows, m1->cols);
    for (int i = 0; i < m1->rows; i++)
    {
        for (int j = 0; j < m1->cols; j++)
        {
            result->data[i][j] = m1->data[i][j] + scalar * m2->data[i][j];
        }
    }
    return result;
}

Matrix *multiplyMatrix(Matrix *m1, Matrix *m2)
{
    if (m1->cols != m2->rows)
    {
        return NULL;
    }
    Matrix *result = createMatrix(m1->rows, m2->cols);
    for (int i = 0; i < m1->rows; i++)
    {
        for (int j = 0; j < m2->cols; j++)
        {
            for (int k = 0; k < m1->cols; k++)
            {
                result->data[i][j] += m1->data[i][k] * m2->data[k][j];
            }
        }
    }
    return result;
}

Matrix *transposeMatrix(Matrix *m)
{
    Matrix *result = createMatrix(m->cols, m->rows);
    for (int i = 0; i < m->rows; i++)
    {
        for (int j = 0; j < m->cols; j++)
        {
            result->data[j][i] = m->data[i][j];
        }
    }
    return result;
}

Matrix *inverseMatrix(Matrix *m)
{
    if (m->rows != m->cols)
    {
        return NULL;
    }

    if (determinantMatrix(m) == 0)
    {
        return NULL;
    }

    Matrix *identity = createMatrix(m->rows, m->cols);
    for (int i = 0; i < m->rows; i++)
    {
        identity->data[i][i] = 1;
    }

    Matrix *result = copyMatrix(m);
    for (int i = 0; i < result->rows; i++)
    {
        double pivot = result->data[i][i];
        if (pivot == 0)
        {
            for (int j = i + 1; j < result->rows; j++)
            {
                if (result->data[j][i] != 0)
                {
                    swapRows(result, i, j);
                    swapRows(identity, i, j);
                    break;
                }
            }
            pivot = result->data[i][i];
        }
        multiplyRow(result, i, 1 / pivot);
        multiplyRow(identity, i, 1 / pivot);
        for (int j = 0; j < result->rows; j++)
        {
            if (j != i)
            {
                double scalar = -result->data[j][i];
                addRow(result, j, i, scalar);
                addRow(identity, j, i, scalar);
            }
        }
    }
    destroyMatrix(result);
    return identity;
} 

double determinantMatrix(Matrix *m)
{
    if (m->rows != m->cols)
    {
        return NAN;
    }

    if (m->rows == 1)
    {
        return m->data[0][0];
    }

    double det = 0;
    for (int i = 0; i < m->rows; i++)
    {
        Matrix *minor = createMatrix(m->rows - 1, m->cols - 1);
        for (int j = 1; j < m->rows; j++)
        {
            for (int k = 0; k < m->cols; k++)
            {
                if (k < i)
                {
                    minor->data[j - 1][k] = m->data[j][k];
                } else if (k > i)
                {
                    minor->data[j - 1][k - 1] = m->data[j][k];
                }
            }
        }
        double sign = (i % 2 == 0) ? 1 : -1;
        det += sign * m->data[0][i] * determinantMatrix(minor);
        destroyMatrix(minor);
    }

    return det;
}

void addRow(Matrix *m, int r1, int r2, double scalar)
{
    for (int i = 0; i < m->cols; i++)
    {
        m->data[r1][i] += scalar * m->data[r2][i];
    }
}

void multiplyRow(Matrix *m, int r, double scalar)
{
    for (int i = 0; i < m->cols; i++)
    {
        m->data[r][i] *= scalar;
    }
}

void swapRows(Matrix *m, int r1, int r2)
{
    double *temp = m->data[r1];
    m->data[r1] = m->data[r2];
    m->data[r2] = temp;
}

void addColumn(Matrix *m, int c1, int c2, double scalar)
{
    for (int i = 0; i < m->rows; i++)
    {
        m->data[i][c1] += scalar * m->data[i][c2];
    }
}

void multiplyColumn(Matrix *m, int c, double scalar)
{
    for (int i = 0; i < m->rows; i++)
    {
        m->data[i][c] *= scalar;
    }
}

void swapColumns(Matrix *m, int c1, int c2)
{
    for (int i = 0; i < m->rows; i++)
    {
        double temp = m->data[i][c1];
        m->data[i][c1] = m->data[i][c2];
        m->data[i][c2] = temp;
    }
}


int main()
{
    char banner[] = 
" _____                                                 _____ \n"
"( ___ )-----------------------------------------------( ___ )\n"
" |   |                                                 |   | \n"
" |   |  _   _                           _           _  |   | \n"
" |   | | \\ | |_   _ _ __ ___   ___ _ __(_) ___ __ _| | |   | \n"
" |   | |  \\| | | | | '_ ` _ \\ / _ \\ '__| |/ __/ _` | | |   | \n"
" |   | | |\\  | |_| | | | | | |  __/ |  | | (_| (_| | | |   | \n"
" |   | |_| \\_|\\__,_|_| |_| |_|\\___|_|  |_|\\___\\__,_|_| |   | \n"
" |   |    / \\   _ __   __ _| |_   _ ___(_)___          |   | \n"
" |   |   / _ \\ | '_ \\ / _` | | | | / __| / __|         |   | \n"
" |   |  / ___ \\| | | | (_| | | |_| \\__ \\ \\__ \\         |   | \n"
" |   | /_/   \\_\\_| |_|\\__,_|_|\\__, |___/_|___/         |   | \n"
" |   |                        |___/                    |   | \n"
" |___|                                       byEnsarGok|___| \n"
"(_____)-----------------------------------------------(_____)\n";

    printf("%s", banner);


    Matrix *m = createMatrix(3, 3);
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            scanf("%Lf", &m->data[i][j]);
        }
    }

    printMatrix(m);
    printf("Inverse:\n"); 
    Matrix *inverse = inverseMatrix(m);
    printMatrix(inverse);
    destroyMatrix(m);

    return 0;

    // success code, 0 means no error
    // first byte of the input is the error code
    // the rest is the id of the character that caused the error
    StatusCode status;
    status.code = 0;
    status.pos = NULL;
    status.expected = '\0';

    EVALABLE *f;
    printf("Enter your function: ");
    char input[256];
    if (fgets(input, 256, stdin) == NULL)
    {
        printf("Failed to read input.\n");
        return 1;
    }
    input[strlen(input) - 1] = '\0';
    parseExpression(input, &f, &status);
    if (status.code != 0)
    {
        int pos = status.pos - input + 1;
        switch (status.code)
        {
            case 1:
                printf("[Failed to parse]   ");
                for (int i = 0; i < pos; i++)
                {
                    printf(" ");
                }
                printf("^ Unexpected character.\n");
                break;
            case 2:
                printf("[Failed to parse]   ");
                for (int i = 0; i < pos; i++)
                {
                    printf(" ");
                }
                printf("^ Expected '%c', received '%c'\n", status.expected, *status.pos);
                break;
        }
        return 1;
    }
    f = optimize(f);
    printf("f(x) = ");
    print(f);
    printf("\n");
    
    destroy(f);

    return 0;
}

