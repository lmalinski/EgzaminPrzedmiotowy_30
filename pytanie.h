#ifndef PYTANIE_H
#define PYTANIE_H

#include <QString>

class Pytanie
{
    QString m_head    = "";
    QString m_know    = "";
    QString m_underst = "";
    QString m_discus  = "";
    int m_num;
    int m_blok;
public:
    Pytanie() = default;
    Pytanie(QString &rawData, int num, int blok);
    QString getTresc();
    QString getPlainHead();
    QString getHead();
    QString getKnow();
    QString getUnderst();
    QString getDiscus();
    int getNum () {return m_num;}
    int getBlok () {return m_blok;}
};

#endif // PYTANIE_H
