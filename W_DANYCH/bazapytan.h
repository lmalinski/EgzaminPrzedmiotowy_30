#ifndef BAZAPYTAN_H
#define BAZAPYTAN_H

#include <QString>

#include "W_DANYCH\importer.h"

class BazaPytan
{
private:
    Importer *m_importer = nullptr;
public:
    BazaPytan(QString dir);
    ~BazaPytan(){delete m_importer;}
    void setPrzedmiot(QString nazwa) {m_importer->setPrzedmiot(nazwa);}
    void wczytajDane() {m_importer->wczytajDane();}
    int getNumBlok() {return m_importer->getNumBlok();}
    QVector<Pytanie> & getBlok(int num) {return m_importer->getBlok(num);}
    QString getPrzedmiot() {return m_importer->getPrzedmiot();}
    QStringList getListaPrzedmiotow() {return m_importer->getListaPrzedmiotow();}


};

#endif // BAZAPYTAN_H
