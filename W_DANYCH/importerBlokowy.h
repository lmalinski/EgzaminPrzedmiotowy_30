#ifndef IMPORTERBLOKOWY_H
#define IMPORTERBLOKOWY_H

#include "importer.h"

// Klasa importer blokowy jest już konkretnym importerem czytajacym cały blok pytań
// z pojdeynczego pliku o nazwie blokX (X to duza litera od A do Z). Litera A to blok 0

class ImporterBlokowy : public Importer
{
    void wczytajBlok(QString &path, QVector<Pytanie> &dst, int numBlok, TypPytania typ);
public:
    explicit ImporterBlokowy(QString dir);
    ~ImporterBlokowy(){}
    std::vector<QVector<Pytanie>> wczytajDane(TypPytania typ) override;
};

#endif // IMPORTERBLOKOWY_H
