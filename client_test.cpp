#include "client_test.h"
#include <QFile>
#include <Qt3DCore/private/qcircularbuffer_p.h>

Qt3DCore::QCircularBuffer<unsigned char> readBuffer;
Qt3DCore::QCircularBuffer<unsigned char> writeBuffer;
QVector<char> paket_array(0);
QByteArray paket_byte(0);
QFile file_write("writeLog.txt");
QFile file_read("readLog.txt");

ClientStuff::ClientStuff(const QString hostAddress,int portNumber,QObject *parent) : QObject(parent), m_nNextBlockSize(0)
{
    status = false;

    host = hostAddress;
    port = portNumber;

    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, &QTcpSocket::disconnected, this, &ClientStuff::closeConnection);

    timeoutTimer = new QTimer();
    timeoutTimer->setSingleShot(true);
    connect(timeoutTimer, &QTimer::timeout, this, &ClientStuff::connectionTimeout);
}

void ClientStuff::connect2host()
{
    timeoutTimer->start(3000);

    tcpSocket->connectToHost(host, port);
    connect(tcpSocket, &QTcpSocket::connected, this, &ClientStuff::connected);
    connect(tcpSocket, &QTcpSocket::readyRead, this, &ClientStuff::readyRead);
}

void ClientStuff::connectionTimeout()
{
    //qDebug() << tcpSocket->state();
    if(tcpSocket->state() == QAbstractSocket::ConnectingState)
    {
        tcpSocket->abort();
        emit tcpSocket->error(QAbstractSocket::SocketTimeoutError);
    }
}

void ClientStuff::connected()
{
    status = true;
    emit statusChanged(status);
}

bool ClientStuff::getStatus() {return status;}

void ClientStuff::readyRead()
{

    /*QDataStream in(tcpSocket);
    //in.setVersion(QDataStream::Qt_5_10);
    for (;;)
    {
        if (!m_nNextBlockSize)
        {
            if (tcpSocket->bytesAvailable() < sizeof(quint64)) { break; }
            in >> m_nNextBlockSize;
        }
        if (tcpSocket->bytesAvailable() < m_nNextBlockSize) { break; }

        QString str; in >> str;

        if (str == "0")
        {
            str = "Connection closed";
            closeConnection();
        }

        emit hasReadSome(str);
        m_nNextBlockSize = 0;
    }*/
    /*QDataStream in(tcpSocket);
    //in.setVersion(QDataStream::Qt_5_10);
    for (uint i=0;i<paket_boyutu;i++)
    {
        if (!m_nNextBlockSize)
        {
            if (tcpSocket->bytesAvailable() < sizeof(quint64)) { break; }
            in >> m_nNextBlockSize;
        }
        if (tcpSocket->bytesAvailable() < m_nNextBlockSize) { break; }
        QByteArray data=tcpSocket->readAll();
        readBuffer.append(data[i]);

        //QString str; in >> str;
        QString str;
        if (str == "0")
        {
            str = "Connection closed";
            closeConnection();
        }

        str=QString::number(readBuffer.capacity());
        emit hasReadSome(str);
        m_nNextBlockSize = 0;
    }*/
    //QByteArray byte;
    //QDataStream in(&byte, QIODevice::ReadOnly);
    //QDataStream in(tcpSocket);
    //out.setVersion(QDataStream::Qt_5_10);
    //in << quint16(0) << paket_array.at(1);
   // in.device()->seek(0);
    //in << quint16(byte.size() - sizeof(quint16));
    //in >> byte;
    QByteArray rcvdData = tcpSocket->read(tcpSocket->bytesAvailable());
    for(uint i=0;i<paket_boyutu;i++)
    {
        //in>>readBuffer[i];
        readBuffer.append(rcvdData.at(i));
    }
    //QString str = QString(rcvdData.toStdString().c_str());
    QString str=QString::number(readBuffer.capacity());
    emit hasReadSome(str);

    /*QByteArray rcvdData = tcpSocket->read(tcpSocket->bytesAvailable());
    for(uint i=0;i<paket_boyutu;i++)
    {
        readBuffer.append(rcvdData.at(i));
    }
    //QString str = QString(rcvdData.toStdString().c_str());
    QString str=QString::number(readBuffer.capacity());
    emit hasReadSome(str);*/


}

//void ClientStuff::gotDisconnection()
//{
//    status = false;
//    emit statusChanged(status);
//}

void ClientStuff::closeConnection()
{
    timeoutTimer->stop();

    //qDebug() << tcpSocket->state();
    disconnect(tcpSocket, &QTcpSocket::connected, 0, 0);
    disconnect(tcpSocket, &QTcpSocket::readyRead, 0, 0);

    bool shouldEmit = false;
    switch (tcpSocket->state())
    {
        case 0:
            tcpSocket->disconnectFromHost();
            shouldEmit = true;
            break;
        case 2:
            tcpSocket->abort();
            shouldEmit = true;
            break;
        default:
            tcpSocket->abort();
    }

    if (shouldEmit)
    {
        status = false;
        emit statusChanged(status);
    }
}
void ClientStuff::paket_olustur(uint paket_boyutu)
{    
    for(uint i=0;i<paket_boyutu;i++)
    {
    paket_array.append(i);
    }
    for(uint i=0;i<paket_boyutu;i++)
    {
    paket_byte.append(i);
    }
    writeBuffer.setCapacity(paket_boyutu);
    readBuffer.setCapacity(paket_boyutu);
    if (!file_write.open(QIODevice::WriteOnly | QIODevice::Text)){return;}
    if (!file_read.open(QIODevice::WriteOnly | QIODevice::Text)){return;}
}
void ClientStuff::tcp_paket_gonder()
{   
    QByteArray byte;    
    QDataStream out(&byte, QIODevice::WriteOnly);
    //out.setVersion(QDataStream::Qt_5_10);
    out << quint16(0) << paket_byte;
    out.device()->seek(0);
    out << quint16(byte.size() - sizeof(quint16));

    for(uint i=0;i<paket_boyutu;i++)
    {
        //out << quint16(0) <<paket_array.at(i);
        writeBuffer.append(paket_array.at(i));
        tcpSocket->write(byte);
    }
}

void ClientStuff::tcp_log_yaz()
{
    //for(uint8_t a=0;a<1;a++)





    QTextStream write_out(&file_write);
    QTextStream read_out(&file_read);

    for(uint8_t i=0;i<writeBuffer.size();i++)
    {
    write_out << writeBuffer.at(i);
    read_out << readBuffer.at(i);
    }

}
