#include "urn.h"
#include <QDebug>

Urn::Urn()
{
    setNum(10);
    setup();

}

Urn::Urn(int liczbaElementow)
{
    setNum(liczbaElementow);
    setup();
}

void Urn::setup()
{
    //const int BUFER_SIZE = 3;
    std::random_device rseed;
    m_generator.seed((rseed()));
    m_zakresLos = m_liczbaElementow-1;
}

void Urn::losujInit()
{
    std::uniform_int_distribution<int> zakres(0, m_zakresLos--);
    int numer = zakres(m_generator);
    m_bufor.push_front(m_elementy[numer]);
    m_elementy.removeAt(numer);
}

QVector<int> Urn::losuj(int numPyt)
{
    /*std::uniform_int_distribution<int> zakres(0, m_zakresLos);
    int numer = zakres(m_generator);
    qInfo() << numer;
    int wylosowany = m_elementy[numer];
    m_elementy[numer] = m_bufor.last();
    m_bufor.removeLast();
    m_bufor.push_front(wylosowany);
    return wylosowany;*/
    QVector<int> numery(numPyt);
    for(int i=0;i<numPyt;i++)
    {
        //losowanie bez zwracania:
        std::uniform_int_distribution<int> zakres(0, m_zakresLos);
        int numer = zakres(m_generator);
        numery[i] = m_elementy[numer];
        std::swap(m_elementy[numer],m_elementy[m_zakresLos--]);
    }
    // reset losowania:
    m_zakresLos = m_liczbaElementow-1;
    return numery;
}

void Urn::setNum(int liczbaElementow)
{
    if(liczbaElementow != m_liczbaElementow)
    {
        m_elementy.clear();
        m_elementy.resize(liczbaElementow);
        for(int idx=0;idx<liczbaElementow;idx++)
            m_elementy[idx] = idx;
        m_liczbaElementow = liczbaElementow;
        m_zakresLos = m_liczbaElementow-1;
    }
}
