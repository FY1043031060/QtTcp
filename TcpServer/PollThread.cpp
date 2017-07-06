#include "PollThread.h"
PollThread::PollThread(QObject *parent) : QObject(parent)
{

}

void PollThread::processWork()
{
    emit m_tcpThread.dataSend(QByteArray(5,'a'));
}
