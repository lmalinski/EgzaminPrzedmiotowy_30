#include "W_USLUG\program.h"
#include <QDebug>

// KONSTRUKTORY I SETUP:

// W konstruktorze podajemy katalog docelowy, w którym importer
// ma szukać przedmiotów i pytań:
Program::Program(QObject *parent)
    : QObject{parent},m_BazaPytan(new BazaPytan("Pytania"))
{}

// METODY - USŁUGI:

// USŁUGA zlecająca wybór przedmiotu do bazy pytań (importera):
void Program::setPrzedmiot(QString nazwa)
{
    m_BazaPytan->setPrzedmiot(nazwa);
}

// USŁUGA zlecajaca zadanie wczytania pytań dla aktualnie wybranego przedmiotu.
// Następnie metoda parametryzuje instancje urn do losowania indeksów pytań.
void Program::wczytajPytania()
{   
    m_BazaPytan->wczytajDane();
}

// USŁUGA "forwardująca" liczbę bloków zapisaną w bazie:
int Program::getLiczbaBlokow()
{
    return m_BazaPytan->getLiczbaBlokow();
}

// USŁUGA losująca pytania dla danego bloku. Przyjmuje liczbę dodatkowych pytań, gdyż
// zawsze losuje minimum 3 pytania (obowiązkowe na egzaminie):
void Program::losujPytania(int blok, int addNumWiedza,int addNumZrozum)
{
    //Losowanie pytan -> delegowane do bazy:
    m_WylosPyt[blok] = m_BazaPytan->losujPytania(blok,addNumWiedza,addNumZrozum);
    QVector<QString> tresci(m_WylosPyt[blok].size());
    int numPyt = m_WylosPyt[blok].size();
    for(int i = 0;i<numPyt;i++)
        tresci[i] = m_WylosPyt[blok][i].getTresc();

    // Zlecenie wypisania pytań do warstwy prezentacji przy pomocy sygnału:
    emit wypisz(tresci, blok);
}

// // USŁUGA "forwardująca" aktualnie obsługiwany przedmiot (zapisany w bazie)
QString Program::getAktPrzedmiot()
{
    return m_BazaPytan->getPrzedmiot();
}

// // USŁUGA "forwardująca" aktualną listę przedmiotów znalezioną w katalogu z bazą pytań
QStringList Program::getListaPrzedmiotow()
{
    return m_BazaPytan->getListaPrzedmiotow();
}

