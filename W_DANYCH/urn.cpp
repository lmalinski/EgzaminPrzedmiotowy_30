#include "urn.h"
#include <QDebug>

// KONSTRUKTORY i SETUP:

Urn::Urn()
{
    setLiczElem(10);
    ustaw();
}

Urn::Urn(int liczbaElementow)
{
    setLiczElem(liczbaElementow);
    ustaw();
}

void Urn::ustaw()
{
    std::random_device rseed;
    m_generator.seed((rseed()));
    m_zakresLos = m_liczbaElementow-1;
}

// KLUCZOWE USŁUGI ZEWNETRZNE:

QVector<int> Urn::losuj(int numPyt)
// Zwykle losowanie zadanej liczby indeksów z pelnej puli, tak aby się nie powtórzyły
{
    QVector<int> numery(numPyt);
    // Losowanie bez zwracania (tylko w obrebie pojedynczego wywolania metody):
    for(int i=0;i<numPyt;i++)
    {
        std::uniform_int_distribution<int> zakres(0, m_zakresLos);
        int numer = zakres(m_generator);
        numery[i] = m_elementy[numer];
        std::swap(m_elementy[numer],m_elementy[m_zakresLos--]);
    }
    // Reset losowania - przywrócenie wylosowanych indkesów do puli.
    m_zakresLos = m_liczbaElementow-1;
    return numery;
}

QVector<int> Urn::losujRedukcjaPowt(int numPyt)
// Zaawansowane losowanie zadanej liczby pytan z puli, tak aby się nie powtórzyły.
// To losowanie modyfikuje rozkład tak aby wylosowane pytania  miały obnizone pradopodbieństwo
// wylosowania w kolejnych losowaniach.
{
    // Obowiazkowa noramalizajca liczebnosci tak aby suma liczbnosci byla rowna liczbie elementów w puli
    // Jest to konieczne, gdyż po losowaniu liczbenosci wylosowania sie zwiększają co spowoduje
    // ze suma prawdopodbieństw w rozkałdzie pzrekorczy 1.0.
    normalizujLiczebnosci();
    QVector<int> numery(numPyt);

    // Ponizsza petla wykonuje losowanie bez zwracania zadanel liczby pytan:
    for(int idx=0; idx<numPyt; idx++)
    {
        // W kazdej iteracji budowany jest nowy rozkład prawdopodobieństwa, gdyż zbiór się modyfikuje
        // w skutek losowania bez zwracania. Jest to najprostrze rozwiązanie choć nie jest to implmentacja
        // optymalna pod względem liczby wykonywanych opreacji. Ponieważ w tym programie takie losowanie
        // trwa ułąmek sekundy i jest powtarzane raz na kilkadziesiąt minut, wydaność nie jest tu kluczowa
        // W tych okolicznosciach prostrze rozwiazanie jest lepsze gdyż, jest mniej zawodne.
        generujRozklad(); // Buduje rozkłąd prawdopodobienstwa na podstawie liczebnści (znormalizowanych)
        generujDytsr(); // Na podstwie rozkładu buduje dystrybuantę

        // Tu jest algorytm losowania z dystrybuanty, który pozwala na wylowanie elementu z rozkaładu
        // który nie jest rónwomierny:
        int numer = losujZDystr();
        Q_ASSERT_X( 0 <= numer && numer <= m_zakresLos,"Urna",
                    (QString("Z dytsrybuanty wylosowano nieprawidowy indeks: ") + QString::number(numer)).toLatin1().data());
        numery[idx] = m_elementy[numer];

        // Poniższa modyfikacja liczebnosci *2 sprawia, że w kolejnym loswaniu szansa na ponowne wylosowanie
        // tego indeksu bedzie dwukrotnie mniejsza niż obecnie. Dzieki normalnizacji, pozniejsze losowanie
        // innych indeksów, bedzie obnizac ich parwdopodobienstwo, jednoczensie zwiększajc proawdopodobienstwo
        // wylosowania tego idneksu.
        m_liczLos[numer] *= 2;
        std::swap(m_elementy[numer],m_elementy[m_zakresLos]);
        std::swap(m_liczLos[numer],m_liczLos[m_zakresLos]);
        m_zakresLos--;
        // Tu jest wazne, że w obrebie jednego wykonannia tej metody wylosowane pytania są eliminowane
        // z rozkłądu prawdopodbienstwa aby się nie powtorzyły.
    }
    // Reset losowania - powoduje że wylosowane w tym wywołąniu indkesy wracają do puli.
    m_zakresLos = m_liczbaElementow-1;
    return numery;
}

void Urn::setLiczElem(int liczbaElementow)
{
    if(liczbaElementow != m_liczbaElementow)
    {
        // Wektory wymagajace zmiany rozmiaru i reinicjalizacji:
        m_elementy.clear();
        m_elementy.resize(liczbaElementow);
        m_liczLos.clear();
        m_liczLos.resize(liczbaElementow);

        // Wektory wymagajacące tylko zmiany rozmiaru (nadpisywane przy użyciu)
        m_rozklad.resize(liczbaElementow);
        m_dystr.resize(liczbaElementow);

        // Reset tresci wektorów:
        for(int idx=0;idx<liczbaElementow;idx++)
        {
            m_elementy[idx] = idx;
            m_liczLos[idx] = 1.0f;
        }

        // Aktualziacja pol sterujacych losowaniem:
        m_liczbaElementow = liczbaElementow;
        m_zakresLos = m_liczbaElementow-1;
    }
}

// ALGORYTMY WEWNĘTRZNE NA POTZREBY LOSOWANIA ZAAWANSOWANEGO:

void Urn::generujRozklad()
{
    // UWAGA liczenie rozkładu zawsze odbywa się w aktualnym zakresie losowania
    // (po ewnetulanym wykluczeniu wylowanych indeksów w skutek losowania bez zwracania):

    //Reset rozkałdu prawdopodobieństwa:
    m_rozklad.fill(0.0f);
    int n = m_zakresLos + 1;

    // Liczymy odwrotnosci liczebnosci (używajac programowania funkcyjnego):
    std::transform(m_liczLos.begin(),m_liczLos.begin()+ n,
                   m_rozklad.begin(),[](float arg){return 1.0f/arg;});

    // Normalizacja rozkłądu liczebnosci. Jest ona tu potzrebna, gdyż przed losowaniem bez zwracania
    // suma parwdopodobienst powinna być rónwa 1.0 (normalizacja zewnęrzna pzred losowaniem).
    // Niestet w skutek eliminacji indeksow i modyfikacji liczbenosci suma ta sie zmienia.
    // Na potrzeby działania algorytmu losowania z dystrybuanty chcemy miec zawsze sumę równa 1.0.

    // W tym celu sumujemy pradwopodobienstwa (suma powinna być różna od 1.0):
    float suma = std::accumulate(m_rozklad.begin(),m_rozklad.begin()+n,0.0f);

    // Dzielimy kazdy element przez sumę - czyli nurmalizujemy do sumy 1.0 uzyskujac prawidłowy rozkład.
    std::transform(m_rozklad.begin(),m_rozklad.begin()+ n,
                   m_rozklad.begin(),[suma](float arg){return arg/suma;});

}

void Urn::generujDytsr()
{
    // Reset dystrybuanty:
    m_dystr.fill(0.0f);
    int n = m_zakresLos + 1;

    // Wyznaczenie dystrybuanty, czyli skumulowanego rozkałdu proawdopodobienstwa.
    // Uzwyamy tu mutowalnej lambdy, która w kazdy element dystrybuanty wstawia
    // sumę birzącą rozkałdu. Osattni elemnt powinien być równy 1.0.
    std::transform(m_rozklad.begin(),m_rozklad.begin()+ n, m_dystr.begin(),
                   [sum = 0.0f](float arg) mutable {return sum += arg;} ); // Tu analizator skąłdni się gubi, ale jest to OK!
}

void Urn::normalizujLiczebnosci()
{
    // Normalizacja liczebności, która musi nastąpić po każdym losowaniu aby dostosować
    // prawdopodobienstwa wylowania idneksów do sumy równej liczebnosci puli. To zapewnia
    // że w kolejnych losowaniach liczebności nie akumulują się do nieskończonosci, eliminujac
    // indeksy i destabilizując algorytm numerycznie.
    // Ta naormalizacja wykonywana jest po pełnym losowaniu kiedy indkesy wylosowane, są
    // przywarcone do puli (maja zmodyfikowane liczebnosci i to zaburza rozkład).

    // Liczenie aktualnej sumy liczebnosci:
    float suma = std::accumulate(m_liczLos.begin(),m_liczLos.end(),0.0f);

    // Normalizacja liczebności tak aby ich suma była równa liczbie elemnetów w puli.
    std::transform(m_liczLos.begin(),m_liczLos.end(),m_liczLos.begin(),
                   [this,suma](float arg){return arg*m_liczbaElementow/suma; });
}

int Urn::losujZDystr()
{
    // Wartosci dsytrybuanty celowo nie są rozłozone równomiernie w przedziale <0,0, 1,0>
    // co zapewnia loswoanie z różnym pradopodobieństwemy dla poszczególnych elementów
    // patrz też Algorytm Losowania Koła Fortuy).

    // Wylosuj licbę rzeczywistą z pzredziału <0.0, 1,0>
    std::uniform_real_distribution<float> zakres(0.0f,1.0f);
    float val = zakres(m_generator);

    // Znadzć pierwszą wartość dystrybuanty wiekszą od wylosowanej wartości
    // i zwróc jej indeks (ten indeks to wylowany indeks z puli):
    for(int idx=0;idx<m_zakresLos+1;idx++)
    {
        if( val <= m_dystr[idx])
            return idx;
    }

    // Ta lnijka tylko zapewnia, że coś napewno zostanie zwrócone (-1 to sygnalizacja błędu)
    // Tw wartość nigdy nie powinna systąpić, jesli algorytm pracuje na prawidłowych danych.
    return -1;
}


