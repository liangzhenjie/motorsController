#include "SerialCommunicateUnit.h"
#include <QSerialPort>
#include "innfosproxy.h"
#include <QDebug>
#include "userdefine.h"
#include <QThread>


SerialCommunicateUnit::SerialCommunicateUnit(quint32 unitId, const QString portName, quint32 bauRate, QObject *parent):
    CommunicateUnit(unitId,parent),
    m_sPortName(portName),
    m_nBauRate(bauRate)
{

}


void SerialCommunicateUnit::progress()
{
    QSerialPort serial(m_sPortName);
    serial.setBaudRate(m_nBauRate);
    if(!serial.open(QIODevice::ReadWrite))
    {
        emit error(tr("open %1 failed!").arg(m_sPortName));
        return;
    }
    quint8 nWaitTime = 1;
    int nFailCnt = 0;
    QByteArray sendData;

    while (!m_bStop)
    {
        if(serial.waitForReadyRead(nWaitTime))
        {
            QByteArray readData = serial.readAll();
            if(serial.error() == QSerialPort::ReadError)
            {
                serial.clearError();
            }
            emit response(m_nUnitId,readData);
            qDebug()<<"readProxy"<<readData.toHex()<<QTime::currentTime().toString("hh:mm:ss.zzz")<<m_nUnitId<<"serial";

        }
        //timer.restart();
        if(sendData.size() > 0 && serial.isWritable())
        {
            int nLen = serial.write(sendData);
            if(serial.waitForBytesWritten(30))
            {
                qDebug()<<"SendProxy"<<nLen<<sendData.toHex()<<QTime::currentTime().toString("hh:mm:ss.zzz")<<m_nUnitId<<"serial"<<m_nBauRate;
                sendData.clear();
            }
            else {

                if(++nFailCnt > 2)
                {
                    serial.clearError();
                    sendData.clear();
                    nFailCnt = 0;
                }
            }
        }

        m_qmMutex.lock();
        if(sendData.size()==0)
        {
#ifdef USING_VECTOR
            while (m_dataVector.size() > 0)
            {
                QByteArray data = m_dataVector.at(0);
                sendData.append(data);
                m_dataVector.pop_front();
                quint8 proxyId = data[2];
                if(proxyId != D_SET_POSITION && proxyId != D_READ_CUR_CURRENT && proxyId != D_READ_CUR_VELOCITY && proxyId != D_READ_CUR_POSITION)//只有部分指令才能多条同时发
                    break;
            }
#else
            QMapIterator<quint8,QVector<QByteArray>> it(m_dataMap);
            while (it.hasNext())
            {
                it.next();
                if(it.value().size()>0)
                {
                    QByteArray data = it.value().front();
                    sendData.append(data);
                    m_dataMap[it.key()].pop_front();
                    quint8 proxyId = data[2];
                    if(proxyId != D_SET_POSITION && proxyId != D_READ_CUR_CURRENT && proxyId != D_READ_CUR_VELOCITY && proxyId != D_READ_CUR_POSITION)//只有部分指令才能多条同时发
                        break;
                }
            }
#endif
        }
        m_qmMutex.unlock();
    }
    serial.close();
    emit finished(m_nUnitId);
}
