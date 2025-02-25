#include "importerblokowy.h"
#include <QDir>

ImporterBlokowy::ImporterBlokowy(QObject *parent, QString dir)
    : Importer(parent,dir)
{}

void ImporterBlokowy::readData()
{
    QString egzPath(m_dir + "/" + m_curPrzedmiot);
    QDir pytania = QDir::current();
    pytania.cd(egzPath);

    QString filter = "Blok*";
    QStringList filters = {filter};
    pytania.setFilter(QDir::Files);
    pytania.setNameFilters(filters);

    QStringList files = pytania.entryList();
    int numBlok = files.length();
    assert(numBlok > 0);
    QVector<Pytanie> blok;
    for(int idx = 0 ;idx < numBlok; idx++)
    {
        QString pytPath = egzPath + "/" + files.at(idx);
        readBlock(pytPath,blok,idx);
        m_bloki.push_back(blok);
    }
    m_numBlok = m_bloki.size();


}

void ImporterBlokowy::readBlock(QString &path, QVector<Pytanie> &dst, int numBlok)
{
    //Wczytanie pliku i pobranie pełnej tresci:
    QString rawData;
    QFile plik(path);
    if(plik.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QByteArray data = plik.readAll();
        std::string str = data.toStdString();
        rawData = str.c_str();
        plik.close();
    }

    //Separacja pytań n aznaku (@):
    QStringList separated = rawData.split("@");

    // Oczyszczenie ze znaków '\n' poprzedzających pytania 2-n:
    // i dodawanie pytań
    for(int idx = 0; idx < separated.size()-1; idx++ )
    {
        auto wpis = separated.at(idx);
        if(wpis.at(0) == '\n')
            wpis.remove(0,1);
        dst.push_back(Pytanie(wpis,idx,numBlok));
    }
}
