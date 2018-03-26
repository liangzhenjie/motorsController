#include "autorecoginze.h"
#include "communication.h"
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

AutoRecognize * AutoRecognize::m_pAutoRecognize = nullptr;
AutoRecognize::AutoRecognize(QObject *parent) :
    QObject(parent),
    m_bFindAvaliable(false),
    m_bTryNext(true),
    m_pSocket(nullptr)
{
    //findAvailablePorts();
    m_motorsInfo.clear();
    m_pSocket = new QUdpSocket(this);
    QString localHost = QHostInfo::localHostName();
    //QString localHost = "192.168.1.100";
    QHostAddress addr = QHostAddress(localHost);
    if(m_pSocket->bind(addr,2001,QAbstractSocket::ShareAddress))
    {

        qDebug() << tr("bind %1 successfully").arg(addr.toString());
    }
    else {
        qDebug() << tr("bind %1 failed").arg(addr.toString());
    }
    connect(m_pSocket,&QUdpSocket::readyRead,this,&AutoRecognize::onIpBroadcast);
}

void AutoRecognize::findCommunicationUnits()
{
    qint64 nLen = m_pSocket->writeDatagram(InnfosProxy::getProxyContent(0,D_CAN_CONNECT),QHostAddress::Broadcast,2000);
    qDebug() << "write" << nLen << InnfosProxy::getProxyContent(0,D_CAN_CONNECT).toHex();
    QTimer::singleShot(300,[=]
    {
        if(!m_bFindAvaliable)
        {
            //mediator->errorOccur(0,UserDefine::ERR_IP_ADDRESS_NOT_FOUND,"No available ip address!");
        }
        else
        {
            m_pSocket->close();
            int i=1;
            foreach (QHostAddress addr, m_addrVector)
            {
                Communication::getInstance()->addCommunication(addr.toString(),2000+i);
                //InnfosProxy::SendProxy(0,D_CAN_CONNECT);
                InnfosProxy::SendProxy(0,D_READ_ADDRESS);
                ++i;
            }
            QTimer::singleShot(800,this,SLOT(waitTimeout()));
        }
    });
}

AutoRecognize *AutoRecognize::getInstance()
{
    if (!m_pAutoRecognize)
    {
        m_pAutoRecognize = new AutoRecognize;
        //connect(Communication::getInstance(),&Communication::OpenPort,m_pAutoRecognize,&AutoRecognize::onPortOpen);//todo
    }

    return m_pAutoRecognize;
}

void AutoRecognize::startRecognize(bool bRetry)
{
    if(bRetry)
    {
        Communication::getInstance()->stop();
        m_motorsInfo.clear();
        m_addrVector.clear();
    }
    findCommunicationUnits();
//    Communication::getInstance()->addCommunication("192.168.1.2",2000);
//    InnfosProxy::SendProxy(0,D_CAN_CONNECT);
//    InnfosProxy::SendProxy(0,D_READ_ADDRESS);
}

void AutoRecognize::addMototInfo(quint8 nDeviceId, quint32 nDeviceMac)
{
    m_motorsInfo.insert(nDeviceId,nDeviceMac);
}

void AutoRecognize::openFailed()
{
    m_bTryNext = true;
}

void AutoRecognize::waitTimeout()

{
    qDebug() << "wait timeout";
    mediator->recognizeFinished(m_motorsInfo);
    if(m_pSocket)
        m_pSocket->close();
    //autoDestroy();
}

void AutoRecognize::onIpBroadcast()
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

