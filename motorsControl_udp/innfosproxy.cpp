#include "innfosproxy.h"
#include "mediator.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include "communication.h"
#include <QMutexLocker>
#include "autorecoginze.h"
#include "userdefine.h"
//#define DEVICEID_127 //the max device id is 127



InnfosProxy::InnfosProxy(const int nId, const quint8 nDeviceId):
    m_nId(nId),
    m_pData(nullptr)
{
    m_pData = new IData();
    m_pData->AddProxyBegin(nId,nDeviceId);
}

InnfosProxy::~InnfosProxy()
{
    if (m_pData)
    {
        delete m_pData;
    }
}

void InnfosProxy::Send()
{
    if (/*CanSend()*/true)
    {
        Mediator::getInstance()->SendRequest(m_pData->GetBuffer());
    }

}

void InnfosProxy::decode(quint8 communicateUnitId,QByteArray &buf)
{
    IData data(buf);
    if (buf.size() < 6/* || !data.CheckData()*/)
    {
        return;
    }
    bool bRet = false;
    quint8 nMode = buf.at(2);
    quint8 nDeviceId = buf.at(1);
#ifdef DEVICEID_127
    nDeviceId -= 128;
#endif
    switch (nMode)
    {
    case D_READ_CUR_POSITION:
    {
        data.Skip(5);
        qint32 nValue = data.ReadInt();
        qreal real = nValue;
        real = (real) / (1 << 24);
        Mediator::getInstance()->SetCurParam(nDeviceId,real,nMode);

    }
        break;
    case D_READ_CUR_CURRENT:
    {
        data.Skip(5);
        qint32 nValue = data.ReadInt();
        qreal real = nValue;
        real = (real)*motorDataMgrInstance->getMotorDataAttrValueAsDouble(nDeviceId,MotorData::CURRENT_SCALE) / (1 << 24);
        Mediator::getInstance()->SetCurParam(nDeviceId,real,nMode);

    }
        break;
    case D_READ_CUR_VELOCITY:
    case D_READ_CURRENT_MAXSPEED:
    {
        data.Skip(5);
        qint32 nValue = data.ReadInt();
        qreal real = nValue;
        real = (real)*motorDataMgrInstance->getMotorDataAttrValueAsDouble(nDeviceId,MotorData::VELOCITY_SCALE) / (1 << 24);
        Mediator::getInstance()->SetCurParam(nDeviceId,real,nMode);

    }
        break;
    case D_READ_CHANNEL_1:
    {
        data.Skip(5);
        qint32 nValue = data.ReadShort();
        qreal real = nValue;
        real = (real)/**curScale*/ / (1 << 15);
        Mediator::getInstance()->chartVauleChange(UserDefine::channel_1,real);
    }
        break;
    case D_READ_CHANNEL_2:
    {
        data.Skip(5);
        qint32 nValue = data.ReadShort();
        qreal real = nValue;
        real = (real)/**curScale*/ / (1 << 15);
        Mediator::getInstance()->chartVauleChange(UserDefine::channel_2,real);
    }
        break;
    case D_READ_CHANNEL_3:
    {
        data.Skip(5);
        qint32 nValue = data.ReadShort();
        qreal real = nValue;
        real = (real)/**velScale*/ / (1 << 15);
        Mediator::getInstance()->chartVauleChange(UserDefine::channel_3,real);
    }
        break;
    case D_READ_CHANNEL_4:
    {
        data.Skip(5);
        qint32 nValue = data.ReadShort();
        qreal real = nValue;
        real = (real) / (1 << 15);
        Mediator::getInstance()->chartVauleChange(UserDefine::channel_4,/*real*16*/real);
    }
        break;
    case D_READ_CHART_FREQUENCY:
    {
        data.Skip(5);
        qint32 nValue = data.ReadShort();
        qreal real = nValue;
        //real = (real) / (1 << 15);
        Mediator::getInstance()->SetCurParam(nDeviceId,real,nMode);
    }
        break;
    case D_READ_CHART_THRESHOLD:
    {
        data.Skip(5);
        qint32 nValue = data.ReadShort();
        qreal real = nValue;
        real = (real) / (1 << 15);
        Mediator::getInstance()->SetCurParam(nDeviceId,real,nMode);
    }
        break;
    case D_READ_POS_P:
    case D_READ_POS_I:
    case D_READ_POS_D:
    case D_READ_CUR_P:
    case D_READ_CUR_I:
    case D_READ_VEL_P:
    case D_READ_VEL_I:
    case D_READ_INERTIA:
    {
        data.Skip(5);
        qint32 nValue = data.ReadInt();
        qreal real = nValue;
        real = (real) / (1 << 24);
        Mediator::getInstance()->SetCurParam(nDeviceId,real,nMode);

    }
        break;
    case D_READ_VOLTAGE:
    {
        data.Skip(5);
        quint16 nValue = data.ReadUShort();
        qreal real = nValue;
        real = (real) / 1024;
        Mediator::getInstance()->SetCurParam(nDeviceId,real,nMode);

    }
        break;
    case D_READ_CURRENT_PID_MIN:
    case D_READ_CURRENT_PID_MAX:
    case D_READ_VELOCITY_PID_MIN:
    case D_READ_VELOCITY_PID_MAX:
    case D_READ_POSITION_PID_MIN:
    case D_READ_POSITION_PID_MAX:
    case D_READ_PROFILE_POS_ACC:
    case D_READ_PROFILE_POS_DEC:
    case D_READ_PROFILE_POS_MAX_SPEED:
    case D_READ_PROFILE_VEL_ACC:
    case D_READ_PROFILE_VEL_DEC:
    case D_READ_PROFILE_VEL_MAX_SPEED:
    case D_READ_POS_OFFSET:
    case D_READ_HOMING_CUR_MAX:
    case D_READ_HOMING_CUR_MIN:
    case D_READ_CALIBRATION_ANGLE:
    {
        data.Skip(5);
        qreal maximum = data.ReadInt();
        maximum = maximum / (1 << 24);
        Mediator::getInstance()->SetCurParam(nDeviceId,maximum , nMode);

    }
        break;
    case D_READ_MAX_POS:
    case D_READ_MIN_POS:
    {
        data.Skip(5);
        qreal maximum = data.ReadInt();
        maximum = maximum / (1 << 24);
        Mediator::getInstance()->SetCurParam(nDeviceId,maximum , nMode);

    }
        break;
    case D_CHECK_ERROR:
    {
        data.Skip(5);
        quint16 nErrorId = data.ReadUShort();
        if(nErrorId > 0)
        {
            Mediator::getInstance()->SetCurParam(nDeviceId,nErrorId , nMode);
        }

    }
        break;
    case D_READ_ADDRESS:
    {
         data.Skip(5);
         int nMac = data.ReadInt();
//         quint32 nMac = 1000;
         Mediator::getInstance()->reciveMotorInfo(communicateUnitId,nMac,nDeviceId);
    }
        break;
    case D_READ_HOMING_LIMIT:
    {
         data.Skip(4);
         quint8 lenth = data.ReadByte();//check data lenth, compat different proxy version
         if(lenth == 1)
         {
             quint8 nValue = data.ReadByte();
             Mediator::getInstance()->SetCurParam(nDeviceId,nValue,nMode);
         }
         else
         {
             qint16 nValue = data.ReadShort();
             Mediator::getInstance()->SetCurParam(nDeviceId,nValue,nMode);
         }

    }
        break;
    case D_READ_CURRENT_SCALE:
    {
        data.Skip(5);
        qreal maximum = data.ReadShort();
        maximum = maximum / (1 << 8);
        if(maximum>=8 && maximum <= 35)//to confirm that current is valid.
        {
           Mediator::getInstance()->SetCurParam(nDeviceId,maximum , nMode);
        }
        else
        {
            //MsgBox::Tip(nullptr,tr("Tip"),"Current maximum is error!"); //todo
        }


    }
        break;
    case D_READ_FILTER_C_STATUS:
    case D_READ_FILTER_V_STATUS:
    case D_READ_FILTER_P_STATUS:
    case D_READ_MOTOR_MODE:
    {
        data.Skip(5);
        qint8 status = data.ReadByte();
        Mediator::getInstance()->SetCurParam(nDeviceId,status,nMode);
    }
        break;
    case D_READ_FILTER_C_VALUE:
    case D_READ_FILTER_V_VALUE:
    case D_READ_FILTER_P_VALUE:
    case D_READ_TEMP_INVERTER:
    case D_READ_TEMP_MOTOR:
    case D_READ_TEMP_PROTECT:
    case D_READ_TEMP_RECOVERY:
    {
        data.Skip(5);
        qreal value = data.ReadUShort();
        value = value / (1 << 8);
        Mediator::getInstance()->SetCurParam(nDeviceId,value , nMode);
    }
        break;
    case D_READ_LOCK_ENERGY:
    {
        data.Skip(5);
        qreal value = data.ReadInt();
        value = value / 75.225;
        Mediator::getInstance()->SetCurParam(nDeviceId,value , nMode);
    }
        break;
    case D_READ_CALIBRATION_SWITCH:
    case D_READ_MOTORS_SWITCH:
    {
        data.Skip(5);
        qint8 status = data.ReadByte();
        Mediator::getInstance()->SetCurParam(nDeviceId,status,nMode);
    }
        break;

    case D_CRC_ERROR:
    {
        //int a = 0;
    }
        break;
    case D_READ_VERSION:
    {
        data.Skip(5);
        quint32 version = data.ReadUByte()*100+data.ReadUByte()*10+data.ReadUByte();
        Mediator::getInstance()->SetCurParam(nDeviceId,version,nMode);
    }
        break;
    default:
        bRet = true;
        break;
    }

}

void InnfosProxy::SendProxy(const quint8 nDeviceId, const int nIdx)
{

    NoDataProxy proxy(nIdx,nDeviceId);
    proxy.m_pData->WriteShort(0);
    proxy.m_pData->AddProxyEnd();
    proxy.Send();
}

void InnfosProxy::SendProxy(const quint8 nDeviceId, const quint32 nDeviceMac, const int nIdx, const quint8 newDeviceId)
{
    InnfosProxy proxy(nIdx,nDeviceId);
    proxy.m_pData->WriteShort(5);
    proxy.m_pData->WriteData(nDeviceMac);
    proxy.m_pData->WriteData(newDeviceId);
    proxy.m_pData->AddCRCCode(5);
    proxy.m_pData->AddProxyEnd();
    proxy.Send();
}

void InnfosProxy::SendQrealProxy(const quint8 nDeviceId, const int nIdx, qreal data)
{
    switch (nIdx) {
    case D_SET_CHART_THRESHOLD:
    {
        qint16 nScaleValue = data*(1<<15);
        SendProxy(nDeviceId,nIdx,nScaleValue);
    }
        break;
    case D_SET_CURRENT:
    case D_SET_CURRENT_ID:
    {
        int nScaleValue = data/motorDataMgrInstance->getMotorDataAttrValueAsDouble(nDeviceId,MotorData::CURRENT_SCALE)*(1<<24);
        SendProxy(nDeviceId,nIdx,nScaleValue);
    }
        break;
    case D_SET_VELOCITY:
    case D_SET_CURRENT_MAXSPEED:
    {
        int nScaleValue = data/motorDataMgrInstance->getMotorDataAttrValueAsDouble(nDeviceId,MotorData::VELOCITY_SCALE)*(1<<24);
        SendProxy(nDeviceId,nIdx,nScaleValue);
    }
        break;
    default:
    {
        int nScaleValue = data*(1<<24);
        SendProxy(nDeviceId,nIdx,nScaleValue);
    }
        break;
    }
}

//template<class T>
//void SendProxy(const int nDeviceId, const int nIdx, T data)
//{
//    InnfosProxy proxy(nIdx,nDeviceId);
//    proxy.m_pData->WriteShort(sizeof(T));
//    proxy.m_pData->WriteData(data);
//    proxy.m_pData->AddCRCCode(sizeof(T));
//    proxy.m_pData->AddProxyEnd();
//}


/*无数据协议*/
NoDataProxy::NoDataProxy(const int nId, const quint8 nDeviceId):
    InnfosProxy(nId,nDeviceId)
{

}

void NoDataProxy::decode(quint8 communicateUnitId, QByteArray &buf)
{
    if (buf.size() < 6)
    {
        return;
    }
//    bool bRet = false;
//    quint8 nMode = buf.at(2);
//    switch (nMode)
//    {

//    default:
//        bRet = true;
//        break;
//    }
}

/*有数据无校验协议*/
NoCrcProxy::NoCrcProxy(const int nId, const quint8 nDeviceId) :
    InnfosProxy(nId,nDeviceId)
{

}

void NoCrcProxy::decode(quint8 communicateUnitId, QByteArray &buf)
{
    IData data(buf);
    if (buf.size() < 6)
    {
        return;
    }
    bool bRet = false;
    quint8 nMode = buf.at(2);
    quint8 nDeviceId = buf.at(1);
#ifdef DEVICEID_127
    nDeviceId -= 128;
#endif
    switch (nMode)
    {
    case D_HANDSHAKE:
        //IData::deviceId = buf.at(1);
        if (buf.at(5) == '\0')
        {
            Mediator::getInstance()->Handshake(nDeviceId,false);//
        }
        else
        {
            Mediator::getInstance()->Handshake(nDeviceId,true);
        }
        break;
    case D_SET_POSITION_P:
    case D_SET_POSITION_I:
    case D_SET_POSITION_D:
    case D_SET_CURRENT_P:
    case D_SET_CURRENT_I:
    case D_SET_VELOCITY_P:
    case D_SET_VELOCITY_I:
    case D_SET_CURRENT:
    case D_SET_POSITION:
    case D_SET_VELOCITY:
    case D_SET_DEVICE_ID:
    case D_CLEAR_ERROR:
    case D_CLEAR_HOMING:
    case D_SET_PAIRS:
    case D_SAVE_PARAM:
        if (buf.at(5) == '\x1')
        {
            Mediator::getInstance()->SetSucceed(nDeviceId,nMode);
            bRet = true;
        }
        else
        {
            Mediator::getInstance()->SetFailed(nMode);
        }
        break;
    case D_CHART_DATA_STATR:
        Mediator::getInstance()->receiveNoDataProxy(nMode);
        break;
    case D_CAN_CONNECT:
    {
        mediator->onCanConnected(communicateUnitId);
    }
        break;

//    case D_SET_SWITCH_MOTORS:
//    {
//        quint8 nValue = buf.at(5);
//        quint8 nSwitch = motorDataMgrInstance->getMotorDataAttrValueAsInt(nDeviceId,MotorData::MOTOR_SWITCH);
//        if(nValue == 0)//如果设置失败了，保持原来的值不变
//        {
//            Mediator::getInstance()->SetCurParam(nDeviceId,nSwitch==UserDefine::MOTOR_SWITCH_ON?UserDefine::MOTOR_SWITCH_OFF:UserDefine::MOTOR_SWITCH_ON,nMode);
//        }
//        else
//        {
//            Mediator::getInstance()->SetCurParam(nDeviceId,nSwitch,nMode);
//            qDebug() << "open success" << nSwitch;
//        }

//    }
//        break;
    case D_READ_MOTORS_SWITCH://暂时处理
    {
        data.Skip(5);
        quint8 status = data.ReadUByte();
        Mediator::getInstance()->SetCurParam(nDeviceId,status,nMode);
        qDebug() << "switch" << status;
    }
        break;
    case D_READ_MOTOR_MODE://暂时处理
    {
        data.Skip(5);
        quint8 status = data.ReadUByte();
        Mediator::getInstance()->SetCurParam(nDeviceId,status,nMode);
        qDebug() << "switch" << status;
    }
        break;
    default:
        if (buf.at(5) == '\x1')
        {
            Mediator::getInstance()->SetSucceed(nDeviceId,nMode);
        }
        else
        {
            Mediator::getInstance()->SetFailed(nMode);
        }
        break;
    }
}

ProxyWatcher * ProxyWatcher::m_pWatcher = nullptr;

ProxyWatcher *ProxyWatcher::getInstance()
{
    if(!m_pWatcher)
        m_pWatcher = new ProxyWatcher();
    return m_pWatcher;
}

void ProxyWatcher::autoDestroy()
{
    if(m_pWatcher)
    {
        m_pWatcher->checkTimeout();
        delete m_pWatcher;
    }
    m_pWatcher = nullptr;
}

void ProxyWatcher::addSendItem(const QByteArray &buf)
{
    checkTimeout();
    if(buf.size() >3 && buf.at(2)==2)
        return;
    logStruct * pStruct = new logStruct();
    pStruct->sendTime = QTime::currentTime();
    pStruct->sendContent = buf;
    m_sendContents.push_back(pStruct);
}

void ProxyWatcher::reciveItem(const QByteArray &buf)
{

    checkTimeout();
    QMutexLocker locker(&m_qmMutex);
    foreach (logStruct * pStruct, m_sendContents) {
        QByteArray content = pStruct->sendContent;
        if((content.at(1)==buf.at(1) || content.at(1)=='\0') && content.at(2)==buf.at(2))
        {
            m_sendContents.removeOne(pStruct);
            delete pStruct;
            break;
        }
    }
}

ProxyWatcher::~ProxyWatcher()
{
    for (int i=m_sendContents.size();--i>=0;)
    {
        delete m_sendContents.at(i);
    }
    //delete m_pLogStream;
}

void ProxyWatcher::checkTimeout()
{
    QMutexLocker locker(&m_qmMutex);
    QTime curTime = QTime::currentTime();
    for (int i=m_sendContents.size();--i>=0;)
    {
        logStruct * pStruct = m_sendContents.at(i);
        if(pStruct->sendTime.msecsTo(curTime) > 1000)
        {
            m_sendContents.removeOne(pStruct);
            QString str;
            str = QString(pStruct->sendContent.toHex()) + QString("   ")+pStruct->sendTime.toString();
            if(m_pLogFile)
            {
                QTextStream out(m_pLogFile);
                out<<"Fail "<< str/*QString(pStruct->sendContent.toHex()) << "  " <<pStruct->sendTime.toString()*/;
                endl(out);

            }
            //qDebug()<<"Fail "<< pStruct->sendContent.toHex() << pStruct->sendTime;
            delete pStruct;
#ifdef WARNING_DEBUG
            //Communication::getInstance()->stop();
            //int nRet = QMessageBox::warning(nullptr,tr("Warning"),str,QMessageBox::Ok);
//            if(nRet)
//                Communication::getInstance()->reStart();
#endif
        }
    }
}

ProxyWatcher::ProxyWatcher(QObject *parent):
    QObject(parent),
    m_pLogFile(nullptr),
    m_pLogStream(nullptr)
{
    m_pLogFile = new QFile("./logFile.txt",this);
    if(m_pLogFile->open(QFile::WriteOnly|QFile::Text|QFile::Truncate))
    {
        //m_pLogStream = new QTextStream(m_pLogFile);
    }
    else
    {
        m_pLogFile->close();
        m_pLogFile = nullptr;
    }
}
