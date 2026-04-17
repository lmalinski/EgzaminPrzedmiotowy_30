#ifndef PYTANIE_H
#define PYTANIE_H

#include <QString>

enum class TypPytania{Wiedza,Zrozumienie};

class Pytanie
{
    QString m_tresc = "";
    TypPytania m_typ = TypPytania::Wiedza;
    int m_numer = 0;
    int m_blok = 0;
public:
    Pytanie() = default;
    Pytanie(QString &rawData, TypPytania typ, int num, int blok);
    QString getTresc();
    TypPytania getTyp() const {return m_typ;}
    int getNumer () const {return m_numer;}
    int getBlok () const {return m_blok;}
};

#endif // PYTANIE_H
