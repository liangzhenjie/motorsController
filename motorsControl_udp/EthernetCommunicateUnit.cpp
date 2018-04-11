#include "EthernetCommunicateUnit.h"
#include "innfosproxy.h"
#include <QDebug>
#include "userdefine.h"
#include <QThread>
#include <QHostInfo>

EthernetCommunicateUnit::EthernetCommunicateUnit(quint32 unitId, const QString unitAddr, quint32 port, QObject *parent):
    CommunicateUnit(unitId,parent),
    m_sUnitAddr(unitAddr),
    m_nPort(port)
{

}

void EthernetCommunicateUnit::progress()
{
    QUdpSocket socket;
    QString localHost = QHostInfo::localHostName();
    //QString localHost = "192.168.1.100";
    if(!socket.bind(QHostAddress(localHost),m_nPort,QAbstractSocket::ShareAddress))
    {
        emit error(tr("bind host %1 port %2 failed!").arg(localHost).arg(m_nPort));
        qDebug() << socket.error() << socket.errorString() << localHost << m_nPort;
        return;
    }
    else {
        qDebug() << "bind" << localHost;
    }
    QByteArray sendData;
    bool bHasResponse = false;
    while (!m_bStop)
    {
        while(socket.hasPendingDatagrams())
        {
            QByteArray readData;

            readData.resize(socket.pendingDatagramSize());
            socket.readDatagram(readData.data(),readData.size());
            emit response(m_nUnitId,readData);
            qDebug()<<"readProxy"<<readData.toHex()<<QTime::currentTime().toString("hh:mm:ss.zzz")<<m_nUnitId;
            bHasResponse = true;
        }
        //timer.restart();
        if(sendData.size() > 0)
        {
            int nLen = socket.writeDatagram(sendData,QHostAddress(m_sUnitAddr),2000);
            if(nLen == sendData.size())
            {
                qDebug()<<"SendProxy"<<nLen<<sendData.toHex()<<QTime::currentTime().toString("hh:mm:ss.zzz")<<m_nUnitId << "new";
                //emit response(m_nUnitId,sendData);
                sendData.clear();
            }
            quint8 proxyId = sendData[2];
            if(proxyId != D_SET_POSITION && proxyId != D_READ_CUR_CURRENT && proxyId != D_READ_CUR_VELOCITY && proxyId != D_READ_CUR_POSITION)
                bHasResponse = false;
            else
                bHasResponse = true;
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
        if(!bHasResponse)
        {
            QThread::usleep(600);
        }
        else
        {
            QThread::usleep(50);
        }
        bHasResponse = true;
    }
    socket.close();
    emit finished(m_nUnitId);
}
