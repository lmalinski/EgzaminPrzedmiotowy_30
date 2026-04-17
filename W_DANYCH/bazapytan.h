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
    std::vector<QVector<Pytanie>> m_wiedzaBlok;
    std::vector<QVector<Pytanie>> m_zrozumienieBlok;
    //liczba elemetów urn jest dostsosowana do liczby typow pytan - pobranie z liczby katalogów, gdyż nie da się odczytać liczby wartosci w enum
    std::array<std::vector<Urn>,std::size(NAZWY_KATALOGOW)> m_urnyDoLosowania;

public:
    BazaPytan(QString dir);
    ~BazaPytan(){delete m_importer;}
    void setPrzedmiot(QString nazwa) {m_importer->setPrzedmiot(nazwa);}
    void wczytajDane();
    int getLiczbaBlokow() {return m_liczbaBlokow;}
    //QVector<Pytanie> & getBlok(int num) {return m_bloki.at(num);}
    QString getPrzedmiot() {return m_importer->getPrzedmiot();}
    QStringList getListaPrzedmiotow() {return m_importer->getListaPrzedmiotow();}
    QVector<Pytanie> losujPytania(int blok, int addNumWiedza, int addNumZrozum);


};

#endif // BAZAPYTAN_H
