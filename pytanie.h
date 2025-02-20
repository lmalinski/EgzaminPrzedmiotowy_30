#ifndef PYTANIE_H
#define PYTANIE_H

#include <QString>

class Pytanie
{
    QString m_head = "";
    QString m_crit = "";
    QString m_other = "";
    int m_num;
    int m_blok;
public:
    Pytanie() = default;
    Pytanie(QString &rawData, int num, int blok);
    QString getTresc();
    int getNum () {return m_num;}
    int getBlok () {return m_blok;}
};

#endif // PYTANIE_H
