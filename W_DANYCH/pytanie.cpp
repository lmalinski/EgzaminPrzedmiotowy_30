#include "pytanie.h"
#include <QDebug>

// KONSTRUKTOR - przyjmuje surowe dane pobrane z pliku ze znacznikami sterującymi oraz numer pytania i bloku:

Pytanie::Pytanie(QString &rawData, TypPytania typ, int num, int blok)
    : m_tresc(rawData), m_typ(typ), m_numer(num),m_blok(blok)
{}

QString Pytanie::getTresc()
{
    switch(m_typ)
    {
    case TypPytania::Wiedza:
        return "<p style=\"color:red\">" + m_tresc + "</p>";
        break;
    case TypPytania::Zrozumienie:
        return "<p style=\"color:green\">" + m_tresc + "</p>";
        break;
    }
    return "";
}

