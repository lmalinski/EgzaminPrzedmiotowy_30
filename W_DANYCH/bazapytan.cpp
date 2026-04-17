#include "bazapytan.h"
#include "W_DANYCH\importerBlokowy.h"

BazaPytan::BazaPytan(QString dir)
    :m_importer(new ImporterBlokowy(dir))
{}

void BazaPytan::wczytajDane()
{
    m_bloki = m_importer->wczytajDane();
    m_liczbaBlokow = m_bloki.size();
    m_urnyDoLosowania.resize(m_liczbaBlokow);
    for (int blok = 0; blok < m_liczbaBlokow; blok++)
        m_urnyDoLosowania[blok].setLiczElem(m_bloki[blok].size());
}

QVector<Pytanie> BazaPytan::losujPytania(int blok, int addNum)
{
    Q_ASSERT_X(m_urnyDoLosowania[blok].maElementy(),"BazaPytan/losujPytania","Niezainicjalizowana urna");

    // Wyznaczenie liczby pytań do losowania i przygotowanie vectora buforującego wylosowane pytania:
    int toLos = MIN_LICZ_PYT;// + numNumWiedza;
    QVector<Pytanie> wylosPyt(toLos);

    // Losowanie indeksów pytań z redukcją powtórzeń (patrz urna.cpp).
    // Istotne jest to, że urny losuja tylko indeksy pytań:
    QVector<int> numery = m_urnyDoLosowania[blok].losujRedukcjaPowt(toLos);

    // Pobranie pytań z bazy zgodnie z wylosowanymi indeksami:
    for(int pyt = 0; pyt < toLos; pyt++)
        wylosPyt[pyt] = m_bloki[blok].at(numery[pyt]);

    return wylosPyt;
}
