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
    EVALABLE *exponent;
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

Logarithm *createLogarithm(EVALABLE *cofactor, EVALABLE *base, EVALABLE *exponent);
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

Logarithm *createLogarithm(EVALABLE *cofactor, EVALABLE *base, EVALABLE *exponent) {
    Logarithm *l = (Logarithm *)malloc(sizeof(Logarithm));
    l->type = LOGARITHM;
    if (cofactor == NULL) {
        cofactor = (EVALABLE *)createConstant(1);
    }
    l->cofactor = cofactor;
    if (base == NULL) {
        base = (EVALABLE *)createConstant(1);
    }
    l->base = base;
    if (exponent == NULL) {
        exponent = (EVALABLE *)createConstant(1);
    }
    l->exponent = exponent;
    return l;
}

void destroyLogarithm(Logarithm *l) {
    destroy(l->cofactor);
    destroy(l->base);
    destroy(l->exponent);
    free(l);
}

double evaluateLogarithm(Logarithm *l, double value) {
    return evaluate(l->cofactor, value) * log(evaluate(l->base, value)) / log(evaluate(l->exponent, value));
}

EVALABLE *deriveLogarithm(Logarithm *l) {
    return NULL;
}

void printLogarithm(Logarithm *l) {
    printf("(");
    print(l->cofactor);
    printf(")log_");
    print(l->exponent);
    printf("(");
    print(l->base);
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

int main() {
    // (x-5)(x-10) -> x^2 - 15x + 50
    // roots: 5, 10
    // a = 3, b = 7, epsilon = 0.0001
    Function *f = createFunction();
    Exponential *x2 = createExponential(NULL, (EVALABLE *)createVariable(NULL), (EVALABLE *)createConstant(2));
    Variable *x15 = createVariable((EVALABLE *)createConstant(-15));
    Constant *x50 = createConstant(50);

    addFunctionArg(f, (EVALABLE *)x2);
    addFunctionArg(f, (EVALABLE *)x15);
    addFunctionArg(f, (EVALABLE *)x50);

    printf("f(x) = ");
    print((EVALABLE *)f);
    printf("\n");

    EVALABLE *df = derive((EVALABLE *)f);
    printf("f'(x) = ");
    print(df);
    printf("\n");


    // Get string input from user and parse it to create a function
    
    printf("Enter a function: ");
    // char input[256];
    // scanf("%s", input);
    char input[] = "x^(sin(log_5 (x3))";
    printf("You entered: %s\n", input);


    int idx = 0;
    char d[] = "+-*/^_()";
    char nextDelim = input[strcspn(input, d)];
    char *token = strtok(input, d);
    while (token != NULL) {
        printf("Token: %s\n", token);
        printf("Next Delim: %c\n", nextDelim);
        nextDelim = input[strcspn(input, d)];
        token = strtok(NULL, d);
    }


    return 0;
}

