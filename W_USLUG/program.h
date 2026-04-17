#ifndef PROGRAM_H
#define PROGRAM_H

#include <QObject>

#include "W_DANYCH\bazapytan.h"

class Program : public QObject
{
    Q_OBJECT
public:
    explicit Program(QObject *parent = nullptr);
    ~Program() {delete m_BazaPytan;}
    void setPrzedmiot(QString nazwa);
    void wczytajPytania();
    int getLiczbaBlokow();
    void losujPytania(int blok, int addNumWiedza, int addNumZrozum);
    QString getAktPrzedmiot();
    QStringList getListaPrzedmiotow();
    void odznaczPytania(QVector<int>,int);
    void ponownieWypiszWylosowane(int blok);

signals:
    void wypisz(QVector<Pytanie>&,int);

private:
    static const int MAX_LICZ_BLOKOW = 2;
    BazaPytan * m_BazaPytan =nullptr;
    std::array<QVector<Pytanie>,MAX_LICZ_BLOKOW> m_WylosPyt;

};

#endif // PROGRAM_H
