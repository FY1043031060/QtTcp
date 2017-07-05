#ifndef WIDGET_H
#define WIDGET_H
#include <QMainWindow>
#include <QWidget>
#include <QTcpSocket> //通信套接字
#include <QFile>      //文件对象
#include <QCamera>      //打开摄像头设备
#include <QCameraInfo>  //摄像头设备信息
#include <QCameraViewfinder>   //用户显示捕获的视频
#include <QCameraImageCapture> //视频截图
#include <QMediaRecorder>      //录制视频
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
    
    QTcpSocket *tcpSocket; //通信套接字
    
    QFile file; //文件对象
    QString fileName; //文件名字
    qint64 fileSize;  //文件大小
    qint64 recvSize;  //已经接收文件大小
    
    bool isStart;
    
    QCamera *camera;
    QCameraImageCapture *imageCapture;
    QMediaRecorder* mediaRecorder;
    
};

#endif // WIDGET_H
