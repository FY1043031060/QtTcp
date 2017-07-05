#ifndef WIDGET_H
#define WIDGET_H
#include <QMainWindow>
#include <QWidget>
#include <QTcpSocket> //ͨ���׽���
#include <QFile>      //�ļ�����
#include <QCamera>      //������ͷ�豸
#include <QCameraInfo>  //����ͷ�豸��Ϣ
#include <QCameraViewfinder>   //�û���ʾ�������Ƶ
#include <QCameraImageCapture> //��Ƶ��ͼ
#include <QMediaRecorder>      //¼����Ƶ
#include <qlist.h>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    
private slots:
    void on_buttonConnect_clicked();
    
    void on_buttonSend_clicked();
    
    void on_buttonClost_clicked();
    
private:
    Ui::Widget *ui;
    
    QTcpSocket *tcpSocket; //ͨ���׽���
    
    QFile file; //�ļ�����
    QString fileName; //�ļ�����
    qint64 fileSize;  //�ļ���С
    qint64 recvSize;  //�Ѿ������ļ���С
    
    bool isStart;
    
    QCamera *camera;
    QCameraImageCapture *imageCapture;
    QMediaRecorder* mediaRecorder;
    
};

#endif // WIDGET_H
