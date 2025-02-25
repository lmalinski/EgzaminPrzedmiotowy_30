#include "importer.h"
#include <QDir>
#include <QDebug>

Importer::Importer(QObject *parent, QString dir)
    : QObject{parent},m_dir(dir)
{
    wykryjPrzedmioty();
    m_curPrzedmiot = m_listaPrzedmiotow.first();
}

void Importer::wykryjPrzedmioty()
{
    QDir pytania = QDir::current();
    pytania.cd(m_dir);
    pytania.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    m_listaPrzedmiotow = pytania.entryList();
}

