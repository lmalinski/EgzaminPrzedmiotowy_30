#include "pytanie.h"
#include <QDebug>

// KONSTRUKTOR - przyjmuje surowe dane pobrane z pliku ze znacznikami sterującymi oraz numer pytania i bloku:

Pytanie::Pytanie(QString &rawData, TypPytania typ, int num, int blok)
    : m_tresc(rawData), m_typ(typ), m_numer(num),m_blok(blok)
{}

QString Pytanie::getTresc()
{

    QString sygnatura = QString(NAZWY_KATALOGOW[(int)m_typ][0]) + " " + ((m_numer<9)? "0" : "") + QString::number(m_numer+1);
    switch(m_typ)
    {
    case TypPytania::Wiedza:
        return "<p style=\"color:red\">" + sygnatura + ": " + m_tresc + "</p>";
        break;
    case TypPytania::Zrozumienie:
        return "<p style=\"color:green\">" + sygnatura + ": " + m_tresc + "</p>";
        break;
    }
    return "";
}

