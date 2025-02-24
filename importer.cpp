#include "importer.h"
#include <QDir>
#include <QDebug>

Importer::Importer(QObject *parent)
    : QObject{parent}
{}

void Importer::wykryjPrzedmioty()
{
    QDir pytania = QDir::current();
    pytania.cd("Pytania");
    pytania.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    m_listaPrzedmiotow = pytania.entryList();
}
