# Sayısal Analiz

## BLM1022 - Sayısal Analiz Dönem Projesi - 2024

Yazar: [Ensar Gök](https://www.linkedin.com/in/ensargok/)

Github: [NumericalAnalysis](https://github.com/ensargx/NumericalAnalysis)

## Özellikler

### Fonksiyon Optimizasyonu
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


### Hatalı Syntax Geribildirimi
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

