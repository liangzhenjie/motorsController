#include "communicateunit.h"
#include <QMap>
#include <QVector>
#include "innfosproxy.h"
#include <QDebug>
#include "userdefine.h"
#include <QThread>
#include <QHostInfo>

CommunicateUnit::CommunicateUnit(quint32 unitId, const QString unitAddr, quint32 port, QObject *parent) :
    QObject(parent),
    m_nUnitId(unitId),
    m_sUnitAddr(unitAddr),
    m_nPort(port),
    m_bStop(false),
    m_nConnectionStatus(UserDefine::NO_CONNECT)
{
    
}

void CommunicateUnit::addRelateId(quint8 id)
{
    qDebug() << "Add start" << id << m_nUnitId;
    if(!isContainsRelateId(id))
    {
        qDebug() << "Add" << id << m_nUnitId;
        m_relateIdList.append(id);
    }

}

void CommunicateUnit::switchOldRelateIdToNewRelateId(quint8 oldId, quint8 newId)
{
    for(int i=0;i<m_relateIdList.size();++i)
    {
        if(m_relateIdList[i] == oldId)
        {
            m_relateIdList.removeAt(i);
            break;
        }
    }
    m_relateIdList.append(newId);
}

void CommunicateUnit::setRelateIdList(const QList<quint8> &idList)
{
    m_relateIdList.clear();
    m_relateIdList = idList;
}

bool CommunicateUnit::isContainsRelateId(const quint8 id) const
{
    if(m_relateIdList.contains(id))
        return true;
    return false;
}

void CommunicateUnit::sendData(const QByteArray &sendData)
{
    QMutexLocker locker(&m_qmMutex);
#ifdef USING_VECTOR
    m_dataVector.push_back(sendData);
#else
    QList<quint8> keys = m_dataMap.uniqueKeys();
    quint8 nDeviceId = sendData[1];
    if(keys.contains(nDeviceId))
    {
        m_dataMap[nDeviceId].push_back(sendData);
    }
    else
    {
        QVector<QByteArray> array;
        array.push_back(sendData);
        m_dataMap.insert(nDeviceId,array);
    }
#endif
}

void CommunicateUnit::stopCommunication()
{
    QMutexLocker locker(&m_qmMutex);
    m_bStop = true;
}

bool CommunicateUnit::hasDataWaiting()
{
    QMutexLocker locker(&m_qmMutex);
#ifdef USING_VECTOR
    return m_dataVector.size() > 0;
#else
    QMapIterator<quint8,QVector<QByteArray>> it(m_dataMap);
    while (it.hasNext())
    {
        it.next();
        if(it.value().size()>0)
            return true;
    }
#endif
    return false;
}

bool CommunicateUnit::isAvailable() const
{
    return m_relateIdList.size() > 0;
}

void CommunicateUnit::setConnectionStatus(quint8 nStatus)
{
    m_nConnectionStatus += nStatus;
}

quint8 CommunicateUnit::getConnectionStatus() const
{
    return m_nConnectionStatus;
}


void CommunicateUnit::progress()
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
