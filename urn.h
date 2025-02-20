#ifndef URN_H
#define URN_H
#include <random>
#include <QVector>
#include <QQueue>
#include <string>

class Urn
{
    QVector<int> m_elementy;
    QQueue<int> m_bufor;
    int m_zakresLos,m_liczbaElementow;
    std::mt19937 m_generator;
    void setup();
public:
    Urn(int liczbaElementow);
    Urn();
    QVector<int> losuj(int numPyt);
    bool maElementy(){return (m_elementy.size()>0);}
    void losujInit();
    void setNum(int liczbaElementow);
};
#endif // URN_H
