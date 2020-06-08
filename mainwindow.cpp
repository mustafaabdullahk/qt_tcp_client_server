#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>



MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->pushButton_disconnect->setVisible(false);

    client = new ClientStuff("192.168.1.100", 1000);

    setStatus(client->getStatus());

    connect(client, &ClientStuff::hasReadSome, this, &MainWindow::receivedSomething);
    //connect(client, &ClientStuff::tcp_paket_gonder, this, &MainWindow::receivedSomething);
    connect(client, &ClientStuff::statusChanged, this, &MainWindow::setStatus);
    // FIXME change this connection to the new syntax
    connect(client->tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(gotError(QAbstractSocket::SocketError)));
}

MainWindow::~MainWindow()
{
    delete client;
    delete ui;

}

void MainWindow::setStatus(bool newStatus)
{
    if(newStatus)
    {
        ui->label_status->setText(tr("<font color=\"green\">Bağlandı</font>"));
        ui->pushButton_connect->setVisible(false);
        ui->pushButton_disconnect->setVisible(true);
    }
    else
    {
        ui->label_status->setText(tr("<font color=\"red\">Bağlantı yok</font>"));
        ui->pushButton_connect->setVisible(true);
        ui->pushButton_disconnect->setVisible(false);
    }
}

void MainWindow::receivedSomething(QString msg)
{
    ui->textEdit_log->append(msg);
}

void MainWindow::gotError(QAbstractSocket::SocketError err)
{
    //qDebug() << "got error";
    QString strError = "unknown";
    switch (err)
    {
        case 0:
            strError = "Connection was refused";
            break;
        case 1:
            strError = "Remote host closed the connection";
            break;
        case 2:
            strError = "Host address was not found";
            break;
        case 5:
            strError = "Connection timed out";
            break;
        default:
            strError = "Unknown error";
    }
    ui->textEdit_log->append(strError);
}

void MainWindow::on_pushButton_connect_clicked()
{
    client->connect2host();
}

void MainWindow::on_pushButton_send_clicked()
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    //out.setVersion(QDataStream::Qt_5_10);
    out << quint16(0) << ui->lineEdit_message->text();

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    client->tcpSocket->write(arrBlock);
}

void MainWindow::on_pushButton_disconnect_clicked()
{
    client->closeConnection();
}

void MainWindow::tcp_paket_gonder()
{

    if(client->adim!=client->sayac_paket)
    {
    ui->textEdit_log->append("interval;");
    client->tcp_paket_gonder();
    client->sayac_paket++;
    }
    else
    {
        client->timerPaket->stop();
    }
}
void MainWindow::tcp_log_yaz()
{
    if(client->adim!=client->sayac_dosya)
    {
        client->tcp_log_yaz();
        client->sayac_dosya++;
    }
    else{client->timerDosya->stop();}

}
void MainWindow::on_pushButton_teststart_clicked()
{
    client->interval = ui->lineEdit_packagesize->text().toUInt()/(ui->lineEdit_speed->text().toUInt()*1024);
    client->adim = ui->lineEdit_datasize->text().toUInt()/ui->lineEdit_packagesize->text().toUInt();
    client->paket_boyutu = ui->lineEdit_packagesize->text().toUInt();
    client->paket_olustur(client->paket_boyutu);

    client->sayac_paket=0;
    client->sayac_dosya=0;

    connect(client->timerPaket, SIGNAL(timeout()), this, SLOT(tcp_paket_gonder()));
    connect(client->timerDosya, SIGNAL(timeout()), this, SLOT(tcp_log_yaz()));

    client->timerPaket->setInterval(client->interval);
    client->timerPaket->start();
    client->timerDosya->setInterval(client->interval+50);
    client->timerDosya->start();

}
