#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //发送按键失能
    ui->sendButton->setEnabled(false);
    //读取串口信息
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts()) {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite)) {
        ui->portNameBox->addItem(serial.portName());
        serial.close();
        }
    }
    //设置波特率
    ui->baudrateBox->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    ui->baudrateBox->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    ui->baudrateBox->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    ui->baudrateBox->addItem(QStringLiteral("57600"), QSerialPort::Baud57600);
    ui->baudrateBox->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
    ui->baudrateBox->setCurrentIndex(0);

    //设置数据位
    ui->dataBitsBox->addItem(QStringLiteral("8"), QSerialPort::Data8);
    ui->dataBitsBox->setCurrentIndex(0);
    //设置奇偶校验位
    ui->ParityBox->addItem(tr("None"), QSerialPort::NoParity);
    ui->ParityBox->setCurrentIndex(0);
    //设置停止位
    ui->stopBitsBox->addItem(QStringLiteral("1"), QSerialPort::OneStop);
    ui->stopBitsBox->setCurrentIndex(0);
    //接受按键失能
    ui->recieveButton->setEnabled(false);

    ui->checkBox->setChecked(0);
    ui->checkBox2->setChecked(0);
    ui->checkBox3->setChecked(0);
    ui->checkBox4->setChecked(0);
    ui->checkBox5->setChecked(0);
    ui->checkBox6->setChecked(0);
    ui->checkBox7->setChecked(0);
    ui->checkBox8->setChecked(0);

    //初始化基础参数
    for(int i = 0;i < 8;i++){
        countList.append(0);
    }
    box_1_on=box_2_on=box_3_on=box_4_on=box_5_on=box_6_on=box_7_on=box_8_on = false;

    //初始化ylist结构
    yList.append(list_1);
    yList.append(list_2);
    yList.append(list_3);
    yList.append(list_4);
    yList.append(list_5);
    yList.append(list_6);
    yList.append(list_7);
    yList.append(list_8);

//画图部分

    //画图参数设置
    setupDrawing();



//应该不需要了
    //事件过滤器
//    ui->plot->installEventFilter(this);



//    connect(ui->plot,SIGNAL(mousePress(QMouseEvent *)),this,SLOT(SLOT_mouseTracetoCoord(QMouseEvent *)));

//画图
    //初始化坐标系
    axisPaint(ui->plot);
    //初始化画笔和图层
    setting(ui->plot);
    //画八个实时折线
    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(plotCurves()));
    //接收数据并parse
    QObject::connect(&serial, &QSerialPort::readyRead, this, &MainWindow::serialPort_readyRead);        

    dataTimer.start(0);
   // sendTimer.start(100);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SendPacket(){

}

void MainWindow::recievePacket(){

}

void MainWindow::serialPort_readyRead()
{
    //从接收缓冲区中读取数据
    QByteArray buffer = serial.readAll();
    //从界面中读取以前收到的数据
    QString recv = ui->recvTextEdit->toPlainText();
    recv += QString(buffer);
    //清空以前的显示
    ui->recvTextEdit->clear();
    //重新显示
    ui->recvTextEdit->append(recv);
}

void MainWindow::on_searchButton_clicked()
{
    ui->portNameBox->clear();
    //通过QSerialPortInfo查找可用串口
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        ui->portNameBox->addItem(info.portName());
    }
}

void MainWindow::on_openButton_clicked()
{
    if(ui->openButton->text()==QString("打开串口"))
    {
        //设置串口名
        serial.setPortName(ui->portNameBox->currentText());
        //设置波特率
        serial.setBaudRate(ui->baudrateBox->currentText().toInt());

        serial.setDataBits(QSerialPort::Data8);      //数据位为8位
        serial.setFlowControl(QSerialPort::NoFlowControl); //无流控制
        serial.setParity(QSerialPort::NoParity); //无校验位
        serial.setStopBits(QSerialPort::OneStop); //一位停止位

        //打开串口
        if(!serial.open(QIODevice::ReadWrite))
        {
            QMessageBox::about(NULL, "提示", "无法打开串口！");
            return;
        }

        //下拉菜单控件失能
        ui->portNameBox->setEnabled(false);
        ui->baudrateBox->setEnabled(false);
        ui->dataBitsBox->setEnabled(false);
        ui->ParityBox->setEnabled(false);
        ui->stopBitsBox->setEnabled(false);
        ui->recieveButton->setEnabled(true);

        ui->openButton->setText(QString("关闭串口"));
        //发送按键使能
        ui->sendButton->setEnabled(true);
    }
    else
    {
        //关闭串口
        serial.close();

        //下拉菜单控件使能
        ui->portNameBox->setEnabled(true);
        ui->baudrateBox->setEnabled(true);
        ui->dataBitsBox->setEnabled(true);
        ui->ParityBox->setEnabled(true);
        ui->stopBitsBox->setEnabled(true);
        ui->recieveButton->setEnabled(false);

        ui->openButton->setText(QString("打开串口"));
        //发送按键失能
        ui->sendButton->setEnabled(false);
    }
}

void MainWindow::on_sendButton_clicked()
{
    //获取界面上的数据并转换成utf8格式的字节流
    QByteArray data = ui->sendTextEdit->toPlainText().toUtf8();
    serial.write(data);
}

void MainWindow::on_clearButton_clicked()
{
    ui->recvTextEdit->clear();
}

/*
void MainWindow::on_recieveButton_clicked()
{
    //连接信号和槽
    if(ui->recieveButton->text() == QString("接收数据")){
        QObject::connect(&serial, &QSerialPort::readyRead, this, &MainWindow::serialPort_readyRead);        
        //10 Hz频率发送数据
        connect(&sendTimer, SIGNAL(timeout()), this, SLOT(SendPacket()));
        ui->recieveButton->setText(QString("读取中"));
    }else{
        QObject::disconnect(&serial, &QSerialPort::readyRead, this, &MainWindow::serialPort_readyRead);
        disconnect(&sendTimer, SIGNAL(timeout()), this, SLOT(SendPacket()));
        ui->recieveButton->setText(QString("接收数据"));
    }
}
*/

void MainWindow::on_startPaint_clicked()
{
    //连接信号和槽
    if(ui->startPaint->text() == QString("暂停")){
        dataTimer.stop();
        ui->startPaint->setText(QString("开始"));
    }else{
        dataTimer.start();
        ui->startPaint->setText(QString("暂停"));
    }
}

void MainWindow::setupDrawing(){
    //8个折线画笔
    pen1.setColor(Qt::green);
    pen1.setStyle(Qt::SolidLine);
    pen1.setWidthF(1);
    pen2.setColor(Qt::red);
    pen2.setStyle(Qt::SolidLine);
    pen2.setWidthF(1);
    pen3.setColor(Qt::gray);
    pen3.setStyle(Qt::SolidLine);
    pen3.setWidthF(1);
    pen4.setColor(Qt::black);
    pen4.setStyle(Qt::SolidLine);
    pen4.setWidthF(1);
    pen5.setColor(Qt::blue);
    pen5.setStyle(Qt::SolidLine);
    pen5.setWidthF(1);
    pen6.setColor(Qt::darkYellow);
    pen6.setStyle(Qt::SolidLine);
    pen6.setWidthF(1);
    pen7.setColor(QColor("indigo"));
    pen7.setStyle(Qt::SolidLine);
    pen7.setWidthF(1);
    pen8.setColor(QColor("lightpink"));
    pen8.setStyle(Qt::SolidLine);
    pen8.setWidthF(1);
    pen9.setColor(Qt::black);
    pen9.setStyle(Qt::SolidLine);
    pen9.setWidthF(0.01);

/*  penList.append(pen1);
    penList.append(pen2);
    penList.append(pen3);
    penList.append(pen4);
    penList.append(pen5);
    penList.append(pen6);
    penList.append(pen7);
    penList.append(pen8);   */
}

//绘制发生的地方
//bool MainWindow::eventFilter(QObject *watched, QEvent *event)
//{
//    if(watched == ui->plot && event->type() == QEvent::Paint)
//    {
//    //初始化坐标系
//        axisPaint(ui->plot);
//    //初始化画笔和graph
//        setting(ui->plot);
//    //画八个不同的折线
//        if(box_1_on){
//            widgetPaint(0);
//        }
//        if(box_2_on){
//            widgetPaint(1);
//        }
//        if(box_3_on){
//            widgetPaint(2);
//        }
//        if(box_4_on){
//            widgetPaint(3);
//        }
//        if(box_5_on){
//            widgetPaint(4);
//        }
//        if(box_6_on){
//            widgetPaint(5);
//        }
//        if(box_7_on){
//            widgetPaint(6);
//        }
//        if(box_8_on){
//            widgetPaint(7);
//        }
//    //完善坐标系

//    }
//    return QMainWindow::eventFilter(watched,event);
//}

void MainWindow::axisPaint(QCustomPlot *customPlot)
{
    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    customPlot->xAxis->setTicker(timeTicker);
    customPlot->axisRect()->setupFullAxesBox();
    customPlot->yAxis->setRange(-100, 100);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));
}

void MainWindow::setting(QCustomPlot *customPlot){
    customPlot->addGraph();
    customPlot->graph(0)->setPen(pen1);
    customPlot->addGraph();
    customPlot->graph(1)->setPen(pen2);
    customPlot->addGraph();
    customPlot->graph(2)->setPen(pen3);
    customPlot->addGraph();
    customPlot->graph(3)->setPen(pen4);
    customPlot->addGraph();
    customPlot->graph(4)->setPen(pen5);
    customPlot->addGraph();
    customPlot->graph(5)->setPen(pen6);
    customPlot->addGraph();
    customPlot->graph(6)->setPen(pen7);
    customPlot->addGraph();
    customPlot->graph(7)->setPen(pen8);
    customPlot->addGraph();
    customPlot->graph(8)->setPen(pen9);
}

void MainWindow::widgetPaint(int j){
    static QTime time(QTime::currentTime());

    //创建时间轴
    double key = time.elapsed()/1000.0;
    static double lastPointKey = 0;

    //最快更新率为0.01s，请在这里改变绘图刷新频率
    if (key-lastPointKey > 0.01){
        dataCreator();
        //添加点
        if((yList[j].size() >= countList[j]) & !yList[j].isEmpty()){
            ui->plot->graph(j)->addData(key,(double)yList[j][countList[j]]);
//            ui->plot->graph(j)->addData(key,j*30*qrand()/(double)RAND_MAX);
            //调整axis的范围
            ui->plot->graph(j)->rescaleValueAxis(true);
            lastPointKey = key;
//            ui->plot->replot();

            //更新countList（为了能够在关闭一个曲线一段时间后重新打开时能继续画他的图）
            countList[j]++;
            int maxVal = countList[0];
            for(int i = 0;i < 8;i++){
                if(maxVal < countList[i]){
                    maxVal = countList[i];
                }
            }
            for(int i = 0;i < 8;i++){
                countList[i] = maxVal;
            }
        }
    }

    //使时间轴能够滚动
    ui->plot->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->plot->replot();

    //计算帧率并显示在左下角
    static double lastFpsKey;
    static int frameCount;
    ++frameCount;
    if (key-lastFpsKey > 2)
    {
      ui->statusBar->showMessage(
            QString("%1 FPS")
            .arg(frameCount/(key-lastFpsKey), 0, 'f', 0));
      lastFpsKey = key;
      frameCount = 0;
    }
}

void MainWindow::plotCurves(){
    if(box_1_on){
        widgetPaint(0);
    }
    if(box_2_on){
        widgetPaint(1);
    }
    if(box_3_on){
        widgetPaint(2);
    }
    if(box_4_on){
        widgetPaint(3);
    }
    if(box_5_on){
        widgetPaint(4);
    }
    if(box_6_on){
        widgetPaint(5);
    }
    if(box_7_on){
        widgetPaint(6);
    }
    if(box_8_on){
        widgetPaint(7);
    }
}

//监测哪些曲线需要绘制
void MainWindow::on_checkBox_stateChanged(int arg1)
{
    if(ui->checkBox->isChecked() == true){
        box_1_on = true;
    }else{
        ui->plot->graph(0)->data().data()->clear();
        box_1_on = false;
    }
}
void MainWindow::on_checkBox2_stateChanged(int arg1)
{
    if(ui->checkBox2->isChecked() == true){
        box_2_on = true;
    }else{
        ui->plot->graph(1)->data().data()->clear();
        box_2_on = false;
    }
}
void MainWindow::on_checkBox3_stateChanged(int arg1)
{
    if(ui->checkBox3->isChecked() == true){
        box_3_on = true;
    }else{
        ui->plot->graph(2)->data().data()->clear();
        box_3_on = false;
    }
}
void MainWindow::on_checkBox4_stateChanged(int arg1)
{
    if(ui->checkBox4->isChecked() == true){
        box_4_on = true;
    }else{
        ui->plot->graph(3)->data().data()->clear();
        box_4_on = false;
    }
}
void MainWindow::on_checkBox5_stateChanged(int arg1)
{
    if(ui->checkBox5->isChecked() == true){
        box_5_on = true;
    }else{
        ui->plot->graph(4)->data().data()->clear();
        box_5_on = false;
    }
}
void MainWindow::on_checkBox6_stateChanged(int arg1)
{
    if(ui->checkBox6->isChecked() == true){
        box_6_on = true;
    }else{
        ui->plot->graph(5)->data().data()->clear();
        box_6_on = false;
    }
}
void MainWindow::on_checkBox7_stateChanged(int arg1)
{
    if(ui->checkBox7->isChecked() == true){
        box_7_on = true;
    }else{
        ui->plot->graph(6)->data().data()->clear();
        box_7_on = false;
    }
}
void MainWindow::on_checkBox8_stateChanged(int arg1)
{
    if(ui->checkBox8->isChecked() == true){
        box_8_on = true;
    }else{
        ui->plot->graph(7)->data().data()->clear();
        box_8_on = false;
    }
}

void MainWindow::dataCreator(){
    for(int i = 0;i < 8;i++){
        yList[i].append((i+1)*20*qrand()/(double)RAND_MAX);
    }
}
