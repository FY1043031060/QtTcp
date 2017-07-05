#include "widget.h"
#include "ui_widget.h"
#include<QHostAddress>
#include<QDebug>      //����
#include<QMessageBox> //�Ի���
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    
    tcpSocket=NULL;
    
    //����ռ䣬ָ��������
    tcpSocket=new QTcpSocket(this);
    
    setWindowTitle("�ͻ���");
    
    //����
    connect(tcpSocket,&QTcpSocket::connected,
            [=]()
    {
        ui->textEditRead->setText(QStringLiteral("��Ϣ:�ɹ��ͷ�������������"));
    }
    );
    
    //����
    connect(tcpSocket,&QTcpSocket::readyRead,
            [=]()
    {
        //��ȡ�Է����͵�����
        QByteArray buf=tcpSocket->readAll();
        //ui->textEditRead->append(buf); //׷�ӵ��༭����
        qDebug() << __FUNCTION__ << buf;
        QString temp=QString(buf);
        //QString str="01##hello##1024";
        //str.section("##",0,0);     //��"##"�ָȡ��0�ε���0�� Ҳ����"hello"
  
        //��0����Ԥ��: 01Ϊ�ı� 02Ϊ�ļ�ͷ 03Ϊ��Ƶ��Ϣ ����Ϊ�ļ�����
        if(temp.left(4)=="01##")  // ������ı���׷�ӵ��༭����
            ui->textEditRead->append(temp.right(temp.size()-4));
        //����ͷ 
        else if(temp.left(4)=="02##") //�����ͷ�ļ�������ͷ����Ϣ
        { 
            //��ʼ��
            fileName=temp.section("##",1,1);
            fileSize=temp.section("##",2,2).toInt();
            ui->textEditRead->append("fileName:"+fileName);
            ui->textEditRead->append("fileSize:"+fileSize);
            ui->progressBar->setMaximum(fileSize);
            recvSize=0;
            file.setFileName(fileName);
            //���ļ�
            bool isOk=file.open(QIODevice::WriteOnly);
            if(false == isOk)
                qDebug()<<"�򿪳���";
            qDebug()<<"�򿪳ɹ�";
        }
        else if(temp.left(4)=="03##") //����ǻ�ȡ��Ƶ��Ϣ�����ȡ
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
        { //�ļ���Ϣ
            qint64 len =file.write(buf);
            qDebug()<<len;
            recvSize+=len;
            ui->progressBar->setValue(recvSize);
            if(recvSize==fileSize)
            {
                file.close();
                QMessageBox::information(this,"���","�ļ��������");
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
    //��ȡ������IP�Ͷ˿�
    QString ip= ui->lineEditIP->text();
    quint16 port=ui->lineEditPort->text().toInt();
    
    //�����ͷ�������������
    tcpSocket->connectToHost(QHostAddress(ip),port);
}

void Widget::on_buttonSend_clicked()
{
    if(NULL == tcpSocket)return;
    //��ȡ�༭������
    QString str= "01##"+ui->textEditWrite->toPlainText();
    //��������
    tcpSocket->write(str.toUtf8().data());
    
}

void Widget::on_buttonClost_clicked()
{
    if(NULL == tcpSocket)return;
    //�����ͶԷ��Ͽ�����
    tcpSocket->disconnectFromHost();
    tcpSocket->close();
}
