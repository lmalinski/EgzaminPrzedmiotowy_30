#include "program.h"
#include <QDebug>
//#include "importerJednostkowy.h"
#include "importerblokowy.h"

Program::Program(QObject *parent)
    : QObject{parent},m_BazaPytan(new ImporterBlokowy(this,"PytaniaNEW"))
{}

void Program::setPrzedmiot(QString nazwa)
{
    m_BazaPytan->setPrzedmiot(nazwa);
}

void Program::loadPytania()
{
    m_BazaPytan->readData();
    for (int blok = 0; blok < m_BazaPytan->getNumBlok(); blok++)
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
        QVector<int> numery = m_bloki[blok].losuj(toLos);
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
