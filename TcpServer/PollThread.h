#ifndef POLLTHREAD_H
#define POLLTHREAD_H

#include <QObject>
#include <QThread>
#include <TcpServerThread.h>

class PollThread : public QObject
{
    Q_OBJECT
public:
    explicit PollThread(QObject *parent = nullptr);

signals:

public slots:
    void processWork();
private:
    TCPServerThread m_tcpThread;

};

#endif // POLLTHREAD_H
