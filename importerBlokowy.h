#ifndef IMPORTERBLOKOWY_H
#define IMPORTERBLOKOWY_H

#include "importer.h"

class ImporterBlokowy : public Importer
{
    Q_OBJECT
    void readBlock(QString &path, QVector<Pytanie> &dst, int numBlok);
public:
    explicit ImporterBlokowy(QObject *parent, QString dir);
    void readData() override;
};

#endif // IMPORTERBLOKOWY_H
