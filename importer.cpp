#include "importer.h"
#include <QDir>
#include <QDebug>

Importer::Importer()
{
    wykryjPrzedmioty();
    m_curPrzedmiot = m_listaPrzedmiotow.first();
}

void Importer::wykryjPrzedmioty()
{
    QDir pytania = QDir::current();
    pytania.cd("Pytania");
    pytania.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    m_listaPrzedmiotow = pytania.entryList();
}

void Importer::setPrzedmiot(QString nazwa)
{
    m_curPrzedmiot = nazwa;
}

void Importer::readData()
{
    QString egzPath("Pytania/" + m_curPrzedmiot);
    char sig = 'A';
    for(auto &blok:m_bloki)
        readBlock(egzPath,blok,sig++);
}

void Importer::readBlock(QString &path, QVector<Pytanie> &dst, char sig)
{
    QDir pytania = QDir::current();
    pytania.cd(path);

    QString filter = QString(sig) + "-*";
    QStringList filters = {filter};
    pytania.setFilter(QDir::Files);
    pytania.setNameFilters(filters);

    QStringList fileSigs = pytania.entryList();
    int numPyt = fileSigs.length();
    dst.resize(numPyt);
    for(int i = 0; i<numPyt;i++)
    {
        QString pytPath = path + "/" + fileSigs.at(i);
        dst[i] = readPyt(pytPath,i, (int)(sig-'A'));
    }
}

Pytanie Importer::readPyt(QString &path, int num, int blok)
{
    QString rawData;
    QFile plik(path);
    if(plik.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QByteArray data = plik.readAll();
        std::string str = data.toStdString();
        rawData = str.c_str();
        plik.close();
    }
    return Pytanie(rawData,num,blok);
}
