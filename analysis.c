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

#define MAX_FUNC_ARGS 256

#define double long double

double evaluate(EVALABLE *e, double value);
#define EVALTYPE(x) ((EvalAble *)x)->type

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
    return value;
}

void printVariable(Variable *v)
{
    printf("(x)");
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
    m->args[m->argCount++] = arg;
    m->isDivided[m->argCount] = isDivided;
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
                printf(" / ");
            } else {
                printf(" * ");
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
    for (int i = 0; i < f->argCount && f->args[i] != NULL; i++)
    {
        if (f->isPositive[i] == 1)
        {
            printf("+");
        } else
        {
            printf("-");
        }
        print(f->args[i]);
    }
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

/* Prototypes for solvers */

double solveBisection(EVALABLE *e, double a, double b, double epsilon);
double solveRegulaFalsi(EVALABLE *e, double a, double b, double epsilon);



/* solver implementations */

double solveBisection(EVALABLE *e, double a, double b, double epsilon)
{
    // TODO: Aralığı tanımsız yapan değer var mı kontrol et.
    double fa = evaluate(e, a);
    double fb = evaluate(e, b);
    double c;
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
    double c;
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

/* Parser functions */

char *parseExpression(char *input, EVALABLE **e);
char *parseLogarithm(char *input, EVALABLE **e);
char *parseInsideParantheses(char *input, EVALABLE **e);
char *parseTrigonometric(char *input, EVALABLE **e);


char *parseTrigonometric(char *input, EVALABLE **e)
{
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
        printf("Error: Expected '(' after trigonometric function\n");
        return input;
    }
    EVALABLE *arg;
    input = parseInsideParantheses(input, &arg);
    *e = (EVALABLE *)createTrigonometric(type, arg);
    return input;
}

char *parseInverseTrigonometric(char *input, EVALABLE **e)
{
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
        printf("Error: Expected '(' after inverse trigonometric function\n");
        return input;
    }
    EVALABLE *arg;
    input = parseInsideParantheses(input, &arg);
    *e = (EVALABLE *)createInverseTrigonometric(type, arg);
    return input;
}

char *parseInsideParantheses(char *input, EVALABLE **e)
{
    if (input[0] != '(')
    {
        printf("Error: Expected '(' at the beginning of parantheses\n");
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
    char *insideParantheses = (char *)malloc(i);
    strncpy(insideParantheses, input+1, i-2);
    insideParantheses[i] = '\0';
    input += i;
    printf("Inside parantheses: %s\n", insideParantheses);
    parseExpression(insideParantheses, e);
    free(insideParantheses);
    return input;
}

char *parseLogarithm(char *input, EVALABLE **e)
{
    printf("Parsing logarithm\n");
    if (input[0] != 'l' || input[1] != 'o' || input[2] != 'g')
    {
        return input;
    }
    input += 3;
    if (input[0] != '_')
    {
        printf("Error: Expected '_' after 'log'\n");
        return input;
    }
    input++;
    EVALABLE *base;
    input = parseInsideParantheses(input, &base);
    if (input[0] != '(')
    {
        printf("Error: Expected '(' after base of logarithm\n");
        return input;
    }
    EVALABLE *value;
    input = parseInsideParantheses(input, &value);
    *e = (EVALABLE *)createLogarithm(base, value);
    return input;
}

char *parseExpression(char *input, EVALABLE **e)
{
    SumChain *f = createSumChain();
    MulChain *m = createMulChain();
    EVALABLE *arg;
    int isPositive = 1;
    int isDivided = 0;
    // 0: Start
    // 1: Coefficient
    while (input[0])
    {
        // Check if it is a constant
        if (input[0] == ' ')
        {
            input++;
            continue;
        }
        if (strncmp(input, "log", 3) == 0)
        {
            input = parseLogarithm(input, &arg);
        }
        else if (
            strncmp(input, "sin", 3) == 0 || 
            strncmp(input, "cos", 3) == 0 || 
            strncmp(input, "tan", 3) == 0 || 
            strncmp(input, "csc", 3) == 0 || 
            strncmp(input, "sec", 3) == 0 || 
            strncmp(input, "cot", 3) == 0)
        {
            input = parseTrigonometric(input, &arg);
        }
        else if (
            strncmp(input, "asin", 4) == 0 ||
            strncmp(input, "acos", 4) == 0 ||
            strncmp(input, "atan", 4) == 0 ||
            strncmp(input, "acsc", 4) == 0 ||
            strncmp(input, "asec", 4) == 0 ||
            strncmp(input, "acot", 4) == 0)
        {
            input = parseInverseTrigonometric(input, &arg);
        }
        else if (input[0] == '(')
        {
            EVALABLE *temp;
            input = parseInsideParantheses(input, &temp);
        }
        else if (input[0] >= '0' && input[0] <= '9')
        {
            printf("Parsing constant\n");
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
            printf("Constant: %Lf\n", value);
        }
        else if (input[0] == '^')
        {
            printf("Parsing exponential\n");
            input++;
            EVALABLE *exponent;
            input = parseInsideParantheses(input, &exponent);
            arg = (EVALABLE *)createExponential(arg, exponent);
        }
        else if (input[0] == 'x')
        {
            arg = (EVALABLE *)createVariable();
            input++;
        }
        else if (input[0] == 'e')
        {
            input++;
            arg = (EVALABLE *)createConstant(M_E);
        }
        else if (input[0] == '+')
        {
            printf("Parsing addition\n");
            printf("Arg: ");
            print(arg);
            printf("\n");

            addMulChainArg(m, arg, isDivided);
            arg = NULL;
            printf("MulChain: ");
            print((EVALABLE *)m);
            printf("\nArgcount: %d\n", m->argCount);
            printf("isPositive: %d\n", isPositive);

            EVALABLE *val;
            if (m->argCount == 1)
            {
                val = m->args[0];
            }
            else
            {
                val = (EVALABLE *)m;
            }
            addSumChainArg(f, val, isPositive);

            m = createMulChain();
            printf("SumChain: ");
            print((EVALABLE *)f);
            printf("\n");
            isPositive = 1;
            isDivided = 0;
            input++;
        }
        else if (input[0] == '-')
        {
            addMulChainArg(m, arg, isDivided);
            arg = NULL;
            addSumChainArg(f, (EVALABLE *)m, isPositive);
            isPositive = 0;
            isDivided = 0;
            input++;
        }
        else if (input[0] == '*')
        {
            printf("Parsing multiplication\n");
            addMulChainArg(m, arg, isDivided);
            arg = NULL;
            isDivided = 0;
            input++;
        }
        else if (input[0] == '/')
        {
            addMulChainArg(m, arg, isDivided);
            arg = NULL;
            isDivided = 1;
            input++;
        }
    }
    if (arg != NULL)
    {
        printf("Adding last arg\n");
        printf("Arg: ");
        print(arg);
        printf("\n");
        addMulChainArg(m, arg, isDivided);
    }
    printf("Adding last mulchain\n");
    printf("function before: ");
    print((EVALABLE *)f);
    printf("\n");
    addSumChainArg(f, (EVALABLE *)m, isPositive);
    printf("SumChain after: ");
    print((EVALABLE *)f);
    printf("\n");
    printf("Mulchain length: %d\n", m->argCount);
    printf("Sumchain length: %d\n", f->argCount);
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

    *e = result;
    return input;
}

int main()
{
    EVALABLE *f;
    printf("Enter the function: ");
    char input[256];
    fgets(input, 256, stdin);
    input[strlen(input) - 1] = '\0';
    parseExpression(input, &f);
    print(f);
    printf("\n");
    printf("Enter the value to evaluate: ");
    double value;
    scanf("%Lf", &value);
    double result = evaluate(f, value);
    printf("Result: %Lf\n", result);
    
    printf("Type: ");
    switch (EVALTYPE(f))
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

    destroy(f);

    return 0;
}

