#ifndef ABSTRACTAUTORECONGNIZE_H
#define ABSTRACTAUTORECONGNIZE_H
#include <QObject>
#include <QMap>

class AbstractAutoRecongnize : public QObject
{
    Q_OBJECT
public:
    static AbstractAutoRecongnize * getInstance(int nType);
    explicit AbstractAutoRecongnize(QObject *parent = 0);
    virtual void startRecognize()=0;
    virtual void waitTimeout()=0;
public:
    void addMototInfo(quint8 nDeviceId,quint32 nDeviceMac);
    QMap<quint8,quint32> getMotorsInfo()const{return m_motorsInfo;}
signals:

public slots:

protected:
    QMap<quint8,quint32> m_motorsInfo;
};

#endif // ABSTRACTAUTORECONGNIZE_H
