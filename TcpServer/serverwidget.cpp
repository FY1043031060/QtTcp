#include "serverwidget.h"
#include "ui_serverwidget.h"
#include<QFileDialog>         //选择文件对话框
#include<QDebug>              //输出
#include<QFileInfo>           //文件信息
ServerWidget::ServerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServerWidget)
{
    ui->setupUi(this);
    
    tcpServer = NULL;
    tcpSocket = NULL;
    
    //监听套接字
    tcpServer=new QTcpServer(this); 
    //监听
    tcpServer->listen(QHostAddress::Any,8888);
    
    setWindowTitle("服务器端口：8888");
    
    //选择文件 发送文件 禁用状态
    ui->buttonSend->setEnabled(false);       //禁用【发送文本】按钮
    ui->buttonFSend->setEnabled(false);      //禁用【发送文件】按钮
    ui->buttonMedia->setEnabled(false);      //禁用【查看视频】按钮
    ui->buttonMediaInfo->setEnabled(false);  //禁用【获取视频信息】按钮
    
    //如果客户端成功和服务器连接
    //tctServer会自动触发newConnection()
    connect(tcpServer,&QTcpServer::newConnection,
            [=]()
    {
        //取出建立好连接的套接字
        tcpSocket=tcpServer->nextPendingConnection();
        //获取对方IP和端口
        QString ip=tcpSocket->peerAddress().toString();   //IP地址 字符串
        quint16 port = tcpSocket->peerPort();             //端口 无符号整型
        QString temp=QString("[%1:%2] 成功连接").arg(ip).arg(port);
        ui->textEditRead->setText(temp);//显示到编辑区
        
        //成功连接后，才可以使用相应
        ui->buttonSend->setEnabled(true);       //可用【发送文本】按钮
        ui->buttonFSend->setEnabled(true);      //可用【发送文件】按钮
        ui->buttonMedia->setEnabled(true);      //可用【查看视频】按钮
        ui->buttonMediaInfo->setEnabled(true);  //可用【获取视频信息】按钮
        
        connect(tcpSocket,&QTcpSocket::readyRead,
                [=]()
        {
            //从通信套接字中取出内容
            QByteArray array = tcpSocket->readAll();
            QString temp =QString(array);
            ui->textEditRead->append(temp.right(temp.size()-4));

        }
        );
        
        connect(&timer,&QTimer::timeout,
                [=]()
        {
            //关闭定时器
            timer.stop();
            
            //发送文件
            sendData();
        }
        );
    }
    );
}

ServerWidget::~ServerWidget()
{
    tcpServer->close();
    tcpSocket->disconnectFromHost();
    tcpSocket->close();
    
    delete ui;
}

void ServerWidget::on_buttonSend_clicked()
{
    //获取编辑区内容
    QString str="01##"+ui->textEditWrite->toPlainText();
    //给对方发送数据，使用套接字tcpSocket
    qDebug()<<str.toUtf8().data();
    tcpSocket->write(str.toUtf8().data());   //发送文本信息
}

void ServerWidget::on_buttonMediaInfo_clicked() // 【获取视频信息】按钮
{
    //获取编辑区内容
    QString str="03##";                      //"03##"标记为获取视频信息
    //给对方发送数据，使用套接字tcpSocket
    qDebug()<<str.toUtf8().data();
    tcpSocket->write(str.toUtf8().data());   //发送文本信息
}

void ServerWidget::on_buttonClose_clicked()
{
    if(NULL == tcpSocket)return;
    //主动和客户端断开连接
    tcpSocket->disconnectFromHost();
    tcpSocket->close();
    tcpSocket = NULL;
    
    //断开连接 禁用相应按钮
    ui->buttonSend->setEnabled(false);       //禁用【发送文本】按钮
    ui->buttonFSend->setEnabled(false);      //禁用【发送文件】按钮
    ui->buttonMedia->setEnabled(false);      //禁用【查看视频】按钮
    ui->buttonMediaInfo->setEnabled(false);  //禁用【获取视频信息】按钮
}

//发送文件按钮
void ServerWidget::on_buttonFSend_clicked()
{
    QString filePath=QFileDialog::getOpenFileName(this,"open","../");
    if(false != filePath.isEmpty())
        ui->textEditRead->append("信息:选择文件路径出错!");
    else
    {
        fileName.clear();
        fileSize=0;
        //获取文件信息
        QFileInfo info(filePath);
        fileName=info.fileName(); //获取文件名字
        fileSize=info.size();     //获取文件大小
        
        sendSize=0; //发送文件大小
        
        //只读方式打开文件
        //指定文件的名字
        file.setFileName(filePath);
        //打开文件
        bool isOk=file.open(QIODevice::ReadOnly);
        if(false == isOk)
            qDebug()<<"只读文件打开失败";
        else 
        {
            qDebug()<<"只读文件打开成功";
            ui->textEditRead->append(filePath);
            ui->buttonFSend->setEnabled(true);
        }
        //先发送文件头信息  文件名##文件大小
        QString head=QString("02##%1##%2").arg(fileName).arg(fileSize);
        //发送头部信息
        qint64 len =tcpSocket->write(head.toUtf8());
        if(len<=0) //头部信息发送失败
            ui->textEditRead->append("信息:头部信息发送失败!");
        else
        {
            ui->textEditRead->append("信息:头部信息发送成功!");
            //发送真正文件信息
            //防止TCP粘包文件
            //需要通过定时器延时20ms
            timer.start(20);
        }
    }
}

//发送文件数据
void ServerWidget::sendData()
{
    qint64 len =0;
    do
    {
        // 每次发送数据的大小
        char buf[4*1024] ={0};
        len=0;
        
        //往文件中读数据
        len= file.read(buf,sizeof(buf));
        //发送数据，读多少，发多少
        len=tcpSocket->write(buf,len);
        
        //发送数据需要累加
        sendSize+=len;
        
    }while(len>0);
    
    //是否发送文件完毕
    if(sendSize == fileSize)
    {
        ui->textEditRead->append("信息:文件发送完毕!");
        file.close();
        timer.stop();
    }   
}


