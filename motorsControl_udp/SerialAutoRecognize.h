#ifndef SERIALAUTORECOGNIZE_H
#define SERIALAUTORECOGNIZE_H
#include "AbstractAutoRecongnize.h"
#include <QtSerialPort/QSerialPortInfo>

class SerialAutoRecognize : public AbstractAutoRecongnize
{
    Q_OBJECT
public:
    explicit SerialAutoRecognize(QObject *parent = 0);
    void startRecognize()override;
    void waitTimeout()override;
private:
    void findAvailablePorts();
private:
    QList <QSerialPortInfo> m_portList;
};

#endif // SERIALAUTORECOGNIZE_H
