#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QDebug>
#include <QLabel>
#include <QTextBrowser>
#include <QSpinBox>
#include <QPushButton>
#include <QComboBox>
#include "importer.h"
#include "urn.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushGen_clicked();
    void on_pushClear_clicked();
    void on_przedmiot_currentTextChanged(const QString &spec);
    void on_pushLoad_clicked();
    // Trochę słabe, ale obawiam się, że konieczne przy użyciu Qt Desinera:
    void on_prevA_clicked() {prevPyt(0);}
    void on_nextA_clicked() {nextPyt(0);}
    void on_prevB_clicked() {prevPyt(1);}
    void on_nextB_clicked() {nextPyt(1);}
    void on_ocenaA_currentIndexChanged(int index);
    void on_ocenaB_currentIndexChanged(int index);


private:
    static const int MAX_LICZ_BLOKOW = 2;

    void setupPrzedmiot();
    void setupGUI();
    void dissableAll();
    void feedUrns();
    void free(Urn ** blok);
    void freeAll();
    void wypisz(Pytanie &pyt);
    void nextPyt(int blok);
    void prevPyt(int blok);
    void wyliczOcene();

    Ui::MainWindow *ui;
    Importer m_BazaPytan;
    int m_liczbaOcen;
    std::array<int, MAX_LICZ_BLOKOW> m_wybrPyt;
    std::array<Urn,MAX_LICZ_BLOKOW> m_bloki;
    std::array<QVector<Pytanie>,MAX_LICZ_BLOKOW> m_WylosPyt;
    std::array<QTextBrowser*,MAX_LICZ_BLOKOW> m_pytDisp;
    std::array<QSpinBox*,MAX_LICZ_BLOKOW> m_pytLicz;
    std::array<QPushButton*,MAX_LICZ_BLOKOW> m_prevBut;
    std::array<QPushButton*,MAX_LICZ_BLOKOW> m_nextBut;
    std::array<QComboBox*,MAX_LICZ_BLOKOW> m_oceny;

};

#endif // MAINWINDOW_H
