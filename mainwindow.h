#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "client_test.h"


namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
public slots:
    void setStatus(bool newStatus);
    void receivedSomething(QString msg);
    void gotError(QAbstractSocket::SocketError err);
    void tcp_paket_gonder();
    void tcp_log_yaz();

private slots:
    void on_pushButton_send_clicked();
    void on_pushButton_connect_clicked();
    void on_pushButton_disconnect_clicked();
    void on_pushButton_teststart_clicked();

private:
    Ui::MainWindow *ui;
    ClientStuff *client;

};

#endif // MAINWINDOW_H
