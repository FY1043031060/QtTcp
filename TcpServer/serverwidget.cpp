#include "serverwidget.h"
#include "ui_serverwidget.h"
#include<QFileDialog>         //ѡ���ļ��Ի���
#include<QDebug>              //���
#include<QFileInfo>           //�ļ���Ϣ
ServerWidget::ServerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServerWidget)
{
    ui->setupUi(this);
    
    tcpServer = NULL;
    tcpSocket = NULL;
    
    //�����׽���
    tcpServer=new QTcpServer(this); 
    //����
    tcpServer->listen(QHostAddress::Any,8888);
    
    setWindowTitle("�������˿ڣ�8888");
    
    //ѡ���ļ� �����ļ� ����״̬
    ui->buttonSend->setEnabled(false);       //���á������ı�����ť
    ui->buttonFSend->setEnabled(false);      //���á������ļ�����ť
    ui->buttonMedia->setEnabled(false);      //���á��鿴��Ƶ����ť
    ui->buttonMediaInfo->setEnabled(false);  //���á���ȡ��Ƶ��Ϣ����ť
    
    //����ͻ��˳ɹ��ͷ���������
    //tctServer���Զ�����newConnection()
    connect(tcpServer,&QTcpServer::newConnection,
            [=]()
    {
        //ȡ�����������ӵ��׽���
        tcpSocket=tcpServer->nextPendingConnection();
        //��ȡ�Է�IP�Ͷ˿�
        QString ip=tcpSocket->peerAddress().toString();   //IP��ַ �ַ���
        quint16 port = tcpSocket->peerPort();             //�˿� �޷�������
        QString temp=QString("[%1:%2] �ɹ�����").arg(ip).arg(port);
        ui->textEditRead->setText(temp);//��ʾ���༭��
        
        //�ɹ����Ӻ󣬲ſ���ʹ����Ӧ
        ui->buttonSend->setEnabled(true);       //���á������ı�����ť
        ui->buttonFSend->setEnabled(true);      //���á������ļ�����ť
        ui->buttonMedia->setEnabled(true);      //���á��鿴��Ƶ����ť
        ui->buttonMediaInfo->setEnabled(true);  //���á���ȡ��Ƶ��Ϣ����ť
        
        connect(tcpSocket,&QTcpSocket::readyRead,
                [=]()
        {
            //��ͨ���׽�����ȡ������
            QByteArray array = tcpSocket->readAll();
            QString temp =QString(array);
            ui->textEditRead->append(temp.right(temp.size()-4));

        }
        );
        
        connect(&timer,&QTimer::timeout,
                [=]()
        {
            //�رն�ʱ��
            timer.stop();
            
            //�����ļ�
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
    //��ȡ�༭������
    QString str="01##"+ui->textEditWrite->toPlainText();
    //���Է��������ݣ�ʹ���׽���tcpSocket
    qDebug()<<str.toUtf8().data();
    tcpSocket->write(str.toUtf8().data());   //�����ı���Ϣ
}

void ServerWidget::on_buttonMediaInfo_clicked() // ����ȡ��Ƶ��Ϣ����ť
{
    //��ȡ�༭������
    QString str="03##";                      //"03##"���Ϊ��ȡ��Ƶ��Ϣ
    //���Է��������ݣ�ʹ���׽���tcpSocket
    qDebug()<<str.toUtf8().data();
    tcpSocket->write(str.toUtf8().data());   //�����ı���Ϣ
}

void ServerWidget::on_buttonClose_clicked()
{
    if(NULL == tcpSocket)return;
    //�����Ϳͻ��˶Ͽ�����
    tcpSocket->disconnectFromHost();
    tcpSocket->close();
    tcpSocket = NULL;
    
    //�Ͽ����� ������Ӧ��ť
    ui->buttonSend->setEnabled(false);       //���á������ı�����ť
    ui->buttonFSend->setEnabled(false);      //���á������ļ�����ť
    ui->buttonMedia->setEnabled(false);      //���á��鿴��Ƶ����ť
    ui->buttonMediaInfo->setEnabled(false);  //���á���ȡ��Ƶ��Ϣ����ť
}

//�����ļ���ť
void ServerWidget::on_buttonFSend_clicked()
{
    QString filePath=QFileDialog::getOpenFileName(this,"open","../");
    if(false != filePath.isEmpty())
        ui->textEditRead->append("��Ϣ:ѡ���ļ�·������!");
    else
    {
        fileName.clear();
        fileSize=0;
        //��ȡ�ļ���Ϣ
        QFileInfo info(filePath);
        fileName=info.fileName(); //��ȡ�ļ�����
        fileSize=info.size();     //��ȡ�ļ���С
        
        sendSize=0; //�����ļ���С
        
        //ֻ����ʽ���ļ�
        //ָ���ļ�������
        file.setFileName(filePath);
        //���ļ�
        bool isOk=file.open(QIODevice::ReadOnly);
        if(false == isOk)
            qDebug()<<"ֻ���ļ���ʧ��";
        else 
        {
            qDebug()<<"ֻ���ļ��򿪳ɹ�";
            ui->textEditRead->append(filePath);
            ui->buttonFSend->setEnabled(true);
        }
        //�ȷ����ļ�ͷ��Ϣ  �ļ���##�ļ���С
        QString head=QString("02##%1##%2").arg(fileName).arg(fileSize);
        //����ͷ����Ϣ
        qint64 len =tcpSocket->write(head.toUtf8());
        if(len<=0) //ͷ����Ϣ����ʧ��
            ui->textEditRead->append("��Ϣ:ͷ����Ϣ����ʧ��!");
        else
        {
            ui->textEditRead->append("��Ϣ:ͷ����Ϣ���ͳɹ�!");
            //���������ļ���Ϣ
            //��ֹTCPճ���ļ�
            //��Ҫͨ����ʱ����ʱ20ms
            timer.start(20);
        }
    }
}

//�����ļ�����
void ServerWidget::sendData()
{
    qint64 len =0;
    do
    {
        // ÿ�η������ݵĴ�С
        char buf[4*1024] ={0};
        len=0;
        
        //���ļ��ж�����
        len= file.read(buf,sizeof(buf));
        //�������ݣ������٣�������
        len=tcpSocket->write(buf,len);
        
        //����������Ҫ�ۼ�
        sendSize+=len;
        
    }while(len>0);
    
    //�Ƿ����ļ����
    if(sendSize == fileSize)
    {
        ui->textEditRead->append("��Ϣ:�ļ��������!");
        file.close();
        timer.stop();
    }   
}


