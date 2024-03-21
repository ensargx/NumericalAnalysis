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

typedef enum _EvalAbleType {
    CONSTANT,
    VARIABLE,
    FUNCTION,
    LOGARITHM,
    EXPONENTIAL,
    TRIGONOMETRIC,
    INVERSE_TRIGONOMETRIC,
} EvalAbleType;

/* Struct Prototypes */
typedef struct _Constant Constant;
typedef struct _Variable Variable;
typedef struct _Function Function;
typedef struct _Exponential Exponential;
typedef struct _Logarithm Logarithm;
typedef struct _Trigonometric Trigonometric;
typedef enum _TrigonometricType TrigonometricType;
typedef struct _InverseTrigonometric InverseTrigonometric;


typedef struct _EvalAble {
    EvalAbleType type;
} EvalAble;

typedef struct _Function Function;
typedef struct _Constant Constant;
typedef struct _Variable Variable;
typedef struct _Exponential Exponential;

EvalAbleType checkType(EVALABLE e) {
    return ((EvalAble *)e)->type;
}

typedef struct _Constant {
    EvalAbleType type;
    double value;
} Constant;

typedef struct _Variable {
    EvalAbleType type;
    EVALABLE *cofactor;
} Variable;

typedef struct _Function {
    EvalAbleType type;
    int argCount;
    EVALABLE *args[256];
} Function;

typedef struct _Exponential {
    EvalAbleType type;
    EVALABLE *cofactor;
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
    EVALABLE *cofactor;
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
    EVALABLE *cofactor;
    EVALABLE *arg;
} InverseTrigonometric;

typedef struct _Logarithm {
    EvalAbleType type;
    EVALABLE *cofactor;
    EVALABLE *base;
    EVALABLE *value;
} Logarithm;

/* Function prototypes */
Variable *createVariable(EVALABLE *cofactor);
void destroyVariable(Variable *v);
double evaluateVariable(Variable *v, double value);
EVALABLE *deriveVariable(Variable *v);
void printVariable(Variable *v);

Constant *createConstant(double value);
void destroyConstant(Constant *c);
double evaluateConstant(Constant *c, double value);
EVALABLE *deriveConstant(Constant *c);
void printConstant(Constant *c);

Exponential *createExponential(EVALABLE *cofactor, EVALABLE *base, EVALABLE *exponent);
void destroyExponential(Exponential *e);
double evaluateExponential(Exponential *e, double value);
EVALABLE *deriveExponential(Exponential *e);    
void printExponential(Exponential *e);

Logarithm *createLogarithm(EVALABLE *cofactor, EVALABLE *base, EVALABLE *value);
void destroyLogarithm(Logarithm *l);
double evaluateLogarithm(Logarithm *l, double value);
EVALABLE *deriveLogarithm(Logarithm *l);
void printLogarithm(Logarithm *l);

Function *createFunction();
void addFunctionArg(Function *f, EVALABLE *argType);
void destroyFunction(Function *f);
double evaluateFunction(Function *f, double value);
EVALABLE *deriveFunction(Function *f);
void printFunction(Function *f);

Trigonometric *createTrigonometric(TrigonometricType type, EVALABLE *cofactor, EVALABLE *arg);
void destroyTrigonometric(Trigonometric *t);
double evaluateTrigonometric(Trigonometric *t, double value);
EVALABLE *deriveTrigonometric(Trigonometric *t);
void printTrigonometric(Trigonometric *t);

InverseTrigonometric *createInverseTrigonometric(InverseTrigonometricType type, EVALABLE *cofactor, EVALABLE *arg);
void destroyInverseTrigonometric(InverseTrigonometric *it);
double evaluateInverseTrigonometric(InverseTrigonometric *it, double value);
EVALABLE *deriveInverseTrigonometric(InverseTrigonometric *it);
void printInverseTrigonometric(InverseTrigonometric *it);

double evaluate(EVALABLE *e, double value);
void destroy(EVALABLE *e);
EVALABLE *derive(EVALABLE *e);
void print(EVALABLE *e);

/* Function implementations */

// Variable functions
Variable *createVariable(EVALABLE *cofactor) {
    Variable *v = (Variable *)malloc(sizeof(Variable));
    v->type = VARIABLE;
    if (cofactor == NULL) {
        cofactor = (EVALABLE *)createConstant(1);
    }
    v->cofactor = cofactor;
    return v;
}

void destroyVariable(Variable *v) {
    destroy(v->cofactor);
    free(v);
}

double evaluateVariable(Variable *v, double value) {
    return evaluate(v->cofactor, value) * value;
}

EVALABLE *deriveVariable(Variable *v) {
    // (EVALABLE)*x
    EvalAbleType type = checkType(v->cofactor);
    if (type == CONSTANT) {
        Constant *c = (Constant *)v->cofactor;
        return (EVALABLE *)createConstant(c->value);
    } 
    EVALABLE *derivedCofactor = derive(v->cofactor);
    Function *f = createFunction();
    addFunctionArg(f, (EVALABLE *)derivedCofactor);
    addFunctionArg(f, (EVALABLE *)createConstant(1));
    return (EVALABLE *)f;
}

void printVariable(Variable *v) {
    printf("(");
    print(v->cofactor);
    printf(")x");
}

// Constant functions
Constant *createConstant(double value) {
    Constant *c = (Constant *)malloc(sizeof(Constant));
    c->type = CONSTANT;
    c->value = value;
    return c;
}

void destroyConstant(Constant *c) {
    free(c);
}

double evaluateConstant(Constant *c, double value) {
    return c->value;
}

EVALABLE *deriveConstant(Constant *c) {
    return NULL;
}

void printConstant(Constant *c) {
    printf("%Lf", c->value);
}

// Exponential functions
Exponential *createExponential(EVALABLE *cofactor, EVALABLE *base, EVALABLE *exponent) {
    Exponential *e = (Exponential *)malloc(sizeof(Exponential));
    e->type = EXPONENTIAL;
    if (cofactor == NULL) {
        cofactor = (EVALABLE *)createConstant(1);
    }
    e->cofactor = cofactor;
    if (base == NULL) {
        base = (EVALABLE *)createConstant(1);
    }
    e->base = base;
    if (exponent == NULL) {
        exponent = (EVALABLE *)createConstant(1);
    }
    e->exponent = exponent;
    return e;
}

void destroyExponential(Exponential *e) {
    free(e);
}

double evaluateExponential(Exponential *e, double value) {
    return evaluate(e->cofactor, value) * pow(evaluate(e->base, value), evaluate(e->exponent, value));
}

EVALABLE *deriveExponential(Exponential *e) 
{
    // (EVALABLE)*x
    EvalAbleType type = checkType(e->cofactor);
    if (type == CONSTANT) {
        Constant *c = (Constant *)e->cofactor;
        return (EVALABLE *)createConstant(c->value);
    }
    EVALABLE *derivedCofactor = derive(e->cofactor);
    Function *f = createFunction();
    addFunctionArg(f, (EVALABLE *)derivedCofactor);
    addFunctionArg(f, (EVALABLE *)createExponential(NULL, e->base, e->exponent));
    return (EVALABLE *)f;
}

void printExponential(Exponential *e) {
    printf("(");
    print(e->cofactor);
    printf(")(");
    print(e->base);
    printf(")^(");
    print(e->exponent);
    printf(")");
}

// Trigonometric functions

Trigonometric *createTrigonometric(TrigonometricType type, EVALABLE *cofactor, EVALABLE *arg) {
    Trigonometric *t = (Trigonometric *)malloc(sizeof(Trigonometric));
    t->type = TRIGONOMETRIC;
    if (cofactor == NULL) {
        cofactor = (EVALABLE *)createConstant(1);
    }
    t->cofactor = cofactor;
    if (arg == NULL) {
        arg = (EVALABLE *)createConstant(1);
    }
    t->arg = arg;
    return t;
}

void destroyTrigonometric(Trigonometric *t) {
    destroy(t->cofactor);
    destroy(t->arg);
    free(t);
}

double evaluateTrigonometric(Trigonometric *t, double value) {
    switch (t->trigType) {
        case SIN:
            return evaluate(t->cofactor, value) * sin(evaluate(t->arg, value));
        case COS:
            return evaluate(t->cofactor, value) * cos(evaluate(t->arg, value));
        case TAN:
            return evaluate(t->cofactor, value) * tan(evaluate(t->arg, value));
        case CSC:
            return evaluate(t->cofactor, value) * 1 / sin(evaluate(t->arg, value));
        case SEC:
            return evaluate(t->cofactor, value) * 1 / cos(evaluate(t->arg, value));
        case COT:
            return evaluate(t->cofactor, value) * 1 / tan(evaluate(t->arg, value));
    }
}

EVALABLE *deriveTrigonometric(Trigonometric *t) {
    return NULL;
}

void printTrigonometric(Trigonometric *t) {
    switch (t->trigType) {
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

InverseTrigonometric *createInverseTrigonometric(InverseTrigonometricType type, EVALABLE *cofactor, EVALABLE *arg) {
    InverseTrigonometric *it = (InverseTrigonometric *)malloc(sizeof(InverseTrigonometric));
    it->type = INVERSE_TRIGONOMETRIC;
    if (cofactor == NULL) {
        cofactor = (EVALABLE *)createConstant(1);
    }
    it->cofactor = cofactor;
    if (arg == NULL) {
        arg = (EVALABLE *)createConstant(1);
    }
    it->arg = arg;
    return it;
}

void destroyInverseTrigonometric(InverseTrigonometric *it) {
    destroy(it->cofactor);
    destroy(it->arg);
    free(it);
}

double evaluateInverseTrigonometric(InverseTrigonometric *it, double value) {
    switch (it->trigType) {
        case ASIN:
            return evaluate(it->cofactor, value) * asin(evaluate(it->arg, value));
        case ACOS:
            return evaluate(it->cofactor, value) * acos(evaluate(it->arg, value));
        case ATAN:
            return evaluate(it->cofactor, value) * atan(evaluate(it->arg, value));
        case ACSC:
            return evaluate(it->cofactor, value) * asin(1 / evaluate(it->arg, value));
        case ASEC:
            return evaluate(it->cofactor, value) * acos(1 / evaluate(it->arg, value));
        case ACOT:
            return evaluate(it->cofactor, value) * atan(1 / evaluate(it->arg, value));
    }
}

EVALABLE *deriveInverseTrigonometric(InverseTrigonometric *it) {
    return NULL;
}

void printInverseTrigonometric(InverseTrigonometric *it) {
    switch (it->trigType) {
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

Logarithm *createLogarithm(EVALABLE *cofactor, EVALABLE *base, EVALABLE *value) {
    Logarithm *l = (Logarithm *)malloc(sizeof(Logarithm));
    l->type = LOGARITHM;
    if (cofactor == NULL) {
        cofactor = (EVALABLE *)createConstant(1);
    }
    l->cofactor = cofactor;
    if (base == NULL) {
        base = (EVALABLE *)createConstant(10);
    }
    l->base = base;
    if (value == NULL) {
        value = (EVALABLE *)createConstant(1);
    }
    l->value = value;
    return l;
}

void destroyLogarithm(Logarithm *l) {
    destroy(l->cofactor);
    destroy(l->base);
    destroy(l->value);
    free(l);
}

double evaluateLogarithm(Logarithm *l, double value) {
    // printf("[DEBUG]: evaluateLogarithm: Check implementation & test\n");
    // log_b(a) = log(a) / log(b) 
    return evaluate(l->cofactor, value) * log(evaluate(l->value, value)) / log(evaluate(l->base, value));
}

EVALABLE *deriveLogarithm(Logarithm *l) {
    return NULL;
}

void printLogarithm(Logarithm *l) {
    printf("(");
    print(l->cofactor);
    printf(")log_");
    print(l->base);
    printf("(");
    print(l->value);
    printf(")");
}

// Function functions

Function *createFunction() {
    Function *f = (Function *)malloc(sizeof(Function));
    f->type = FUNCTION;
    f->argCount = 0;
    for (int i = 0; i < MAX_FUNC_ARGS; i++) {
        f->args[i] = NULL;
    }
    return f;
}

void addFunctionArg(Function *f, EVALABLE *arg) {
    f->args[f->argCount++] = arg;
}

void destroyFunction(Function *f) {
    free(f);
}

double evaluateFunction(Function *f, double value) {
    double result = 0;
    for (int i = 0; i < f->argCount && f->args[i] != NULL; i++) {
        result += evaluate((EVALABLE *)f->args[i], value);
    }
    return result;
}

EVALABLE *deriveFunction(Function *f) {
    Function *df = createFunction();
    for (int i = 0; i < f->argCount && f->args[i] != NULL; i++) {
        addFunctionArg(df, derive((EVALABLE *)f->args[i]));
    }
    return (EVALABLE *)df;
}

void printFunction(Function *f) {
    printf("(");
    for (int i = 0; i < f->argCount && f->args[i] != NULL; i++) {
        print((EVALABLE *)f->args[i]);
        if (i < f->argCount - 1 && f->args[i + 1] != NULL) {
            printf(" + ");
        }
    }
    printf(")");
}

// Main functions

void destroy(EVALABLE *e) {
    switch (checkType(e)) {
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
        case FUNCTION:
            destroyFunction((Function *)e);
            break;
    }
}

double evaluate(EVALABLE *e, double value) {
    switch (checkType(e)) {
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
        case FUNCTION:
            return evaluateFunction((Function *)e, value);
    }
}

EVALABLE *derive(EVALABLE *e) {
    switch (checkType(e)) {
        case CONSTANT:
            return deriveConstant((Constant *)e);
        case VARIABLE:
            return deriveVariable((Variable *)e);
        case EXPONENTIAL:
            return deriveExponential((Exponential *)e);
        case TRIGONOMETRIC:
            return deriveTrigonometric((Trigonometric *)e);
        case INVERSE_TRIGONOMETRIC:
            return deriveInverseTrigonometric((InverseTrigonometric *)e);
        case LOGARITHM:
            return deriveLogarithm((Logarithm *)e);
        case FUNCTION:
            return deriveFunction((Function *)e);
    }
}

void print(EVALABLE *e) {
    switch (checkType(e)) {
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
        case FUNCTION:
            printFunction((Function *)e);
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
    if (fa * fb > 0) {
        return NAN;
    }
    while ((b - a) > epsilon) {
        c = (a + b) / 2;
        double fc = evaluate(e, c);
        if (fc == 0) {
            return c;
        }
        else if (fa * fc < 0) {
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
    if (fa * fb > 0) {
        return NAN;
    }
    while ((b - a) > epsilon) {
        c = (a * fb - b * fa) / (fb - fa);
        double fc = evaluate(e, c);
        if (fa * fc < 0) {
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
    if (strncmp(input, "sin", 3) == 0)
    {
        input += 3;
        EVALABLE *arg;
        input = parseInsideParantheses(input, &arg);
        *e = (EVALABLE *)createTrigonometric(SIN, NULL, arg);
    }
    else if (strncmp(input, "cos", 3) == 0)
    {
        input += 3;
        EVALABLE *arg;
        input = parseInsideParantheses(input, &arg);
        *e = (EVALABLE *)createTrigonometric(COS, NULL, arg);
    }
    else if (strncmp(input, "tan", 3) == 0)
    {
        input += 3;
        EVALABLE *arg;
        input = parseInsideParantheses(input, &arg);
        *e = (EVALABLE *)createTrigonometric(TAN, NULL, arg);
    }
    else if (strncmp(input, "csc", 3) == 0)
    {
        input += 3;
        EVALABLE *arg;
        input = parseInsideParantheses(input, &arg);
        *e = (EVALABLE *)createTrigonometric(CSC, NULL, arg);
    }
    else if (strncmp(input, "sec", 3) == 0)
    {
        input += 3;
        EVALABLE *arg;
        input = parseInsideParantheses(input, &arg);
        *e = (EVALABLE *)createTrigonometric(SEC, NULL, arg);
    }
    else if (strncmp(input, "cot", 3) == 0)
    {
        input += 3;
        EVALABLE *arg;
        input = parseInsideParantheses(input, &arg);
        *e = (EVALABLE *)createTrigonometric(COT, NULL, arg);
    }
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
    *e = (EVALABLE *)createLogarithm(NULL, base, value);
    return input;
}

char *parseExpression(char *input, EVALABLE **e)
{
    Function *f = createFunction();
    double coefficient = 1;
    int sign = 1;
    int state = 0;
    // 0: Start
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
            input = parseLogarithm(input, e);
            // add coeeficient to e 
            if (sign == -1)
            {
                coefficient *= -1;
            }
            ((Logarithm *)*e)->cofactor = (EVALABLE *)createConstant(coefficient);
            addFunctionArg(f, *e);
        }
        if (strncmp(input, "sin", 3) == 0 || strncmp(input, "cos", 3) == 0 || strncmp(input, "tan", 3) == 0 || strncmp(input, "csc", 3) == 0 || strncmp(input, "sec", 3) == 0 || strncmp(input, "cot", 3) == 0)
        {
            input = parseTrigonometric(input, e);
            addFunctionArg(f, *e);
        }
        if (input[0] == '(')
        {
            EVALABLE *insideParantheses;
            input = parseInsideParantheses(input, &insideParantheses);
            addFunctionArg(f, insideParantheses);
            if (input[0] != '*')
            {
                if (state != 0)
                {
                    if (sign == -1)
                    {
                        coefficient *= -1;
                    }
                    addFunctionArg(f, (EVALABLE *)createConstant(coefficient));
                    coefficient = 1;
                }
            }
            else if (input[0] == '^')
            {
                input++;
                EVALABLE *exponent;
                input = parseInsideParantheses(input, &exponent);
                addFunctionArg(f, (EVALABLE *)createExponential((EVALABLE *)createConstant(coefficient), insideParantheses, exponent));
                coefficient = 1;
            }
            else
            {
                input++;
            }
            state = 0;
        }
        if (input[0] >= '0' && input[0] <= '9')
        {
            coefficient = 0;
            state = 1;
            while (input[0] >= '0' && input[0] <= '9')
            {
                coefficient = coefficient * 10 + (input[0] - '0');
                input++;
            }
            if (input[0] == '.')
            {
                input++;
                while (input[0] >= '0' && input[0] <= '9')
                {
                    coefficient = coefficient + (double)(input[0] - '0') / 10;
                    input++;
                }
            }
            printf("Coefficient: %Lf\n", coefficient);
            if (input[0] != '*')
            {
                if (sign == -1)
                {
                    coefficient *= -1;
                }
                addFunctionArg(f, (EVALABLE *)createConstant(coefficient));
                coefficient = 1;
            }
            else
            {
                input++;
            }
        }
        if (input[0] == 'x')
        {
            state = 2;
            input++;
            if (input[0] == '^')
            {
                input++;
                EVALABLE *exponent;
                input = parseInsideParantheses(input, &exponent);
                if (sign == -1)
                {
                    coefficient *= -1;
                }
                addFunctionArg(f, (EVALABLE *)createExponential((EVALABLE *)createConstant(coefficient), (EVALABLE *)createVariable(NULL), exponent));
                coefficient = 1;
            }
            else
            {
                if (sign == -1)
                {
                    coefficient *= -1;
                }
                addFunctionArg(f, (EVALABLE *)createVariable((EVALABLE *)createConstant(coefficient)));
                coefficient = 1;
            }
        }
        if (input[0] == 'e')
        {
            input++;
            if (input[0] == '^')
            {
                input++;
                EVALABLE *exponent;
                input = parseInsideParantheses(input, &exponent);
                if (sign == -1)
                {
                    coefficient *= -1;
                }
                addFunctionArg(f, (EVALABLE *)createExponential((EVALABLE *)createConstant(coefficient), (EVALABLE *)createConstant(M_E), exponent));
                coefficient = 1;
            }
            else
            {
                if (sign == -1)
                {
                    coefficient *= -1;
                }
                addFunctionArg(f, (EVALABLE *)createConstant(coefficient * M_E));
                coefficient = 1;
            }
        }
        if (input[0] == '+')
        {
            input++;
            continue;
        }
        if (input[0] == '-')
        {
            sign = -1;
            input++;
            continue;
        }
    }
    *e = (EVALABLE *)f;
    return input;
}

int main()
{
    EVALABLE *f;
    parseExpression("2*log_(10)(sin(5*x))-x", &f);
    double result = evaluate(f, 9);
    print(f);
    printf("\n");
    printf("Result: %Lf\n", result);

    printf("-----------------\n");
    parseExpression("log_(x)(sin(5*x^(2)+sin(5*x)))", &f);
    print(f);
    printf("\n");
    result = evaluate(f, 3);
    printf("Result: %Lf\n", result);

    printf("-----------------\n");
    parseExpression("x^(e^(5*x))", &f);
    print(f);
    printf("\n");
    result = evaluate(f, 3);
    printf("Result: %Lf\n", result);

    printf("-----------------\n");
    parseExpression("x^(sin(log_(5)(x^(3))))", &f);
    print(f);
    printf("\n");
    result = evaluate(f, 3);
    printf("Result: %Lf\n", result);

    return 0;
}

