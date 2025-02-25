#include "importerJednostkowy.h"
#include <QDir>
#include <QDebug>

ImporterJednsotkowy::ImporterJednsotkowy(QObject *parent,QString dir)
    :Importer(parent,dir)
{}

void ImporterJednsotkowy::readData()
{
    QString egzPath(m_dir + "/" + m_curPrzedmiot);
    char sig = 'A';
    QVector<Pytanie> blok;
    while(readBlock(egzPath,blok,sig++))
    {
        m_bloki.push_back(blok);
    }
    m_numBlok = m_bloki.size();
}

bool ImporterJednsotkowy::readBlock(QString &path, QVector<Pytanie> &dst, char sig)
{
    QDir pytania = QDir::current();
    pytania.cd(path);

    QString filter = QString(sig) + "-*";
    QStringList filters = {filter};
    pytania.setFilter(QDir::Files);
    pytania.setNameFilters(filters);

    QStringList fileSigs = pytania.entryList();
    int numPyt = fileSigs.length();
    if (numPyt == 0)
        return false;
    dst.resize(numPyt);
    for(int i = 0; i<numPyt;i++)
    {
        QString pytPath = path + "/" + fileSigs.at(i);
        dst[i] = readPyt(pytPath,i, (int)(sig-'A'));
    }
    return true;
}

Pytanie ImporterJednsotkowy::readPyt(QString &path, int num, int blok)
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
