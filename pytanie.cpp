#include "pytanie.h"
#include <QDebug>

Pytanie::Pytanie(QString &rawData, int num, int blok)
{
    m_num = num;
    m_blok = blok;
    QStringList separated = rawData.split(".");
    m_head = separated.at(0);
    m_crit = ""; m_other = "";
    for(int wpis = 1; wpis <= 2; wpis++)
    {
        QString tresc = separated.at(wpis);
        tresc.remove(0,1);
        m_crit += "- " + tresc + "<br>";
    }
    QString tresc = separated.at(3);
    tresc.remove(0,1);
    m_crit += "- " + tresc;
    for(int wpis = 4; wpis <= 5; wpis++)
    {
        QString tresc = separated.at(wpis);
        tresc.remove(0,1);
        m_other += "- " + tresc + "<br>";
    }
}

QString Pytanie::getTresc()
{
    QString tresc = "<p style=\"color:black\"><u>" + QString::number(m_num + 1) + ". " + m_head + "</u></p>";
    tresc += " Proszę: <p style=\"color:red\">" + m_crit + "</p>";
    tresc += " oraz: <p style=\"color:green\">" + m_other + "</p>";
    return tresc;
}


