#include "pytanie.h"
#include <QDebug>

// KONSTRUKTOR - przyjmuje surowe dane pobrane z pliku ze znacznikami sterujacymi, oraz numer pytania i bloku:

Pytanie::Pytanie(QString &rawData, int num, int blok)
{
    m_numer = num;
    m_blok = blok;

    // Dzielenie tresci pytan na znakach |:
    QStringList separated = rawData.split("|");
    Q_ASSERT_X(separated.size() == 4,"Pytanie","Nieparwidlowa liczba członów pytania");

    // Odczytanie nagłowka:
    m_naglowek = separated.at(0);

    QString bufor; // bufor do wstepnego przetwarzania tresci pytań

    // Odczytanie wiedzy:
    bufor = separated.at(1);
    bufor.remove(0,1);
    m_wiedza = "- " + bufor ;

    // Odytanie zrozumienia:
    bufor = separated.at(2);
    bufor.remove(0,1);
    m_zrozum+= "- " + bufor;

    // Odytanie dyskusji:
    bufor = separated.at(3);
    bufor.remove(0,1);
    m_dyskusja += "- " + bufor;
}

// GETERY TRESCI CZYSTEJ (sam tekst):

QString Pytanie::getCzystyNaglowek()
{
    return m_naglowek;
}

// GETERY TRESCI FORMATOWANEJ (tekst ze znacznikami HTML):

QString Pytanie::getNaglowek()
{
    return "<p style=\"color:black\"><u>" + QString::number(m_numer + 1) + ". " + m_naglowek + "</u></p>";
}

QString Pytanie::getWiedza()
{
    return " <p style=\"color:red\">" + m_wiedza + "</p>";
}

QString Pytanie::getZrozumienie()
{
    return " <p style=\"color:green\">" + m_zrozum + "</p>";
}

QString Pytanie::getDyskusja()
{
    return " <p style=\"color:blue\">" + m_dyskusja + "</p>";
}

QString Pytanie::getTresc()
{
    return getNaglowek() + getWiedza() + getZrozumienie() + getDyskusja();
}


