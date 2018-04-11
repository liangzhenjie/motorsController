#include "communicateunit.h"
#include <QMap>
#include <QVector>
#include "innfosproxy.h"
#include <QDebug>
#include "userdefine.h"
#include <QThread>
#include <QHostInfo>

CommunicateUnit::CommunicateUnit(quint32 unitId, QObject *parent) :
    QObject(parent),
    m_nUnitId(unitId),
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

