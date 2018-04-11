#ifndef COMMUNICATION_H
#define COMMUNICATION_H
#include <QList>
#include <QMap>
#include <QObject>
#include <QUdpSocket>
#include "userdefine.h"

class CommunicateUnit;

class Communication : public QObject
{
    Q_OBJECT
public:
    explicit Communication(int nType=UserDefine::Via_Ethernet,QObject * parent = nullptr);
    //void sendDataBeforeDestroyed();
    ~ Communication();
    int addCommunication(const QString &str,const quint32 num);
    void sendData(quint8 nDeviceId,const QByteArray & data);
    bool hasDataWaitToSend();
    void stop();
    void recordRemainCmds();
    void addRelateIdToUnit(quint32 nUnitId,quint8 nRelateId);
    void removeUnAvailablePorts();
    void changeUnitRelateId(quint8 nOldId,quint8 nNewId);
    void setUnitConnectionStatus(quint32 nUnitId,quint8 nStatus);
public slots:
    void unitFinished(quint8 unitId);
protected:

signals:
    void response(quint32 unitId,const QByteArray response);
    void request(const QByteArray &request);
    void connectionError(quint8 unitId,quint16 errorId,QString errorStr);
private:
private:
    QList<CommunicateUnit *> m_lUnits;
    int m_nCommunicationType;
public:
};

#endif // COMMUNICATION_H
