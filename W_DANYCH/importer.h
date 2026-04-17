#ifndef IMPORTER_H
#define IMPORTER_H

#include <QVector>
#include "pytanie.h"

// Importer jest klasą bazową definiumącą podstawowe narzędzia potrzebne we wszytkich
// mozliwych impoertreach (klasy pochodne od tej) jakie mogą współpracować z programem.
// Pozwala to na stsounkowo latwą podmianę algorytmu wczytywania pytań bez potrzeby
// ingerenecji w resztę programu (interfejs jest zdefiniowany w tej klasie).

class Importer
{
protected:
    QString m_dir;
    QStringList m_listaPrzedmiotow;
    QString m_aktPrzedmiot;
    void wykryjPrzedmioty();
public:
    virtual std::vector<QVector<Pytanie>> wczytajDane(TypPytania typ) = 0;
    explicit Importer(QString dir);
    virtual ~Importer(){}
    QStringList getListaPrzedmiotow() {return m_listaPrzedmiotow;}
    void setPrzedmiot(QString nazwa) {m_aktPrzedmiot = nazwa;}
    QString getPrzedmiot() {return m_aktPrzedmiot;}
};

#endif // IMPORTER_H
