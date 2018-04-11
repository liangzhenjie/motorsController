#include "AbstractAutoRecongnize.h"
#include "mediator.h"
#include "EthernetAutoRecognize.h"
#include "SerialAutoRecognize.h"

AbstractAutoRecongnize *AbstractAutoRecongnize::getInstance(int nType)
{
    AbstractAutoRecongnize * pInstance = nullptr;
    if(nType == UserDefine::Via_Ethernet)
    {
        pInstance = new EthernetAutoRecognize();
    }
    else
    {
        pInstance = new SerialAutoRecognize();
    }
    return pInstance;
}

AbstractAutoRecongnize::AbstractAutoRecongnize(QObject *parent) : QObject(parent)
{

}

void AbstractAutoRecongnize::addMototInfo(quint8 nDeviceId, quint32 nDeviceMac)
{
    m_motorsInfo.insert(nDeviceId,nDeviceMac);
}
