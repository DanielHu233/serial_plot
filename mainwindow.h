#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "qcustomplot.h"

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>
#include <QPainter>
#include <QPointF>
#include <QPen>
#include <qmath.h>
#include <QMessageBox>
#include <QDebug>
#include <QDesktopWidget>
#include <QScreen>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static QSerialPort::BaudRate getBaudRate(int);
    void setupDrawing();
    void axisPaint(QCustomPlot *customPlot);
    void setting(QCustomPlot *customPlot);
    void widgetPaint(int);
    void dataCreator();

public slots:
    void serialPort_readyRead();

    void on_searchButton_clicked();

    void on_openButton_clicked();

    void on_sendButton_clicked();

    void on_clearButton_clicked();

    void on_recieveButton_clicked();

//    void getnPlotData();

//    bool eventFilter(QObject *watched, QEvent *event);

    void plotCurves();

    void SendPacket();

    void recievePacket();

private slots:
    void on_checkBox_stateChanged(int arg1);

    void on_checkBox2_stateChanged(int arg1);

    void on_checkBox3_stateChanged(int arg1);

    void on_checkBox4_stateChanged(int arg1);

    void on_checkBox5_stateChanged(int arg1);

    void on_checkBox6_stateChanged(int arg1);

    void on_checkBox7_stateChanged(int arg1);

    void on_checkBox8_stateChanged(int arg1);

    void on_startPaint_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort serial;
    QList<QList<float>> yList;
    QList<float>list_1,list_2,list_3,list_4,list_5,list_6,list_7,list_8;
//    QList<float> xList;
    QTimer dataTimer;
//    QTimer sendTimer;
    QList<int> countList;
    QPen pen1,pen2,pen3,pen4,pen5,pen6,pen7,pen8,pen9;
//    QList<QPen> penList;
    bool box_1_on,box_2_on,box_3_on,box_4_on,box_5_on,box_6_on,box_7_on,box_8_on;
//    QCPGraph *graph1,*graph2,*graph3,*graph4,*graph5,*graph6,*graph7,*graph8;
};

#endif // MAINWINDOW_H
