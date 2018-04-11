#ifndef SERIALCOMMUNICATEUNIT_H
#define SERIALCOMMUNICATEUNIT_H
#include "communicateunit.h"

class SerialCommunicateUnit : public CommunicateUnit
{
    Q_OBJECT
public:
    SerialCommunicateUnit(quint32 unitId,const QString portName,quint32 bauRate,QObject *parent = 0);
    QString getCommunicationUnitName()override{return m_sPortName;}
public slots:
    virtual void progress();
private:
    QString m_sPortName;
    quint32 m_nBauRate;
};

#endif // SERIALCOMMUNICATEUNIT_H
