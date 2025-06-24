#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCheckBox>
#include <QHBoxLayout>

// KONSTRUKTORY, SETUP i DESTRUKTOR:

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this); //wbudowane

    //tablice do strowania zbiorowego:
    m_pytDisp      = {ui->pytA,ui->pytB};
    m_pytDispPelny = {ui->pytAPelny,ui->pytBPelny};
    m_pytLicz      = {ui->liczA,ui->liczB};
    m_genPush      = {ui->pushGenBA,ui->pushGenBB};
    m_wybPush      = {ui->pushWybBA,ui->pushWybBB};
    m_tabs         = {ui->tabA,ui->tabB};

    //Ustawienie comboboxa z nazwamy przedmiotów:
    ui->przedmiot->clear();
    ui->przedmiot->addItems(m_uslugi.getListaPrzedmiotow());
    ui->przedmiot->setCurrentIndex(1); // domyślnie PPK

    ustawGUI();
    connect(&m_uslugi,SIGNAL(wypisz(QVector<Pytanie>&, int)),this,SLOT(wypisz(QVector<Pytanie>&,int)));
}

void MainWindow::ustawGUI()
{
    wylaczWszystko();
    on_pushWyczysc_clicked();
    ui->pushWczytaj->setEnabled(true);
    QString curPrzedmiot = m_uslugi.getAktPrzedmiot();
    this->setWindowTitle(curPrzedmiot);
    ui->tabBloki->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// METODY (AKCJE):

void MainWindow::wylaczWszystko()
{
    for(int blok=0;blok<MAX_LICZ_BLOKOW;blok++)
    {
        m_pytDisp[blok]->setEnabled(false);
        m_pytDispPelny[blok]->setEnabled(false);
        m_pytLicz[blok]->setEnabled(false);
        m_genPush[blok]->setEnabled(false);
        m_tabs[blok]->setEnabled(false);
    }
}

void MainWindow::wypiszFull(QVector<Pytanie> &pytania, int blok)
{
    m_pytDisp[blok]->hide();
    m_pytDispPelny[blok]->show();
    m_pytDispPelny[blok]->setEnabled(true);
    QString tresc = "";
    if(ui->checkDekomponuj->isChecked()) // wypisywanie z rozbiciem na sekcje
    {
        for(int pyt = 0; pyt < pytania.size(); pyt++)
            tresc += pytania[pyt].getNaglowek();
        for(int pyt = 0; pyt < pytania.size(); pyt++)
            tresc += pytania[pyt].getWiedza();
        for(int pyt = 0; pyt < pytania.size(); pyt++)
            tresc += pytania[pyt].getZrozumienie();
        for(int pyt = 0; pyt < pytania.size(); pyt++)
            tresc += pytania[pyt].getDyskusja();
    }
    else //wypisywanie normalne z podziałem na pytania
    {
        for(int pyt = 0; pyt < pytania.size(); pyt++)
            tresc += pytania[pyt].getTresc();
    }
    m_pytDispPelny[blok]->setText(tresc);
    ui->checkDekomponuj->setEnabled(true);
}

void MainWindow::tworzTabele(int blok,int num)
{
    m_pytDisp[blok]->clear();
    m_pytDisp[blok]->setEnabled(true);
    m_pytDisp[blok]->setColumnCount(2);
    m_pytDisp[blok]->setRowCount(num);
    m_pytDisp[blok]->setColumnWidth(0,50);
    m_pytDisp[blok]->setColumnWidth(1,915);
    m_pytDisp[blok]->setHorizontalHeaderItem(0,new QTableWidgetItem("Usunąć"));
    m_pytDisp[blok]->setHorizontalHeaderItem(1,new QTableWidgetItem("Pytanie"));

    //Checkboxy:
    for(int pyt = 0; pyt < num ;pyt++)
    {
        QWidget *pWidget = new QWidget();
        QCheckBox *pCheckBox = new QCheckBox();
        QHBoxLayout *pLayout = new QHBoxLayout(pWidget);
        pLayout->addWidget(pCheckBox);
        pLayout->setAlignment(Qt::AlignCenter);
        pLayout->setContentsMargins(0,0,0,0);
        pWidget->setLayout(pLayout);
        m_pytDisp[blok]->setCellWidget(pyt,0,pWidget);
        m_chkBoxes[blok].push_back(pCheckBox);
    }

}

QVector<int> MainWindow::sprawdzDoUsuniecia(int blok)
{
    int numRows = m_pytDisp[blok]->rowCount();
    QVector<int> pytNums;
    for(int row =0; row < numRows; row++)
    {
        if(m_chkBoxes[blok][row]->isChecked())
            pytNums.push_back(row);
    }
    return pytNums;
}

void MainWindow::wylaczCheckBokxy(int blok)
{
    int numRows = m_pytDisp[blok]->rowCount();
    QVector<int> pytNums;
    for(int row =0; row < numRows; row++)
        m_chkBoxes[blok][row]->setEnabled(false);
}

void MainWindow::wyborPytan(int blok)
{
    QVector<int> numToRem = sprawdzDoUsuniecia(blok);
    if(numToRem.size() != m_pytLicz[blok]->value())
    {
        ui->statusBar->showMessage("Nie właściwa liczba pytan!");
        return;
    }
    wylaczCheckBokxy(blok);
    m_uslugi.odznaczPytania(numToRem,blok);
    m_wybPush[blok]->setEnabled(false);
}

void MainWindow::genrujPytania(int blok)
{
    m_genPush[blok]->setEnabled(false);
    m_pytLicz[blok]->setEnabled(false);
    m_wybPush[blok]->setEnabled(false);
    ui->pushWyczysc->setEnabled(true);
    m_uslugi.generujPytania(blok,m_pytLicz[blok]->value());
}


// SLOTY (REAKCJE):

void MainWindow::wypisz(QVector<Pytanie>& pytania, int blok)
{
    if(pytania.size() == MIN_LICZ_PYT)
    {
        wypiszFull(pytania,blok);
    }
    else //więcej pytan:
    {
        m_pytDisp[blok]->show();
        m_pytDispPelny[blok]->hide();
        int numPyt = pytania.size();
        tworzTabele(blok,numPyt);
        for(int pyt = 0; pyt < numPyt; pyt++)
            m_pytDisp[blok]->setItem(pyt,1,new QTableWidgetItem(pytania[pyt].getCzystyNaglowek()));
        m_wybPush[blok]->setEnabled(true);
        ui->checkDekomponuj->setEnabled(false);
    }
}

void MainWindow::on_pushWyczysc_clicked()
{
    for(int blok=0;blok<m_uslugi.getLiczbaBlokow();blok++)
    {
        m_tabs[blok]->setEnabled(true);
        m_pytLicz[blok]->setEnabled(true);
        m_genPush[blok]->setEnabled(true);
        m_pytDisp[blok]->clear();
        m_pytDispPelny[blok]->clear();
        m_chkBoxes[blok].clear();
        m_pytLicz[blok]->setValue(0);
    }
    ui->tabBloki->setCurrentIndex(0);
}

void MainWindow::on_pushGenBA_clicked()
{
    genrujPytania(0); // przekazuje numer bloku
}

void MainWindow::on_pushGenBB_clicked()
{
    genrujPytania(1); // przekazuje numer bloku
}

void MainWindow::on_przedmiot_currentTextChanged(const QString &i_przedmiot)
{
    m_uslugi.setPrzedmiot(i_przedmiot);
    ustawGUI();
}

void MainWindow::on_pushWczytaj_clicked()
{
    wylaczWszystko();
    m_uslugi.wczytajPytania();
    ui->pushWczytaj->setEnabled(false);
    on_pushWyczysc_clicked();
}

void MainWindow::on_pushWybBA_clicked()
{
    wyborPytan(0); // przekazuje numer bloku
}


void MainWindow::on_pushWybBB_clicked()
{
    wyborPytan(1); // przekazuje numer bloku
}


void MainWindow::on_checkDekomponuj_stateChanged(int state)
{
    Q_UNUSED(state);
    m_uslugi.zwrocAktWylosowane(ui->tabBloki->currentIndex());
}

