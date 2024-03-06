/*
 * analysis.c
 * Author: Ensar Gök
 *
 * Numerical analysis functions
 */

#include <stdio.h>
#include <math.h>

typedef enum _TermType {
    POLYNOMIAL,
    TRIGONOMETRIC,
    LOGARITHMIC,
} TermType;

typedef struct _PolynomialTerm {
    double coefficient;
    int exponent;
    // evaluate function
} PolynomialTerm;

double evaluatePolynomial(double x, PolynomialTerm *term) {
    return term->coefficient * pow(x, term->exponent);
}

PolynomialTerm derivePolynomial(PolynomialTerm *term) {
    PolynomialTerm derived;
    derived.coefficient = term->coefficient * term->exponent;
    derived.exponent = term->exponent - 1;
    return derived;
}

typedef enum _TrigonometricFunction {
    SIN,
    COS,
    TAN,
    COT,
    SEC,
    CSC,
} TrigonometricFunction;

typedef struct _TrigonometricTerm {
    double coefficient;
    TrigonometricFunction function;
    double period;
    double phase;
} TrigonometricTerm;

double evaluateTrigonometric(double x, TrigonometricTerm *term) {
    double value = 0;
    switch (term->function) {
        case SIN:
            value = sin(x);
            break;
        case COS:
            value = cos(x);
            break;
        case TAN:
            value = tan(x);
            break;
        case COT:
            value = 1 / tan(x);
            break;
        case SEC:
            value = 1 / cos(x);
            break;
        case CSC:
            value = 1 / sin(x);
            break;
    }
    return term->coefficient * value;
}

TrigonometricTerm deriveTrigonometric(TrigonometricTerm *term) {
    TrigonometricTerm derived;
    derived.coefficient = term->coefficient;
    switch (term->function) {
        case SIN:
            derived.function = COS;
            break;
        case COS:
            derived.function = SIN;
            break;
        case TAN:
            derived.function = SEC;
            break;
        case COT:
            derived.function = -CSC;
            break;
        case SEC:
            derived.function = TAN;
            break;
        case CSC:
            derived.function = -COT;
            break;
    }
    derived.period = term->period;
    derived.phase = term->phase + M_PI_2;
    return derived;
}

typedef struct _LogarithmicTerm {
    double coefficient;
    double base;
} LogarithmicTerm;

double evaluateLogarithmic(double x, LogarithmicTerm *term) {
    return term->coefficient * log(x) / log(term->base);
}

LogarithmicTerm deriveLogarithmic(LogarithmicTerm *term) {
    LogarithmicTerm derived;
    derived.coefficient = term->coefficient / log(term->base);
    derived.base = term->base;
    return derived;
}

typedef struct _Term {
    TermType type;
    union {
        PolynomialTerm polynomial;
        TrigonometricTerm trigonometric;
        LogarithmicTerm logarithmic;
    };
} Term;

double evaluateTerm(double x, Term *term) {
    double value = 0;
    switch (term->type) {
        case POLYNOMIAL:
            value = evaluatePolynomial(x, &term->polynomial);
            break;
        case TRIGONOMETRIC:
            value = evaluateTrigonometric(x, &term->trigonometric);
            break;
        case LOGARITHMIC:
            value = evaluateLogarithmic(x, &term->logarithmic);
            break;
    }
    return value;
}

Term deriveTerm(Term *term) {
    Term derived;
    derived.type = term->type;
    switch (term->type) {
        case POLYNOMIAL:
            derived.polynomial = derivePolynomial(&term->polynomial);
            break;
        case TRIGONOMETRIC:
            derived.trigonometric = deriveTrigonometric(&term->trigonometric);
            break;
        case LOGARITHMIC:
            derived.logarithmic = deriveLogarithmic(&term->logarithmic);
            break;
    }
    return derived;
}

typedef struct _Function {
    Term *terms;
    int termCount;
} Function;

double evaluateFunction(double x, Function *function) {
    double value = 0;
    for (int i = 0; i < function->termCount; i++) {
        value += evaluateTerm(x, &function->terms[i]);
    }
    return value;
}

void deriveFunction(Function *function, Function *derived) {
    derived->termCount = function->termCount;
    for (int i = 0; i < function->termCount; i++) {
        derived->terms[i] = deriveTerm(&function->terms[i]);
    }
}

int main() 
{
    // f(x) = 2x^3 + 3x^2 + 5
    // f'(x) = 6x^2 + 6x
    PolynomialTerm term1 = {2, 3};
    PolynomialTerm term2 = {3, 2};
    PolynomialTerm term3 = {5, 0};

    Term terms[3];
    terms[0].type = POLYNOMIAL;
    terms[0].polynomial = term1;
    terms[1].type = POLYNOMIAL;
    terms[1].polynomial = term2;
    terms[2].type = POLYNOMIAL;
    terms[2].polynomial = term3;

    Function function;
    function.termCount = 3;
    function.terms = terms;

    printf("f(2) = %f\n", evaluateFunction(2, &function));

    Function derived;
    derived.termCount = 3;
    derived.terms = terms;
    deriveFunction(&function, &derived);

    printf("f'(2) = %f\n", evaluateFunction(2, &derived));

    // f(x) = sin(x) + 7x^2 + 3.2
    Term terms2[3];
    terms2[0].type = TRIGONOMETRIC;
    terms2[0].trigonometric.coefficient = 1;
    terms2[0].trigonometric.function = SIN;
    terms2[0].trigonometric.period = 2 * M_PI;
    terms2[0].trigonometric.phase = 0;
    terms2[1].type = POLYNOMIAL;
    terms2[1].polynomial.coefficient = 7;
    terms2[1].polynomial.exponent = 2;
    terms2[2].type = POLYNOMIAL;
    terms2[2].polynomial.coefficient = 3.2;
    terms2[2].polynomial.exponent = 0;

    Function function2;
    function2.termCount = 3;
    function2.terms = terms2;

    double pi = M_PI;
    printf("f(%f) = %f\n", pi, evaluateFunction(pi, &function2));
    // expected: 7 * pi^2 + 3.2 = 7 * 9.8696 + 3.2 = 69.286 + 3.2 = 72.486 

    return 0;
}
