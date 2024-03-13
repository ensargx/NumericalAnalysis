/*
 * analysis.c
 * Author: Ensar Gök
 *
 * Numerical analysis functions
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define EVALABLE void*

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
    EvalAble *args[256];
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

Constant *createConstant(double value);
void destroyConstant(Constant *c);
double evaluateConstant(Constant *c, double value);

Exponential *createExponential(EVALABLE *cofactor, EVALABLE *base, EVALABLE *exponent);
void destroyExponential(Exponential *e);
double evaluateExponential(Exponential *e, double value);

Logarithm *createLogarithm(EVALABLE *cofactor, EVALABLE *base, EVALABLE *exponent);
void destroyLogarithm(Logarithm *l);
double evaluateLogarithm(Logarithm *l, double value);

Function *createFunction(int argCount);
void addFunctionArg(Function *f, EvalAble *argType);
void destroyFunction(Function *f);
double evaluateFunction(Function *f, double value);

Trigonometric *createTrigonometric(TrigonometricType type, EVALABLE *cofactor, EVALABLE *arg);
void destroyTrigonometric(Trigonometric *t);
double evaluateTrigonometric(Trigonometric *t, double value);

InverseTrigonometric *createInverseTrigonometric(InverseTrigonometricType type, EVALABLE *cofactor, EVALABLE *arg);
void destroyInverseTrigonometric(InverseTrigonometric *it);
double evaluateInverseTrigonometric(InverseTrigonometric *it, double value);

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
    free(v);
}

double evaluateVariable(Variable *v, double value) {
    return evaluate(v->cofactor, value) * value;
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
    free(l);
}

double evaluateLogarithm(Logarithm *l, double value) {
    return evaluate(l->cofactor, value) * log(evaluate(l->base, value)) / log(evaluate(l->exponent, value));
}

// Function functions

Function *createFunction(int argCount) {
    Function *f = (Function *)malloc(sizeof(Function));
    f->type = FUNCTION;
    f->argCount = argCount;
    return f;
}

void addFunctionArg(Function *f, EvalAble *arg) {
    f->args[f->argCount++] = arg;
}

void destroyFunction(Function *f) {
    free(f);
}

double evaluateFunction(Function *f, double value) {
    double result = 0;
    for (int i = 0; i < f->argCount; i++) {
        result += evaluate((EVALABLE *)f->args[i], value);
    }
    return result;
}

// Main evaluation function
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

int main() {
    // x^(sin(log_5 (x^3))
    Variable *x = createVariable(NULL);
    Constant *c1 = createConstant(5);
    Exponential *x3 = createExponential(NULL, (EVALABLE *)x, (EVALABLE *)createConstant(3));
    Logarithm *l = createLogarithm(NULL, (EVALABLE *)c1, (EVALABLE *)x3);
    Trigonometric *s = createTrigonometric(SIN, NULL, (EVALABLE *)l);
    Exponential *e = createExponential(NULL, (EVALABLE *)x, (EVALABLE *)s);
    printf("%f\n", evaluate((EVALABLE *)e, 7));
    return 0;
}



