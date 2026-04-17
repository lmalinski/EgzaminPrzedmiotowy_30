#include "bazapytan.h"
#include "W_DANYCH\importerBlokowy.h"

BazaPytan::BazaPytan(QString dir)
    :m_importer(new ImporterBlokowy(dir))
{}

void BazaPytan::wczytajDane()
{
    m_bloki = m_importer->wczytajDane();
    m_liczbaBlokow = m_bloki.size();
}
