#include "importerBlokowy.h"
#include <QDir>
#include <qdebug>

// KONSTRUKTOR:

ImporterBlokowy::ImporterBlokowy(QString dir)
    : Importer(dir)
{}

// GŁÓWNE ALGORYTMY WCZYTYWANIA


// Metoda dokonująca pełnego wczytania wszystkich bloków dla wskazanego przedmiotu.
// (ISTOTNE: zmiana przedmiotu usuwa aktualną bazę pytań i wczytuje nową).
std::vector<QVector<Pytanie>> ImporterBlokowy::wczytajDane(TypPytania typ)
{
    // Przygotowanie kontenera na pytania:
    std::vector<QVector<Pytanie>> bloki;

    // Ustalenie katalogu z blokami dla zadanego przedmiotu:
    QString egzPath(m_dir + "/" + m_aktPrzedmiot);
    QDir pytania = QDir::current();
    pytania.cd(egzPath);

    // Uzyskanie listy nazw plików z pytaniami pasujących do formatu "typ*":
    QString filter = QString(NAZWY_KATALOGOW[(int)typ]) +"*";
    QStringList filters = {filter};
    pytania.setFilter(QDir::Files);
    pytania.setNameFilters(filters);
    QStringList files = pytania.entryList();
    int numFiles = files.length();

    // Wczytywanie bloków do bazy:
    Q_ASSERT_X(numFiles > 0, "ImporterBlokowy/wczytajDane", "Brak plików z blokami w katalogu");
    QVector<Pytanie> pojBlok;
    for(int idx = 0; idx < numFiles; idx++)
    {
        pojBlok.clear();
        QString pytPath = egzPath + "/" + files.at(idx);
        wczytajBlok(pytPath, pojBlok, idx, typ);
        bloki.push_back(pojBlok);
        qInfo() << idx << pytPath << pojBlok.size();
    }

    // Zapisanie faktycznej liczby wczytanych bloków:
    return bloki;
}

void ImporterBlokowy::wczytajBlok(QString &path, QVector<Pytanie> &dst, int numBlok, TypPytania typ)
{
    // Wczytanie pliku i pobranie pełnej treści:
    QString rawData;
    QFile plik(path);
    if (plik.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QByteArray data = plik.readAll();
        std::string str = data.toStdString();
        rawData = str.c_str();
        plik.close();
    }

    // Rozdzielenie pytań na podstawie znaku '@':
    QStringList separated = rawData.split("@");

    // Usunięcie znaków '\n' poprzedzających pytania (jeśli występują)
    // oraz dodanie pytań do bazy:
    for (int idx = 0; idx < separated.size() - 1; idx++)
    {
        auto wpis = separated.at(idx);
        if (wpis.at(0) == '\n')
            wpis.remove(0, 1);
        dst.push_back(Pytanie(wpis, typ, idx, numBlok));
    }
}
