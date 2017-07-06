#include "BusTCPServer.h"
#include <QDebug>
BusTCPServer::BusTCPServer(QObject *parent)
    : QObject(parent), m_pTcpServer(nullptr), m_pTcpSocket(nullptr)
{
    m_pTcpServer = new QTcpServer(this);
    m_pTcpServer->listen(QHostAddress::Any,8888);
    initComponent();
}

BusTCPServer::~BusTCPServer()
{
    m_pTcpServer->close();
    if(m_pTcpSocket != NULL)
        m_pTcpSocket->close();
}

void BusTCPServer::onDataSend(QByteArray data)
{
    if(m_pTcpSocket != nullptr)
        if(m_pTcpSocket->isValid())
            m_pTcpSocket->write(data.data(),data.size());
}


void BusTCPServer::onNewConnection()
{
    //取出建立好连接的套接字
    m_pTcpSocket = m_pTcpServer->nextPendingConnection();
    //获取对方IP和端口
    QString ip = m_pTcpSocket->peerAddress().toString();   //IP地址 字符串
    quint16 port = m_pTcpSocket->peerPort();             //端口 无符号整型

    qDebug() << __FUNCTION__ << ip << port;

    connect(m_pTcpSocket, &QTcpSocket::readyRead,
            [&](){
        //从通信套接字中取出内容
        QByteArray array = m_pTcpSocket->readAll();
        QString temp =QString(array);
        qDebug() << __FUNCTION__ << temp;

    });
}

void BusTCPServer::initComponent()
{
    connect(m_pTcpServer, &QTcpServer::newConnection,
            this,&BusTCPServer::onNewConnection);
    connect(m_pTcpServer,&QTcpServer::acceptError,[&](QAbstractSocket::SocketError socketError){
        qDebug() << m_pTcpServer->errorString();
    });
}
