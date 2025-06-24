#ifndef IMPORTER_H
#define IMPORTER_H

#include <QObject>
#include <QVector>
#include "pytanie.h"

class Importer : public QObject
{
    Q_OBJECT
protected:
    int m_numBlok = 0;
    QString m_dir;
    std::vector<QVector<Pytanie>> m_bloki;
    QStringList m_listaPrzedmiotow;
    QString m_curPrzedmiot;
    void wykryjPrzedmioty();
public:
    explicit Importer(QObject *parent, QString dir);
    QStringList getPrzedmiotList() {return m_listaPrzedmiotow;}
    QVector<Pytanie> & getBlok(int num) {return m_bloki[num];}
    void setPrzedmiot(QString nazwa) {m_curPrzedmiot = nazwa;}
    QString getPrzedmiot() {return m_curPrzedmiot;}
    int getNumBlok() {return m_numBlok;}
    virtual void readData() = 0;

signals:
};

#endif // IMPORTER_H
