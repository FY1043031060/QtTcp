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
    //ȡ�����������ӵ��׽���
    m_pTcpSocket = m_pTcpServer->nextPendingConnection();
    //��ȡ�Է�IP�Ͷ˿�
    QString ip = m_pTcpSocket->peerAddress().toString();   //IP��ַ �ַ���
    quint16 port = m_pTcpSocket->peerPort();             //�˿� �޷�������

    qDebug() << __FUNCTION__ << ip << port;

    connect(m_pTcpSocket, &QTcpSocket::readyRead,
            [&](){
        //��ͨ���׽�����ȡ������
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
