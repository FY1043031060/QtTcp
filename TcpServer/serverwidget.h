#ifndef SERVERWIDGET_H
#define SERVERWIDGET_H

#include <QWidget>

//�������Ҫ�����׽��� �ͻ�����Ҫһ��
#include<QTcpServer> //�����׽���
#include<QTcpSocket> //ͨ���׽���
#include<QFile>      //�ļ�����
#include<QTimer>      //��ʱ��
#include<QThread>

namespace Ui {
class ServerWidget;
}

class ServerWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit ServerWidget(QWidget *parent = 0);
    ~ServerWidget();
    
    void sendData();  //�����ļ�����
    
private slots:
    void on_buttonSend_clicked();
    void on_buttonClose_clicked();
    void on_buttonFSend_clicked();
    void on_buttonMediaInfo_clicked();
    
private:
    Ui::ServerWidget *ui;
    
    QTcpServer *tcpServer; //�����׽���
    QTcpSocket *tcpSocket; //ͨ���׽���
    
    QFile file; //�ļ�����
    QString fileName; //�ļ�����
    qint64 fileSize;  //�ļ���С
    qint64 sendSize;  //�Ѿ������ļ���С
    
    QTimer timer;       //��ʱ��
    QThread m_thread;   //�����߳�
};

#endif // SERVERWIDGET_H
