#include "bazapytan.h"
#include "W_DANYCH\importerBlokowy.h"

BazaPytan::BazaPytan(QString dir)
    :m_importer(new ImporterBlokowy(dir))
{}
