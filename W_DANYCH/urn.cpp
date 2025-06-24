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
    std::random_device rseed;
    m_generator.seed((rseed()));
    m_zakresLos = m_liczbaElementow-1;
}

void Urn::generujRozklad()
{
    //UWAGA liczenie rozkładu zawsze odbywa się w zakresie losowania:
    m_rozklad.fill(0.0f);
    int n = m_zakresLos + 1;
    std::transform(m_liczLos.begin(),m_liczLos.begin()+ n,
                   m_rozklad.begin(),[](float arg){return 1.0f/arg;});
    float suma = std::accumulate(m_rozklad.begin(),m_rozklad.begin()+n,0.0f);
    std::transform(m_rozklad.begin(),m_rozklad.begin()+ n,
                   m_rozklad.begin(),[suma](float arg){return arg/suma;});

}

void Urn::generujDytsr()
{
    //UWAGA liczenie rozkładu zawsze odbywa się w zakresie losowania:
    m_dystr.fill(0.0f);
    int n = m_zakresLos + 1;
    std::transform(m_rozklad.begin(),m_rozklad.begin()+ n,
                   m_dystr.begin(),[sum=0.0f](float arg) mutable {return sum+=arg;} );
}

void Urn::normalizujLiczebnosci()
{
    float suma = std::accumulate(m_liczLos.begin(),m_liczLos.end(),0.0f);
    std::transform(m_liczLos.begin(),m_liczLos.end(),m_liczLos.begin(),
                   [this,suma](float arg){return arg*m_liczbaElementow/suma; });
}

QVector<int> Urn::losuj(int numPyt)
{
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

int Urn::losujZDystr()
{
    std::uniform_real_distribution<float> zakres(0.0f,1.0f);
    float val = zakres(m_generator);
    for(int idx=0;idx<m_zakresLos+1;idx++)
    {
        if( val <= m_dystr[idx])
            return idx;
    }
    return -1;
}

QVector<int> Urn::losujRedukcjaPowt(int numPyt)
{
    normalizujLiczebnosci();
    QVector<int> numery(numPyt);
    for(int idx=0; idx<numPyt; idx++)
    {
        generujRozklad();
        generujDytsr();
        //losowanie bez zwracania:
        int numer = losujZDystr();
        numery[idx] = m_elementy[numer];
        m_liczLos[numer] +=10;
        std::swap(m_elementy[numer],m_elementy[m_zakresLos]);
        std::swap(m_liczLos[numer],m_liczLos[m_zakresLos]);
        m_zakresLos--;
    }
    // reset losowania:
    m_zakresLos = m_liczbaElementow-1;
    return numery;
}

void Urn::setNum(int liczbaElementow)
{
    if(liczbaElementow != m_liczbaElementow)
    {
        //Wymagajace zmiany rozmiaru i reinicjalizacji:
        m_elementy.clear();
        m_elementy.resize(liczbaElementow);
        m_liczLos.clear();
        m_liczLos.resize(liczbaElementow);

        //Wymagajacące tylko zmiany rozmiaru (nadpisywane przy użyciu)
        m_rozklad.resize(liczbaElementow);
        m_dystr.resize(liczbaElementow);

        for(int idx=0;idx<liczbaElementow;idx++)
        {
            m_elementy[idx] = idx;
            m_liczLos[idx] = 1.0f;
        }
        m_liczbaElementow = liczbaElementow;
        m_zakresLos = m_liczbaElementow-1;
    }
}
