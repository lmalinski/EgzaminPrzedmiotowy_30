#ifndef BAZAPYTAN_H
#define BAZAPYTAN_H

#include <QString>

#include "W_DANYCH\urn.h"
#include "W_DANYCH\importer.h"

//Ta klasa odpowiada za organizacje i przechowyanie pytan do losowania.
class BazaPytan
{
private:
    int m_liczbaBlokow = 0;
    Importer *m_importer = nullptr;
    std::vector<QVector<Pytanie>> m_bloki;
    std::vector<Urn> m_urnyDoLosowania;
    static const int MIN_LICZ_PYT = 3;

public:
    BazaPytan(QString dir);
    ~BazaPytan(){delete m_importer;}
    void setPrzedmiot(QString nazwa) {m_importer->setPrzedmiot(nazwa);}
    void wczytajDane();
    int getLiczbaBlokow() {return m_liczbaBlokow;}
    //QVector<Pytanie> & getBlok(int num) {return m_bloki.at(num);}
    QString getPrzedmiot() {return m_importer->getPrzedmiot();}
    QStringList getListaPrzedmiotow() {return m_importer->getListaPrzedmiotow();}
    QVector<Pytanie> & getBlok(int num) {return m_bloki.at(num);}
    QVector<Pytanie> losujPytania(int blok, int addNum);


};

#endif // BAZAPYTAN_H
