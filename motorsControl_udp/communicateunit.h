#ifndef COMMUNICATEUNIT_H
#define COMMUNICATEUNIT_H

#include <QObject>
#include <QMutex>
#include <QMap>
#include <QVector>
#include <QUdpSocket>

class CommunicateUnit : public QObject
{
    Q_OBJECT
public:
    explicit CommunicateUnit(quint32 unitId,const QString unitAddr,quint32 port,QObject *parent = 0);

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
    QString getCommunicationUnitName()const{return m_sUnitAddr;}
public slots:
    void progress();
private:
    quint32 m_nUnitId;
    //QHostAddress m_unitAddr;
    QString m_sUnitAddr;
    quint16 m_nPort;
    QList<quint8> m_relateIdList;//motors's ids whitch communicate via this unit
    QMap<quint8,QVector <QByteArray>> m_dataMap;
    QVector<QByteArray> m_dataVector;
    QMutex m_qmMutex;
    bool m_bStop;
    quint8 m_nConnectionStatus;
};

#endif // COMMUNICATEUNIT_H
