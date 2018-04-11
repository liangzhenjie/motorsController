#ifndef COMMUNICATEUNIT_H
#define COMMUNICATEUNIT_H

#include <QObject>
#include <QMutex>
#include <QMap>
#include <QVector>


class CommunicateUnit : public QObject
{
    Q_OBJECT
public:
    explicit CommunicateUnit(quint32 unitId,QObject *parent = 0);

signals:
    void response(quint32 unitId,QByteArray responseData);
    void error(QString errorString);
    void finished(quint32 unitId);
public:
    void addRelateId(quint8 id);
    void switchOldRelateIdToNewRelateId(quint8 oldId,quint8 newId);
    void setRelateIdList(const QList<quint8> &idList);
    bool isContainsRelateId(const quint8 id)const;
    quint32 getUnitId()const{
        return m_nUnitId;
    }
    void sendData(const QByteArray & sendData);
    void stopCommunication();
    bool hasDataWaiting();
    bool isAvailable()const;
    void setConnectionStatus(quint8 nStatus);
    quint8 getConnectionStatus()const;
    virtual QString getCommunicationUnitName()=0;
public slots:
    virtual void progress()=0;
protected:

    QMap<quint8,QVector <QByteArray>> m_dataMap;
    QVector<QByteArray> m_dataVector;
    QMutex m_qmMutex;
    bool m_bStop;
    quint32 m_nUnitId;
    quint8 m_nConnectionStatus;
    QList<quint8> m_relateIdList;//motors's ids whitch communicate via this unit
    //QHostAddress m_unitAddr;


};

#endif // COMMUNICATEUNIT_H
