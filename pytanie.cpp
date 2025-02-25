#include "pytanie.h"
#include <QDebug>

Pytanie::Pytanie(QString &rawData, int num, int blok)
{

    m_num = num;
    m_blok = blok;
    QStringList separated = rawData.split("|");
    Q_ASSERT_X(separated.size()==4,"Pytanie","Nieparwidlowa liczba członów pytania");
    m_head = separated.at(0);
    m_know = ""; m_underst = ""; m_discus = "";
    QString trescKnow = separated.at(1);
    trescKnow.remove(0,1);
    m_know += "- " + trescKnow ;

    QString trescUnderst = separated.at(2);
    trescUnderst.remove(0,1);
    m_underst+= "- " + trescUnderst ;

    QString trescDiscus = separated.at(3);
    trescDiscus.remove(0,1);
    m_discus += "- " + trescDiscus ;
}

QString Pytanie::getTresc()
{
    QString tresc = "<p style=\"color:black\"><u>" + QString::number(m_num + 1) + ". " + m_head + "</u></p>";
    tresc += " <p style=\"color:red\">" + m_know + "</p>";
    tresc += " <p style=\"color:green\">" + m_underst + "</p>";
    tresc += " <p style=\"color:blue\">" + m_discus + "</p>";
    return tresc;
}

QString Pytanie::getPlainHead()
{
    return m_head;
}


