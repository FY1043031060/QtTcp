#include "TCPServerThread.h"
#include <QDebug>
TCPServerThread::TCPServerThread(QObject *parent) :
    QObject(parent)
{
    m_tcpServer.moveToThread(&m_thread);
    connect(this, &TCPServerThread::dataSend,
            &m_tcpServer, &BusTCPServer::onDataSend,Qt::QueuedConnection);
    connect(&m_threadTimer,&QTimer::timeout,[&](){
        emit dataSend(QByteArray(3,'b'));

    });
    m_threadTimer.start(20);
}




