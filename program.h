#ifndef PROGRAM_H
#define PROGRAM_H

#include <QObject>
#include <urn.h>
#include "importer.h"


class Program : public QObject
{
    Q_OBJECT
public:
    explicit Program(QObject *parent = nullptr);
    void setPrzedmiot(QString nazwa);
    void loadPytania();
    int getLiczbaBlokow();
    void generujPytania(int blok, int num);
    QString getAktPrzedmiot();
    QStringList getListaPrzedmiotow();
    void odznaczPytania(QVector<int>,int);


signals:
    void wypisz(QVector<Pytanie>&,int);

private:
    static const int MAX_LICZ_BLOKOW = 2;
    static const int MIN_LICZ_PYT = 3;
    Importer * m_BazaPytan;
    std::array<Urn,MAX_LICZ_BLOKOW> m_bloki;
    std::array<QVector<Pytanie>,MAX_LICZ_BLOKOW> m_WylosPyt;

};

#endif // PROGRAM_H
