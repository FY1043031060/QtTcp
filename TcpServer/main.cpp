#include <QCoreApplication>
#include "TcpServerThread.h"
#include <QTimer>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
//    ServerWidget w;
//    w.show();
    TCPServerThread s;

    QTimer m_timer;
    QObject::connect(&m_timer,&QTimer::timeout,[&](){
        emit s.dataSend(QByteArray(8,'a'));
    });
    m_timer.start(20);

    return a.exec();
}
