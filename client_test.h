#ifndef CLIENTSTUFF_H
#define CLIENTSTUFF_H

#include <QString>
#include <QTcpSocket>
#include <QDataStream>
#include <QTimer>



class ClientStuff : public QObject
{
    Q_OBJECT

public:
    ClientStuff(const QString hostAddress, int portVal, QObject *parent = 0);
    QTcpSocket *tcpSocket;
    bool getStatus();



public slots:
    void closeConnection();
    void connect2host();

signals:
    void statusChanged(bool);
    void hasReadSome(QString msg);


private slots:
    void readyRead();
    void connected();
    void connectionTimeout();

public:
    void paket_olustur(uint paket_boyutu);
    void tcp_paket_gonder();
    void tcp_log_yaz();
    void log_oku();

private:
    QString host;
    int port;
    bool status;
    quint16 m_nNextBlockSize;
    QTimer *timeoutTimer;
public:

    QTimer *timerPaket=new QTimer(this);
    QTimer *timerDosya=new QTimer(this);
    uint timerPaket_interval, timerDosya_interval,adim,sayac_paket,sayac_dosya,interval,paket_boyutu;

    unsigned char paket[];

};

#endif // CLIENTSTUFF_H
