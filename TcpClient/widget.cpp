#include "widget.h"
#include "ui_widget.h"
#include<QHostAddress>
#include<QDebug>      //调试
#include<QMessageBox> //对话框
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    
    tcpSocket=NULL;
    
    //分配空间，指定父对象
    tcpSocket=new QTcpSocket(this);
    
    setWindowTitle("客户端");
    
    //连接
    connect(tcpSocket,&QTcpSocket::connected,
            [=]()
    {
        ui->textEditRead->setText(QStringLiteral("信息:成功和服务器建立连接"));
    }
    );
    
    //接收
    connect(tcpSocket,&QTcpSocket::readyRead,
            [=]()
    {
        //获取对方发送的内容
        QByteArray buf=tcpSocket->readAll();
        //ui->textEditRead->append(buf); //追加到编辑区中
        qDebug() << __FUNCTION__ << buf;
        QString temp=QString(buf);
        //QString str="01##hello##1024";
        //str.section("##",0,0);     //用"##"分割，取第0段到第0段 也就是"hello"
  
        //第0部分预设: 01为文本 02为文件头 03为视频信息 其它为文件数据
        if(temp.left(4)=="01##")  // 如果是文本，追加到编辑区中
            ui->textEditRead->append(temp.right(temp.size()-4));
        //接收头 
        else if(temp.left(4)=="02##") //如果是头文件，解析头部信息
        { 
            //初始化
            fileName=temp.section("##",1,1);
            fileSize=temp.section("##",2,2).toInt();
            ui->textEditRead->append("fileName:"+fileName);
            ui->textEditRead->append("fileSize:"+fileSize);
            ui->progressBar->setMaximum(fileSize);
            recvSize=0;
            file.setFileName(fileName);
            //打开文件
            bool isOk=file.open(QIODevice::WriteOnly);
            if(false == isOk)
                qDebug()<<"打开出错";
            qDebug()<<"打开成功";
        }
        else if(temp.left(4)=="03##") //如果是获取视频信息，则获取
        {
            QList<QCameraInfo> list = QCameraInfo::availableCameras();
            
            //========================================================
        
            //Camera devices:
//            QActionGroup *videoDevicesGroup = new QActionGroup(this);
//            videoDevicesGroup->setExclusive(true);
//            foreach (const QCameraInfo &cameraInfo, QCameraInfo::availableCameras()) {
//                QAction *videoDeviceAction = new QAction(cameraInfo.description(), videoDevicesGroup);
//                videoDeviceAction->setCheckable(true);
//                videoDeviceAction->setData(QVariant::fromValue(cameraInfo));
//                if (cameraInfo == QCameraInfo::defaultCamera())
//                    videoDeviceAction->setChecked(true);
        
//                ui->menuDevices->addAction(videoDeviceAction);
//            }
//            connect(videoDevicesGroup, SIGNAL(triggered(QAction*)), SLOT(updateCameraDevice(QAction*)));
//            connect(ui->captureWidget, SIGNAL(currentChanged(int)), SLOT(updateCaptureMode()));
            
//            setCamera(QCameraInfo::defaultCamera());
            //========================================================
        }
        else
        { //文件信息
            qint64 len =file.write(buf);
            qDebug()<<len;
            recvSize+=len;
            ui->progressBar->setValue(recvSize);
            if(recvSize==fileSize)
            {
                file.close();
                QMessageBox::information(this,"完成","文件接收完成");
            } 
        }
    }
    );
}

Widget::~Widget()
{
    tcpSocket->disconnectFromHost();
    tcpSocket->close();
    
    delete ui;
}

void Widget::on_buttonConnect_clicked()
{
    //获取服务器IP和端口
    QString ip= ui->lineEditIP->text();
    quint16 port=ui->lineEditPort->text().toInt();
    
    //主动和服务器建立连接
    tcpSocket->connectToHost(QHostAddress(ip),port);
}

void Widget::on_buttonSend_clicked()
{
    if(NULL == tcpSocket)return;
    //获取编辑框内容
    QString str= "01##"+ui->textEditWrite->toPlainText();
    //发送数据
    tcpSocket->write(str.toUtf8().data());
    
}

void Widget::on_buttonClost_clicked()
{
    if(NULL == tcpSocket)return;
    //主动和对方断开连接
    tcpSocket->disconnectFromHost();
    tcpSocket->close();
}
