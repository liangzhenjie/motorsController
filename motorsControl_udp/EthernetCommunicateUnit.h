#ifndef ETHERNETCOMMUNICATEUNIT_H
#define ETHERNETCOMMUNICATEUNIT_H
#include "communicateunit.h"
#include <QUdpSocket>

class EthernetCommunicateUnit : public CommunicateUnit
{
    Q_OBJECT
public:
    EthernetCommunicateUnit(quint32 unitId,const QString unitAddr,quint32 port,QObject *parent = 0);
    QString getCommunicationUnitName()override{return m_sUnitAddr;}
public slots:
    virtual void progress();
private:
    QString m_sUnitAddr;
    quint16 m_nPort;
};

#endif // ETHERNETCOMMUNICATEUNIT_H
