#include "W_USLUG\program.h"
#include "W_DANYCH\importerBlokowy.h"
#include <QDebug>

// KONSTRUKTORY I SETUP:

Program::Program(QObject *parent)
    : QObject{parent},m_BazaPytan(new ImporterBlokowy(this,"PytaniaNEW"))
{}

// METODY - USŁUGI:

void Program::setPrzedmiot(QString nazwa)
{
    m_BazaPytan->setPrzedmiot(nazwa);
}

void Program::loadPytania()
{   
    m_BazaPytan->readData();
    int numBlok = m_BazaPytan->getNumBlok();
    for (int blok = 0; blok < numBlok; blok++)
        m_bloki[blok].setNum(m_BazaPytan->getBlok(blok).size());
}

int Program::getLiczbaBlokow()
{
    return m_BazaPytan->getNumBlok();
}

void Program::generujPytania(int blok, int num)
{
    if(m_bloki[blok].maElementy())
    {
        int toLos = MIN_LICZ_PYT + num;
        m_WylosPyt[blok].clear();
        m_WylosPyt[blok].resize(toLos);
        QVector<int> numery = m_bloki[blok].losujRedukcjaPowt(toLos);
        for(int pyt = 0; pyt < toLos; pyt++)
            m_WylosPyt[blok][pyt] = m_BazaPytan->getBlok(blok).at(numery[pyt]);

        emit wypisz(m_WylosPyt[blok], blok);
    }
}

QString Program::getAktPrzedmiot()
{
    return m_BazaPytan->getPrzedmiot();
}

QStringList Program::getListaPrzedmiotow()
{
    return m_BazaPytan->getPrzedmiotList();
}

void Program::odznaczPytania(QVector<int> pytDoOdznaczenia, int blok)
{
    std::sort(pytDoOdznaczenia.rbegin(), pytDoOdznaczenia.rend());
    for(auto idx:pytDoOdznaczenia)
        m_WylosPyt[blok].erase(m_WylosPyt[blok].begin()+idx);
    emit wypisz(m_WylosPyt[blok], blok);
}

void Program::zwrocAktWylosowane(int blok)
{
    assert(blok < MAX_LICZ_BLOKOW);
    if(m_WylosPyt[blok].size() == MIN_LICZ_PYT)
        emit wypisz(m_WylosPyt[blok], blok);
}
