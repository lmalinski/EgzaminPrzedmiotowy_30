#ifndef IMPORTER_H
#define IMPORTER_H

#include <QObject>
#include <QVector>
#include "pytanie.h"

class Importer : public QObject
{
    Q_OBJECT
    std::array<QVector<Pytanie>,2> m_bloki;
    QStringList m_listaPrzedmiotow;
    QString m_curPrzedmiot;

    void wykryjPrzedmioty();
    void readBlock(QString &path, QVector<Pytanie> &dst,char sig);
    Pytanie readPyt(QString &path, int num, int blok);
public:
    QStringList getPrzedmiotList() {return m_listaPrzedmiotow;}
    void setPrzedmiot(QString nazwa);
    QString getPrzedmiot() {return m_curPrzedmiot;}
    void readData();
    Importer();
    QVector<Pytanie> & getBlok(int num) {return m_bloki[num];}
};

#endif // IMPORTER_H
