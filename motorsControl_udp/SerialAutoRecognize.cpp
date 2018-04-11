#include "SerialAutoRecognize.h"
#include "mediator.h"
#include "innfosproxy.h"

SerialAutoRecognize::SerialAutoRecognize(QObject *parent):
    AbstractAutoRecongnize(parent)
{
    m_motorsInfo.clear();
    findAvailablePorts();
}

void SerialAutoRecognize::startRecognize()
{
    m_motorsInfo.clear();
    findAvailablePorts();

    for(int i=0;i<m_portList.size();++i)
    {
        QSerialPortInfo info = m_portList.at(i);
        mediator->addCommunicationUnit(info.portName(),1500000);
        //Communication::getInstance()->addCommunication(info.portName(),1500000);
    }

    InnfosProxy::SendProxy(0,D_CAN_CONNECT);
    InnfosProxy::SendProxy(0,D_READ_ADDRESS);
    QTimer::singleShot(800,[=]{waitTimeout();});
}

void SerialAutoRecognize::waitTimeout()
{
    mediator->recognizeFinished(m_motorsInfo);
}

void SerialAutoRecognize::findAvailablePorts()
{
    m_portList.clear();
    QList<QSerialPortInfo> allportList = QSerialPortInfo::availablePorts();
    foreach (QSerialPortInfo info, allportList) {
        QString manufacturer = info.manufacturer();
        if(manufacturer == "FTDI")//filter some serialport that is not FTDI
            m_portList.append(info);
    }
}
