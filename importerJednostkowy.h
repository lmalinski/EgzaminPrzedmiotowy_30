#ifndef IMPORTERJEDNOSTKOWY_H
#define IMPORTERJEDNOSTKOWY_H

#include <QObject>
#include <QVector>
#include "importer.h"

class ImporterJednsotkowy : public Importer
{
    Q_OBJECT
protected:
    bool readBlock(QString &path, QVector<Pytanie> &dst,char sig);
    Pytanie readPyt(QString &path, int num, int blok);
public:
    ImporterJednsotkowy(QObject *parent,QString dir);
    void readData() override;
};

#endif // IMPORTERJEDNOSTKOWY_H
