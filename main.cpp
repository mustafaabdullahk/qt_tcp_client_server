#include <QApplication>
#include <QTextCodec>
#include "mainwindow.h"
#include <Qt3DCore>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    MainWindow w;
    w.show();
    
    return a.exec();
}
