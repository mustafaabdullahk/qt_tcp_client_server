#ifndef TCP_TEST_H
#define TCP_TEST_H

#include <QString>
#include <QDataStream>
#include <QTimer>
#include <QMainWindow>

class tcp_test_class : public QObject
{
    Q_OBJECT
public:
    tcp_test_class();

public:
    void paket_olustur(uint paket_boyutu);
    void tcp_gonder();
    void log_yaz();
    void log_oku();

private slots:


public:
    QTimer *timerPaket=new QTimer(this);
    QTimer *timerDosya=new QTimer(this);
    int timerPaket_interval, timerDosya_interval;
    unsigned char paket [];
};


#endif // TCP_TEST_H
