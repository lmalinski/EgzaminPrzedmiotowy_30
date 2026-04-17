#include "bazapytan.h"
#include "W_DANYCH\importerBlokowy.h"

BazaPytan::BazaPytan(QString dir)
    :m_importer(new ImporterBlokowy(dir))
{}

void BazaPytan::wczytajDane()
{
    m_wiedzaBlok = m_importer->wczytajDane(TypPytania::Wiedza);
    m_zrozumienieBlok = m_importer->wczytajDane(TypPytania::Zrozumienie);
    m_liczbaBlokow = m_wiedzaBlok.size();
    m_urnyDoLosowania[(int)TypPytania::Wiedza].resize(m_liczbaBlokow);
    m_urnyDoLosowania[(int)TypPytania::Zrozumienie].resize(m_liczbaBlokow);
    for (int blok = 0; blok < m_liczbaBlokow; blok++)
    {
        m_urnyDoLosowania[(int)TypPytania::Wiedza][blok].setLiczElem(m_wiedzaBlok[blok].size());
        m_urnyDoLosowania[(int)TypPytania::Zrozumienie][blok].setLiczElem(m_zrozumienieBlok[blok].size());
    }
}

QVector<Pytanie> BazaPytan::losujPytania(int blok, int addNumWiedza, int addNumZroz)
{
    Q_ASSERT_X(m_urnyDoLosowania[(int)TypPytania::Wiedza][blok].maElementy(),"BazaPytan/losujPytania","Niezainicjalizowana urna dla wiedzy");
    Q_ASSERT_X(m_urnyDoLosowania[(int)TypPytania::Zrozumienie][blok].maElementy(),"BazaPytan/losujPytania","Niezainicjalizowana urna dla zrozumienia");

    const int MIN_LICZ_WIEDZA = 3;
    const int MIN_LICZ_ZROZ = 6;
    // Wyznaczenie liczby pytań do losowania i przygotowanie vectora buforującego wylosowane pytania (OBA TYPY zbiorczo):
    int liczWiedza = MIN_LICZ_WIEDZA + addNumWiedza;
    int liczZrozum = MIN_LICZ_ZROZ + addNumZroz;
    QVector<Pytanie> wylosPyt(liczWiedza + liczZrozum);

    // Losowanie indeksów pytań z redukcją powtórzeń (patrz urna.cpp).
    // Istotne jest to, że urny losuja tylko indeksy pytań:
    QVector<int> numeryWiedza = m_urnyDoLosowania[(int)TypPytania::Wiedza][blok].losujRedukcjaPowt(liczWiedza);
    QVector<int> numeryZrozum = m_urnyDoLosowania[(int)TypPytania::Zrozumienie][blok].losujRedukcjaPowt(liczZrozum);

    // Pobranie pytań z bazy zgodnie z wylosowanymi indeksami:
    for(int pyt = 0; pyt < liczWiedza; pyt++)
        wylosPyt[pyt] = m_wiedzaBlok[blok].at(numeryWiedza[pyt]);
    for(int pyt = liczWiedza; pyt < liczWiedza+liczZrozum; pyt++)
        wylosPyt[pyt] = m_zrozumienieBlok[blok].at(numeryZrozum[pyt-liczWiedza]);
    return wylosPyt;
}
