# Sayısal Analiz

## BLM1022 - Sayısal Analiz Dönem Projesi - 2024

Yazar: [Ensar Gök](https://www.linkedin.com/in/ensargok/) - 22011032

GitHub: [NumericalAnalysis](https://github.com/ensargx/NumericalAnalysis)

# İçindekiler 
 - [Özellikler](#özellikler)
 - Ana Menü
 - Desteklenen Fonksiyonlar
   - Polinom
   - Üstel
   - Logaritmik
   - Trigonometrik
   - Ters Trigonometrik
   - Örnekler
- Matris Girişi
  - Örnek
- Bisection
  - Parametreler
  - Örnek
- Regula-Falsi
  - Parametreler
  - Örnek
- Newton-Raphson
  - Parametreler
  - Örnek
- NxN'lik Bir Matrisin Tersi
  - Parametreler
  - Örnek
- Gauss Eliminasyon Yöntemi
  - Parametreler
  - Örnek
- Gauss-Seidel Yöntemi
  - Parametreler
  - Örnek
- Sayısal Türev
  - Parametreler
  - Örnek
- Simpson Yöntemi
  - Parametreler
  - Örnek
- Trapez Yöntemi
  - Parametreler
  - Örnek
- Değişken Dönüşümsüz Gregory-Newton Enterpolasyonu
  - Parametreler
  - Örnek



# Özellikler

## Fonksiyon Optimizasyonu
Girdiğiniz fonksiyonda x değişkenine bağlı olmayan değerlerin algoritmaya girmeden önce hesaplanıp kullanılması. 
Bu sayede gereksiz işlemler tekrar tekrar yapılmayacak, sadece x değişkenine bağlı değerler hesaplanıp, program daha optimize sonuçlar verecek.

Örnek:
```bash
Enter your function: sin(pi/6)+ln(e*e)
[Optimized] f(x) = 2.500000

Enter your function: ln(sin(4pi/3)+cos(0)+e)+x
[Optimized] f(x) = (x+1.048110)

Enter your function: (ln(sin(4pi/3)+cos(0)+e) + x) * (asin(1)*3+e)
[Optimized] f(x) = (x+1.048110)*7.430671
```

## Hatalı Syntax Geribildirimi
Syntax hatası yaptığınızda nerede hata yapıtğınızı ve ne hatası yaptığınızı belirten geribildirim sistemi.

Geribildirim ile alacağınız dönütler:
- Parantez Eksikliği: Parantez açmayı veya kapatmayı unuttuğunuzda sizi uyarır.
- sin, cos gibi terimleri yanlış yazarsanız sizi uyarır.
- log fonksiyonu için taban veya üs alırken eksik girdiyi uyarır (_).

Örnek:
```bash
Enter your function: 12x^(2+ln(7)
[Failed to parse]                ^ Expected ')', received ''

Enter your function: 17sinx+ln(pi/4)
[Failed to parse]         ^ Expected '(', received 'x'

Enter your function: 24cos(x*pi/3)-log(5)(x^(2))
[Failed to parse]                     ^ Expected '_', received '('

Enter your function: x^(cos(pi))-sim(x)
[Failed to parse]                ^ Unexpected character.
```

# Algoritma Yapısı

## EVALABLE
Tüm hesaplanabilir fonksiyonlar bir Evalable'dir. en basit constant ör. (5) te bir evalable'dir, en kompleks fonksiyon ör. ((ln(sin(4pi/3)+cos(0)+e) + x) * (asin(1)*3+e)) de evalable'dir. 
Evalable'ler `EvalAbleType`'lara göre ayrılırlar. Bu Type'lar şunlardır:

- Constant
- Variable
- SumChain
- MulChain
- Logarithm
- Exponential
- Trigonometric
- InverseTrigonometric

Evet; toplama, çıkarma, çarpma ve bölme işlemi bir evalable'dir ve diğer evalable'leri array halinde saklar ve gerekli işlemi (çarpma, bölme; toplama, çıkarma) tek tek o evalable'lere uygular. Çarpmanın önceliği de burada sağlanmıştır. 

Evalable'lerin struct yapıları ise şu şekildedir:
### EvalAble
```c
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
```

Genel EvalAble yapısı ise şu şekildedir:
```c
typedef struct _EvalAble {
    EvalAbleType type;
} EvalAble;
```
Tek Başına EvalAble kullanılmaz. EvalAble, genel olarak evaluate edilebilen her şeyi temsil eder.

### Constant
```c
typedef struct _Constant {
    EvalAbleType type;
    ldouble_t value;
} Constant;
```

### Variable
```c
typedef struct _Variable {
    EvalAbleType type;
} Variable;
```

### SumChain
```c
typedef struct _SumChain {
    EvalAbleType type;
    int argCount;
    int maxArgs;
    EVALABLE **args;
    int *isPositive;
} SumChain;
```
SumChain, iki adet re-allocate ile boyutu arttırılabilen array'a sahiptir, bu array'ler toplama veya çarpma işlemi uygulanacak EvalAble'yi ve işlemin toplama veya çıkarma olduğunu (isPositive) gösterir.

### MulChain
```c
typedef struct _MulChain {
    EvalAbleType type;
    int argCount;
    int maxArgs;
    EVALABLE **args;
    int *isDivided;
} MulChain;
```
MulChain'de SumChain gibidir, ancak çarpma ve bölme işlemi için gerekli bilgileri tutar.

### Exponential
```c
typedef struct _Exponential {
    EvalAbleType type;
    EVALABLE *base;
    EVALABLE *exponent;
} Exponential;
```
Üslü işlemlerde taban ve üs birer EvalAble'dir, yani istediğimiz her şeyi tabana veya üse yazabiliriz.

### Trigonometric
```c
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
```
Arg değeri kullanılacak trigonometrik fonksiyonun içine yazılacak fonksiyonu temsil ediyor. sin(arg) şeklinde.

### InverseTrigonometric
```c
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
```

### Logarithm
```c
typedef struct _Logarithm {
    EvalAbleType type;
    EVALABLE *base;
    EVALABLE *value;
} Logarithm;
```

### EvalAble üretme, printleme, hesaplama ve yıkma 

Tüm EvalAble'ler için üretim, print, hesap, yıkım ve kopyalama fonksiyonları vardır, bu fonksiyonların görevleri şu şekildedir.

- `create`: İstenilen EvalAble'yi üretip, gerekli memory allocation'ları gerçekleştirmek.
- `destroy`: Yıkılmak istenen EvalAble'nin memory'de allocate edilen tüm verilerini freeler (Valgrind ile bir çok test yapılmıştır, programda hiçbir memory leak bulunmamaktadır, tamamıyla memory-safe bir programdır.)
- `evaluate`: Hesaplanmak istenen EvalAble'yi verilen `x` değerleri (value) için hesaplar.
- `print`: Kullanıcının görmesi için istenilen EvalAble stdout'a print edilir.
- `copy`: İstenilen EvalAble'nin aynısı kopyalanır ve döndürülür.
```c
/* Exponential prototypes */
Exponential *createExponential(EVALABLE *base, EVALABLE *exponent);
void destroyExponential(Exponential *e);
ldouble_t evaluateExponential(Exponential *e, ldouble_t value);
void printExponential(Exponential *e);
```

```c
/* 
 * Evaluate the expression with the given value
 *
 * Parameters:
 * - e: The expression to be evaluated
 * - value: The value to be used in the expression
 * Returns:
 * - The result of the expression
*/
ldouble_t evaluate(EVALABLE *e, ldouble_t value);

/* 
 * Destroy the expression and free the memory
 *
 * Parameters:
 * - e: The expression to be printed
*/
void destroy(EVALABLE *e);

/* 
 * Print the expression
 *
 * Parameters:
 * - e: The expression to be printed
*/
void print(EVALABLE *e);
```

Genel EvalAble'ler için çağırılan metodlar, EvalAbleType değerine göre alakalı fonksiyona iletilir.

```c
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
```

## Optimizasyon Algoritması
Optimizasyon, Verilen bir EvalAble'yi constant haline getirmeyi hedefler. Eğer bir sin fonksiyonu parametre olarak constant değer alıyorsa direkt hesaplanabilir. Bu sebeple öncelikle DFS yardımıyla en genel fonksiyondan en küçük fonksiyona kadar tüm EvalAble'ler tek tek kontrol edilir. Örn. sin(log_(10)(100)) fonksiyonu için optimizasyon algoritması çağırılırsa öncelikle sin'in arg'ı constant mı diye kontrol edilir, log(100) constant değil, logaritmadır. Bu sebeple bu fonksiyon için tekrardan optimize çağırılır, eğer sonuç constant çıkarsa sinüs fonksiyonu da hesaplanabilir demektir, aksi taktirde hesaplanamaz. log_(10)(100) için optimize fonksiyonu çağırıldığında log'un hem tabanı hem de üsü constanttır, bu yüzden log'un değeri hesaplanır ve log_(10)(100)'ün değeri olan 2 constant bir değer olarak sinüse döner, constant bir değer döndüğü için bu sefer tekrardan sinüs fonksiyonunun değeri hesaplanabilir. 

Örnek:
```bash
Enter your function: sin(log_(10)(100))
[Optimized] f(x) = 0.909297
```

Optimize fonksiyonu, her zaman EvalAble return etmek zorundadır, aynı zamanda input olarak aldığı EvalAble'yi temizlemekle de sorumludur.
```c
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
```

## Parser Algoritması

Parser algoritması aldığı string inputu EvalAble'ye dönüştürür. Bu işlem sırasında string'i manipule eder.
Algoritma her zaman SumChain, MulChain ve bir EvalAble'ye (`arg`) sahiptir, bu okunmuş olan EvalAble'yi tutar.

Bu işlem sırasında string'in ilk değerini okur, eğer sayı ise say bitene kadar o sayıyı okur ve constant üretir.
Ayni şekilde eğer `pi` `e` gibi değerler var ise bu değerleri'de matematikteki karşılığıyla constant olarak tutar.

Eğer '+', '-', '*', '/' değerlerinden biri var ise arg'ı SumChain veya Mulchain'e ekler. 

Eğer `sin`, `log` gibi string'ler var ise gerekli parser fonksiyonu ile o fonksiyonu parse eder.

Ayrıca eğer `arg` değeri doluysa ve yeni bir EvalAble parse edilirse o işlem arasında çarpım olduğu varsayılır. ör. `2sin(x)` `2*sin(x)` olarak değerlendirilir.
```c
/* Parser functions */

/* 
 * Parse the expression from the input string
 *
 * The parser function is responsible for parsing the expression from the input string
 * and creating the necessary EvalAble structs to represent the expression.
 *
 * Parameters:
 * - input: The input string to be parsed
 * - e: The pointer to the EvalAble struct that will be created
 * - s: The status code
 * Returns:
 * - The position of the input string after the expression
*/
char *parseExpression(char *input, EVALABLE **e, StatusCode *s);
char *parseLogarithm(char *input, EVALABLE **e, StatusCode *s);
char *parseInsideParantheses(char *input, EVALABLE **e, StatusCode *s);
char *parseTrigonometric(char *input, EVALABLE **e, StatusCode *s);
char *parseInverseTrigonometric(char *input, EVALABLE **e, StatusCode *s);
```

## Yardımcı Fonksiyonlar / Makrolar

```c
typedef long double ldouble_t;
#define ABS(x) ((x) < 0 ? -(x) : (x))

ldouble_t evaluate(EVALABLE *e, ldouble_t value);
#define EVALTYPE(x) ((EvalAble *)x)->type
#define UNUSED(x) (void)(x)
```

# Sayısal Analiz Yöntemleri (Fonksiyonlar için)

## Bisection
```c
ldouble_t solveBisection(EVALABLE *e, ldouble_t a, ldouble_t b, ldouble_t epsilon)
{
    // TODO: Aralığı tanımsız yapan değer var mı kontrol et.
    ldouble_t fa = evaluate(e, a);
    ldouble_t fb = evaluate(e, b);
    ldouble_t c = 0;
    if (fa * fb > 0)
    {
        return NAN;
    }
    while ((b - a) > epsilon)
    {
        c = (a + b) / 2;
        ldouble_t fc = evaluate(e, c);
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
```
(Todo'nun implementi biraz aşırıya kaçacağı için Banu Hoca'ya sorulup yapılmadı)

## Regula Falsi
```c
ldouble_t solveRegulaFalsi(EVALABLE *e, ldouble_t a, ldouble_t b, ldouble_t epsilon)
{
    ldouble_t fa = evaluate(e, a);
    ldouble_t fb = evaluate(e, b);
    ldouble_t c = 0;
    if (fa * fb > 0)
    {
        return NAN;
    }
    while ((b - a) > epsilon)
    {
        c = (a * fb - b * fa) / (fb - fa);
        ldouble_t fc = evaluate(e, c);
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
```

## Newton Raphson
```c
ldouble_t solveNewtonRaphson(EVALABLE *e, ldouble_t x0, ldouble_t epsilon)
{
    ldouble_t x = x0;
    ldouble_t fx = evaluate(e, x);
    ldouble_t dfx = (evaluate(e, x + epsilon) - fx) / epsilon;
    while (ABS(fx) > epsilon)
    {
        x = x - fx / dfx;
        fx = evaluate(e, x);
        dfx = (evaluate(e, x + epsilon) - fx) / epsilon;
    }
    return x;
}
```

## Trapez 
```c
ldouble_t integrateTrapez(EVALABLE *e, ldouble_t a, ldouble_t b, int n)
{
    ldouble_t h = (b - a) / n;
    ldouble_t sum = 0;
    int i;
    for (i = 1; i < n; i++)
    {
        sum += evaluate(e, a + i * h);
    }
    return h * (evaluate(e, a) + evaluate(e, b) + 2 * sum) / 2;
}
```

## Simpson
```c
ldouble_t integrateSimpson38(EVALABLE *e, ldouble_t a, ldouble_t b, int n)
{
    ldouble_t sum = 0;
    ldouble_t h = (b - a) / n;
    int i;
    for (i = 0; i < n; i++)
    {
        b = a + h;
        sum += (b - a) / 8 * (evaluate(e, a) + 3 * evaluate(e, (a + (b-a)/3)) + 3 * evaluate(e, (a + 2*(b-a)/3)) + evaluate(e, b));
        a = b;
    }
    return sum;
}
```
