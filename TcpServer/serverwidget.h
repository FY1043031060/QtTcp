#ifndef SERVERWIDGET_H
#define SERVERWIDGET_H

#include <QWidget>

//服务端需要两个套接字 客户端需要一个
#include<QTcpServer> //监听套接字
#include<QTcpSocket> //通信套接字
#include<QFile>      //文件对象
#include<QTimer>      //定时器
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
    
    void sendData();  //发送文件数据
    
private slots:
    void on_buttonSend_clicked();
    void on_buttonClose_clicked();
    void on_buttonFSend_clicked();
    void on_buttonMediaInfo_clicked();
    
private:
    Ui::ServerWidget *ui;
    
    QTcpServer *tcpServer; //监听套接字
    QTcpSocket *tcpSocket; //通信套接字
    
    QFile file; //文件对象
    QString fileName; //文件名字
    qint64 fileSize;  //文件大小
    qint64 sendSize;  //已经发送文件大小
    
    QTimer timer;       //定时器
    QThread m_thread;   //工作线程
};

#endif // SERVERWIDGET_H
