#ifndef TCPSERVERTHREAD_H
#define TCPSERVERTHREAD_H

#include <QObject>
#include <QThread>
#include "BusTCPServer.h"
#include <QTimer>

class TCPServerThread : public QObject
{
    Q_OBJECT
public:
    explicit TCPServerThread(QObject *parent = nullptr);

signals:
    void dataSend(QByteArray data);

private:
    BusTCPServer m_tcpServer;
    QThread m_thread;
    QTimer m_threadTimer;
};

#endif // TCPSERVERTHREAD_H
