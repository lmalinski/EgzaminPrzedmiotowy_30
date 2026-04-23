#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCheckBox>
#include <QHBoxLayout>

// KONSTRUKTORY, SETUP I DESTRUKTOR:

                                   MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this); // Wbudowane w Qt

    // Tablice wskaźników do sterowania zbiorowego. Pozwalają odwoływać się do kontrolek po numerze,
    // co ułatwia proceduralną modyfikację interfejsu (np. w pętli) i realizację zasady DRY:
    m_pytDispPelny = {ui->pytAPelny, ui->pytBPelny};
    m_pytLiczW     = {ui->liczAW, ui->liczBW};
    m_pytLiczZ     = {ui->liczAZ, ui->liczBZ};
    m_genPush      = {ui->pushGenBA, ui->pushGenBB};
    m_tabs         = {ui->tabA, ui->tabB};

    // Ustawienie comboboxa z nazwami przedmiotów:
    ui->przedmiot->clear();
    ui->przedmiot->addItems(m_uslugi.getListaPrzedmiotow());
    ui->przedmiot->setCurrentIndex(1); // Domyślnie PPK

    // Moja inicjalizacja stanu początkowego kontrolek w GUI:
    ustawGUI();

    // Podłączenie sygnałów z warstwy usług:
    connect(&m_uslugi, SIGNAL(wypisz(QVector<QString>&, int)),
            this, SLOT(wypisz(QVector<QString>&, int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ustawGUI()
{
    wylaczWszystko();
    on_pushWyczysc_clicked();
    ui->pushWczytaj->setEnabled(true);
    QString aktPrzedmiot = m_uslugi.getAktPrzedmiot();
    this->setWindowTitle(aktPrzedmiot);
    ui->tabBloki->setCurrentIndex(0);
}


// METODY (AKCJE):

// Metoda do zbiorowej dezaktywacji większości kontrolek:
void MainWindow::wylaczWszystko()
{
    for(int blok = 0; blok < MAX_LICZ_BLOKOW; blok++)
    {
        m_pytDispPelny[blok]->setEnabled(false);
        m_pytLiczW[blok]->setEnabled(false);
        m_pytLiczZ[blok]->setEnabled(false);
        m_genPush[blok]->setEnabled(false);
        m_tabs[blok]->setEnabled(false);
    }
}

// Metoda wywołująca generowanie pytań dla danego bloku.
// Wydzielona w celu uniknięcia powielania kodu:
void MainWindow::genrujPytania(int blok)
{
    // Dezaktywacja kontrolek GUI:
    m_genPush[blok]->setEnabled(false);
    m_pytLiczW[blok]->setEnabled(false);
    m_pytLiczZ[blok]->setEnabled(false);
    ui->pushWyczysc->setEnabled(true);

    // Wywołanie usługi generowania pytań – przekazywana jest liczba *dodatkowych* pytań,
    // ponieważ domyślnie generowane są 3 pytania bazowe.
    m_uslugi.losujPytania(blok, m_pytLiczW[blok]->value(),m_pytLiczZ[blok]->value());
}

// SLOTY (REAKCJE):

// Ta metoda jest slotem reagującym na sygnał „wypisz” z warstwy usług (program.cpp)
void MainWindow::wypisz(QVector<QString>& pytania, int blok)
{
    // Przełączenie widoczności na QTextBrowser:
    m_pytDispPelny[blok]->setEnabled(true);
    QString tresc = "";
    for(int pyt = 0; pyt < pytania.size(); pyt++)
        tresc += pytania[pyt];


    // Wyświetlenie treści na ekranie:
    m_pytDispPelny[blok]->setText(tresc);
}

// Metoda dokonująca resetu stanu kontrolek i widgetów tekstowych po zakończeniu egzaminu:
void MainWindow::on_pushWyczysc_clicked()
{
    for(int blok = 0; blok < m_uslugi.getLiczbaBlokow(); blok++)
    {
        m_tabs[blok]->setEnabled(true);
        m_pytLiczW[blok]->setEnabled(true);
        m_pytLiczZ[blok]->setEnabled(true);
        m_genPush[blok]->setEnabled(true);
        m_pytDispPelny[blok]->clear();
        m_pytLiczW[blok]->setValue(0);
        m_pytLiczZ[blok]->setValue(0);
    }
    ui->tabBloki->setCurrentIndex(0);
}

// Cztery kolejne metody są reakcjami na naciśnięcie konkretnych przycisków.
// Aby nie powtarzać kodu (zasada DRY), delegują one zadania do metod prywatnych,
// wskazując, którego bloku dotyczy reakcja:

void MainWindow::on_pushGenBA_clicked()
{
    genrujPytania(0); // przekazuje numer bloku
}

void MainWindow::on_pushGenBB_clicked()
{
    genrujPytania(1); // przekazuje numer bloku
}

// Metoda reakcji na zmianę przedmiotu – wywołuje usługę zmiany przedmiotu w warstwie usług
// (czyści całą bazę pytań). Dodatkowo przywraca GUI do stanu początkowego.
void MainWindow::on_przedmiot_currentTextChanged(const QString &i_przedmiot)
{
    m_uslugi.setPrzedmiot(i_przedmiot);
    ustawGUI();
}

// Metoda reakcji na wczytanie pytań. Na czas wczytywania dezaktywuje wszystkie
// kontrolki, które mogłyby wprowadzić program w nieprawidłowy stan.
// Następnie wywołuje usługę wczytania bazy pytań z plików na dysku (dotychczasowa
// baza zostaje całkowicie wymieniona). Potem przywraca GUI do stanu gotowości do generowania pytań.
void MainWindow::on_pushWczytaj_clicked()
{
    wylaczWszystko();
    m_uslugi.wczytajPytania();
    ui->pushWczytaj->setEnabled(false);
    on_pushWyczysc_clicked();
}