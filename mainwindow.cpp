#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Inicjalizacja poniższych tablic na liście inic. tu nie działa,
    // więc trzeba to zrobić w ciele. To wskaźniki, więc koszt
    // kopiowania nie jest duży, a i tak wykonuje się to tylko raz:
    m_pytDisp = {ui->pytA,ui->pytB};
    m_pytLicz = {ui->liczA,ui->liczB};
    m_prevBut = {ui->prevA,ui->prevB};
    m_nextBut = {ui->nextA,ui->nextB};
    m_oceny   = {ui->ocenaA,ui->ocenaB};

    setupPrzedmiot();
    ui->przedmiot->setCurrentIndex(1); //PPK
    setupGUI();
}

void MainWindow::setupGUI()
{
    dissableAll();
    on_pushClear_clicked();
    ui->pushGen->setEnabled(false);
    ui->pushClear->setEnabled(false);
    ui->pushLoad->setEnabled(true);
    QString curPrzedmiot = m_BazaPytan.getPrzedmiot();
    this->setWindowTitle(curPrzedmiot);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushGen_clicked()
{
    ui->pushGen->setEnabled(false);
    m_wybrPyt = {0,0};
    for(int blok=0;blok<MAX_LICZ_BLOKOW;blok++)
    {
        m_prevBut[blok]->setEnabled(false);
        m_nextBut[blok]->setEnabled(false);
        if(m_bloki[blok].maElementy())
        {
            int toLos = m_pytLicz[blok]->value();
            if (toLos == 0)
            {
                m_pytDisp[blok]->setText("<br> <p align=\"center\">\nZaliczony/Zwolniony</p>");
                continue;
            }
            else if(toLos > 1)
                m_nextBut[blok]->setEnabled(true);
            m_WylosPyt[blok].clear();
            m_WylosPyt[blok].resize(toLos);
            QVector<int> numery = m_bloki[blok].losuj(toLos);
            for(int p = 0; p < toLos;p++)
                m_WylosPyt[blok][p] = m_BazaPytan.getBlok(blok).at(numery[p]);
            wypisz(m_WylosPyt[blok][0]);
        }
    }
    ui->pushClear->setEnabled(true);
}

void MainWindow::wypisz(Pytanie &pyt)
{
    int blok = pyt.getBlok();
    m_pytDisp[blok]->setText(pyt.getTresc());
}

void MainWindow::on_pushClear_clicked()
{
    for(int blok=0;blok<MAX_LICZ_BLOKOW;blok++)
    {
        m_pytDisp[blok]->setText("");
        m_pytLicz[blok]->setValue(1);
        m_prevBut[blok]->setEnabled(false);
        m_nextBut[blok]->setEnabled(false);
        m_oceny[blok]->setCurrentIndex(0);
    }
    ui->pushGen->setEnabled(true);
    ui->ocenaFin->setText("--");
}

void MainWindow::setupPrzedmiot()
{
    ui->przedmiot->clear();
    ui->przedmiot->addItems(m_BazaPytan.getPrzedmiotList());
}


void MainWindow::dissableAll()
{
    for(int blok=0;blok<MAX_LICZ_BLOKOW;blok++)
    {
        m_pytDisp[blok]->setEnabled(false);
        m_pytLicz[blok]->setEnabled(false);
        m_oceny[blok]->setEnabled(false);
    }
}


void MainWindow::on_przedmiot_currentTextChanged(const QString &i_przedmiot)
{
    m_BazaPytan.setPrzedmiot(i_przedmiot);
    setupGUI();
}


void MainWindow::on_pushLoad_clicked()
{
    dissableAll();
    m_BazaPytan.readData();
    m_liczbaOcen = 0;
    for(int blok =0;blok<MAX_LICZ_BLOKOW;blok++)
    {
        int num = m_BazaPytan.getBlok(blok).size();
        m_bloki[blok].setNum(num);
        if(num>0)
        {
            m_pytDisp[blok]->setEnabled(true);
            m_pytLicz[blok]->setEnabled(true);
            m_oceny[blok]->setEnabled(true);
            m_liczbaOcen++;
        }
    }
    ui->pushLoad->setEnabled(false);
    on_pushClear_clicked();
}

// Te dwie metody można by jeszcze na siłe zastąpić jedną ale
// obawiam się, że niepotrzebnie skomnplikowało by to kod.
// Trzeba by wtedy przekazywać info o kierunku.
void MainWindow::nextPyt(int blok)
{
    int wybPyt = ++m_wybrPyt[blok];
    m_prevBut[blok]->setEnabled(true);
    if(wybPyt == m_pytLicz[blok]->value()-1)
        m_nextBut[blok]->setEnabled(false);
    wypisz(m_WylosPyt[blok][wybPyt]);
}

void MainWindow::prevPyt(int blok)
{
    int wybrPyt = --m_wybrPyt[blok];
    m_nextBut[blok]->setEnabled(true);
    if(wybrPyt == 0)
        m_prevBut[blok]->setEnabled(false);
    wypisz(m_WylosPyt[blok][wybrPyt]);
}

void MainWindow::wyliczOcene()
{
    double oceny[] = {0.0,2.0,3.0,3.5,4.0,4.5,5.0};
    double srednia = 0;
    for(int numOcena=0;numOcena<m_liczbaOcen;numOcena++)
    {
        double ocena = oceny[m_oceny[numOcena]->currentIndex()];
        if(ocena == 0.0 )
            return;
        if(ocena == 2.0)
        {
            srednia = m_liczbaOcen * 2.0;
            break;
        }
        srednia+= ocena;
    }
    srednia /= m_liczbaOcen;
    ui->ocenaFin->setText(QString::number(srednia,'f',2));
}

void MainWindow::on_ocenaA_currentIndexChanged(int index)
{
    if(index > 0) wyliczOcene();
}

void MainWindow::on_ocenaB_currentIndexChanged(int index)
{
    if(index > 0) wyliczOcene();
}
