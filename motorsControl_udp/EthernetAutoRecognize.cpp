#include "EthernetAutoRecognize.h"
#include "innfosproxy.h"
#include <QTimer>
#include "mediator.h"
#include "userdefine.h"
#include <QThread>
#include <QDebug>
#include <QHostInfo>
//#ifdef linux
//#include <signal.h>
//#include <unistd.h>
//#include <time.h>
//#include <sys/time.h>
//#include <stdlib.h>
//#include <stdio.h>
//#endif //linux

EthernetAutoRecognize::EthernetAutoRecognize(QObject *parent) :
    AbstractAutoRecongnize(parent),
    m_bFindAvaliable(false),
    m_pSocket(nullptr)
{
    //findAvailablePorts();
    m_motorsInfo.clear();
    m_pSocket = new QUdpSocket(this);
    //QString localHost = QHostInfo::localHostName();
    //QString localHost = "192.168.1.100";
    //QHostAddress addr = QHostAddress(localHost);
    QHostAddress addr = QHostAddress::AnyIPv4;
    if(m_pSocket->bind(addr,2001,QAbstractSocket::ShareAddress))
    {
        qDebug() << tr("bind %1 successfully").arg(m_pSocket->localAddress().toString());
    }
    else {
        qDebug() << tr("bind %1 failed").arg(addr.toString());
    }
    connect(m_pSocket,&QUdpSocket::readyRead,this,&EthernetAutoRecognize::onIpBroadcast);
}

void EthernetAutoRecognize::findCommunicationUnits()
{
    qint64 nLen = m_pSocket->writeDatagram(InnfosProxy::getProxyContent(0,D_CAN_CONNECT),QHostAddress("192.168.1.255"),2000);
    qDebug() << "write" << nLen << InnfosProxy::getProxyContent(0,D_CAN_CONNECT).toHex();
    QTimer::singleShot(300,[=]{
        if(!m_bFindAvaliable)
        {
            //mediator->errorOccur(0,UserDefine::ERR_IP_ADDRESS_NOT_FOUND,"No available ip address!");
            waitTimeout();
            qDebug() << "failed";
        }
        else
        {
            m_pSocket->close();
            int i=1;
            foreach (QHostAddress addr, m_addrVector)
            {
                int nId = mediator->addCommunicationUnit(addr.toString(),2000+i);
                mediator->setUnitConnectionStatus(nId,UserDefine::CAN_CONNECTED);
                //InnfosProxy::SendProxy(0,D_CAN_CONNECT);               
                ++i;
            }
            InnfosProxy::SendProxy(0,D_READ_ADDRESS);
            QTimer::singleShot(800,[=]{waitTimeout();});
        }
    });
}

void EthernetAutoRecognize::startRecognize()
{
    //Communication::getInstance()->stop();
    m_motorsInfo.clear();
    m_addrVector.clear();
    findCommunicationUnits();

}

void EthernetAutoRecognize::waitTimeout()

{
    qDebug() << "wait timeout";
    mediator->recognizeFinished(m_motorsInfo);
    if(m_pSocket)
        m_pSocket->close();
    //autoDestroy();
}

void EthernetAutoRecognize::onIpBroadcast()
{
    QHostAddress addr;
    quint16 nPort;
    QByteArray datagram;
    while (m_pSocket->hasPendingDatagrams()) {
        datagram.resize(m_pSocket->pendingDatagramSize());
        m_pSocket->readDatagram(datagram.data(),datagram.size(),&addr,&nPort);
        qDebug() << "receive" << datagram.toHex() << addr.toString() << nPort;
        if((quint8)datagram.at(0)==0xee && (quint8)datagram.at(2) == D_CAN_CONNECT)
        {
            m_addrVector.push_back(addr);
            m_bFindAvaliable = true;
        }
    }

}

