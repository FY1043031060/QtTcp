#include <QCoreApplication>
#include <PollThread.h>
#include <QTimer>
#include <QThread>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QThread th;

    PollThread s;
    s.moveToThread(&th);

    QTimer m_timer;
    QObject::connect(&m_timer,&QTimer::timeout,&s,&PollThread::processWork);
    m_timer.start(20);

    return a.exec();
}
