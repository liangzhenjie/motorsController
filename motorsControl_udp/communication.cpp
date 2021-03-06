﻿#include "communication.h"
#include <QtSerialPort/QSerialPort>
#include <QIODevice>
#include <QDebug>
#include <QTime>
#include <QElapsedTimer>
#include <QFile>
#include <QTextStream>
#include <chrono>
#include <thread>
#include "innfosproxy.h"
#include <QThread>
#include <QRegExp>
#include "communicateunit.h"
#include "userdefine.h"
#include "EthernetCommunicateUnit.h"
#include "SerialCommunicateUnit.h"

Communication::Communication(int nType, QObject *parent):
    QObject(parent),
    m_nCommunicationType(nType)
{

}

//void Communication::sendDataBeforeDestroyed()
//{
//    if(m_pCommucation)
//    {
//#ifdef RELEASE_LOG
//    m_pCommucation->recordRemainCmds();
//#endif
//        while (true)
//        {
//            if(!m_pCommucation->hasDataWaitToSend())
//            {
//                //delete m_pCommucation;
//                break;
//            }
//            QThread::msleep(1);
//        }

//    }
//    //_pCommucation = nullptr;
//}

Communication::~Communication()
{
    stop();
}


int Communication::addCommunication(const QString &str, const quint32 num)
{
//    QRegExp rx("\\d+");
//    int pos = rx.indexIn(addr);
    if(/*pos > -1*/true)
    {
        //quint8 nPortNum = rx.cap(0).toUInt();
        CommunicateUnit * pUnit = nullptr;
        if(m_nCommunicationType == UserDefine::Via_Ethernet)
        {
            pUnit = new EthernetCommunicateUnit(num,str,num);//id与端口一致，现在端口都不一样
        }
        else
        {
            QRegExp rx("\\d+");
            int pos = rx.indexIn(str);
            if(pos > -1)
            {
                quint8 nPortNum = rx.cap(0).toUInt();
                pUnit = new SerialCommunicateUnit(nPortNum,str,num);
            }
        }

        if(pUnit != nullptr)
        {
            QThread * pThread = new QThread();
            pUnit->moveToThread(pThread);
            m_lUnits.append(pUnit);
            connect(pUnit,&CommunicateUnit::finished,pUnit,&CommunicateUnit::deleteLater);
            connect(pUnit,&CommunicateUnit::finished,pThread,&QThread::quit);
            connect(pThread,&QThread::finished,pThread,&QThread::deleteLater);

            connect(pUnit,&CommunicateUnit::response,this,&Communication::response);
            connect(pThread,&QThread::started,pUnit,&CommunicateUnit::progress);

            pThread->start();
            return pUnit->getUnitId();
        }

    }
    else
    {
        qDebug() << "portName error! " << str;
    }
    return -1;
}

void Communication::sendData(quint8 nDeviceId, const QByteArray &data)
{
    if(nDeviceId == 0)//send to all
    {
        for(int i=0;i<m_lUnits.size();++i)
        {
            m_lUnits[i]->sendData(data);
        }
    }
    else
    {
        for(int i=0;i<m_lUnits.size();++i)
        {
            if(m_lUnits[i]->isContainsRelateId(nDeviceId))
            {
                m_lUnits[i]->sendData(data);
                break;
            }
        }
    }
}

bool Communication::hasDataWaitToSend()
{
    for(int i=0;i<m_lUnits.size();++i)
    {
        if(m_lUnits[i]->hasDataWaiting())
        {
            return true;
        }
    }
    return false;
}

void Communication::stop()
{
    //todo
    for(int i=0;i<m_lUnits.size();++i)
    {
        m_lUnits[i]->stopCommunication();
    }
    m_lUnits.clear();
}

void Communication::recordRemainCmds()
{
//    if(m_waitSendDatas.size() > 0)
//    {
//        QFile * pFile = new QFile("waitSend.txt",this);
//        if(pFile->open(QFile::WriteOnly | QFile::Text | QFile::Truncate))
//        {
//            QTextStream stream2(pFile);
//            stream2 << m_waitSendDatas.size();
//            endl(stream2);
//            for(int i=0;i<m_waitSendDatas.size(); ++i)
//            {
//                QTextStream stream(pFile);
//                stream << m_waitSendDatas.at(i).toHex();
//                endl(stream);
//            }
//        }
//        pFile->close();
    //    }
}

void Communication::addRelateIdToUnit(quint32 nUnitId, quint8 nRelateId)
{
    for(int i=0;i<m_lUnits.size();++i)
    {
        CommunicateUnit * pUnit = m_lUnits[i];
        if(pUnit->getUnitId() == nUnitId)
        {
            pUnit->addRelateId(nRelateId);
        }
    }
}

void Communication::removeUnAvailablePorts()
{
    qDebug() << "remove" << m_lUnits.size();
    if(m_lUnits.size() == 0)
        emit connectionError(0,UserDefine::ERR_IP_ADDRESS_NOT_FOUND,"No available ip address!");
    for(int i=m_lUnits.size();--i>=0;)
    {
        if(!m_lUnits[i]->isAvailable())
        {
            if(m_lUnits[i]->getConnectionStatus()&UserDefine::CAN_CONNECTED)
            {
                QString str = tr("ip : %1 connection error, no servo can be connected!").arg(m_lUnits[i]->getCommunicationUnitName());
                emit connectionError(m_lUnits[i]->getUnitId(),UserDefine::ERR_MOTOR_DISCONNECTION,str);
            }
            else
            {
                QString str = tr("ip : %1 connection error, no CAN can be connected!").arg(m_lUnits[i]->getCommunicationUnitName());
                emit connectionError(m_lUnits[i]->getUnitId(),UserDefine::ERR_CAN_DISCONNECTION,str);
                m_lUnits.removeAt(i);
            }

        }
    }
}

void Communication::changeUnitRelateId(quint8 nOldId, quint8 nNewId)
{
    for(int i=0;i<m_lUnits.size();++i)
    {
        CommunicateUnit * pUnit = m_lUnits[i];
        if(pUnit->isContainsRelateId(nOldId))
        {
            pUnit->switchOldRelateIdToNewRelateId(nOldId,nNewId);
        }
    }
}

void Communication::setUnitConnectionStatus(quint32 nUnitId, quint8 nStatus)
{
    for(int i=0;i<m_lUnits.size();++i)
    {
        CommunicateUnit * pUnit = m_lUnits[i];
        if(pUnit->getUnitId() == nUnitId)
        {
            pUnit->setConnectionStatus(nStatus);
            break;
        }
    }
}

void Communication::unitFinished(quint8 unitId)
{
    for(int i=0;i<m_lUnits.size();++i)
    {
        CommunicateUnit * pUnit = m_lUnits[i];
        if(pUnit->getUnitId() == unitId)
        {
            m_lUnits.removeAt(i);
            pUnit->deleteLater();
            break;
        }
    }
}
