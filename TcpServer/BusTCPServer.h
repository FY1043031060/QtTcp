#ifndef BUSTCPSERVER_H
#define BUSTCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
#include <QTimer>

class BusTCPServer : public QObject
{
    Q_OBJECT
public:
    explicit BusTCPServer(QObject *parent = nullptr);
    ~BusTCPServer();
signals:

public slots:
   void onDataSend(QByteArray data);
   void onNewConnection();

private:
    void initComponent();
    QTcpServer * m_pTcpServer;
    QTcpSocket * m_pTcpSocket;

    QTimer m_timer;
};

#endif // BUSTCPSERVER_H
