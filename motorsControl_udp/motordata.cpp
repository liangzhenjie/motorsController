﻿#include "motordata.h"
#include "mediator.h"
#include "innfosproxy.h"
#include "dataUtil.h"
#include <QDebug>
#include <QTimer>

const int nMaxErrorCnt = 30;
MotorData::MotorData(const quint8 nDeviceId, const quint32 nDeviceMac, QObject *parent) :
    QObject(parent),
    m_nHeartFailCnt(0),
    m_nAutoRequestInterval(1000)
{
    initData();
    setValue(DEVICE_ID,nDeviceId);
    setValue(MAC_ID,nDeviceMac);
    userRequestValue(DEVICE_ID,nDeviceId);
    userRequestValue(MAC_ID,nDeviceMac);
    m_pHeartTimer = new QTimer(this);
    connect(m_pHeartTimer,&QTimer::timeout,[=]{
        if(!isOnline())
        {
            setValue(ONLINE_STATUS,UserDefine::Status_Offline);
            m_pHeartTimer->stop();
            m_pValueTimer->stop();
        }
        else
        {
            InnfosProxy::SendProxy(deviceId(),D_HANDSHAKE);
            if(m_motorData[ERROR_ID] == 0)//if error occur, do not check error again
                InnfosProxy::SendProxy(deviceId(),D_CHECK_ERROR);
            ++ m_nHeartFailCnt;
        }
    });
    m_pValueTimer = new QTimer(this);
    connect(m_pValueTimer,&QTimer::timeout,this,&MotorData::requestActualValue);
    InnfosProxy::SendProxy(deviceId(),D_READ_VERSION);
    InnfosProxy::SendProxy(deviceId(),D_READ_MOTORS_SWITCH);
}

MotorData::~MotorData()
{

}


bool MotorData::deviceIdIsAvailable() const
{
    return m_userRequestData[DEVICE_ID] == m_motorData[DEVICE_ID];
}


void MotorData::changeDemand(qreal value)
{
    switch (getValue(MODE_ID).toUInt()) {
    case Mode_Cur:

        setValueByUser(CUR_IQ_SET,value);
        break;
    case Mode_Vel:
    case Mode_Profile_Vel:

        setValueByUser(VEL_SET,value);
        break;
    case Mode_Pos:
    case Mode_Profile_Pos:

        setValueByUser(POS_SET,value);
        break;
    default:
        break;
    }
}

void MotorData::switchAutoRequestActual(bool bStart)
{
    if(bStart)
    {
        m_pValueTimer->start(m_nAutoRequestInterval);
        //m_pHeartTimer->start(1000);
    }
    else
    {
        m_pValueTimer->stop();
        //m_pHeartTimer->stop();
    }
}

void MotorData::setAutoRequestInterval(quint32 mSec)
{
    m_nAutoRequestInterval = mSec;
    m_pValueTimer->stop();
    m_pValueTimer->start(m_nAutoRequestInterval);
}

void MotorData::saveAllParams()
{
    InnfosProxy::SendProxy(deviceId(),D_SAVE_PARAM);
}

void MotorData::clearHomingInfo()
{
    InnfosProxy::SendProxy(deviceId(),D_CLEAR_HOMING);
}

void MotorData::setHomingOperationMode(const quint8 nMode)
{
    InnfosProxy::SendProxy(deviceId(),D_SET_HOMING_OPERATION,nMode);
}

void MotorData::openChartChannel(const int nChannelId)
{
    int nProxyId[UserDefine::channel_cnt] = {D_CHANNEL1_OPEN,D_CHANNEL2_OPEN,D_CHANNEL3_OPEN,D_CHANNEL4_OPEN};
    InnfosProxy::SendProxy(deviceId(),nProxyId[nChannelId]);
}

void MotorData::closeChartChannel(const int nChannelId)
{
    int nProxyId[UserDefine::channel_cnt] = {D_CHANNEL1_CLOSE,D_CHANNEL2_CLOSE,D_CHANNEL3_CLOSE,D_CHANNEL4_CLOSE};
    InnfosProxy::SendProxy(deviceId(),nProxyId[nChannelId]);
}

void MotorData::switchChartAllChannel(bool bOn)
{
    quint8 nProxyId = bOn?D_CHART_OPEN:D_CHART_CLOSE;
    InnfosProxy::SendProxy(deviceId(),nProxyId);
}

void MotorData::switchCalibrationVel(quint8 nValue)
{
    InnfosProxy::SendProxy(deviceId(),D_SWITCH_CALIBRATION_VEL,nValue);
}

void MotorData::switchCalibration(quint8 nValue)
{
    InnfosProxy::SendProxy(deviceId(),D_SWITCH_CALIBRATION,nValue);
}

void MotorData::startCalibration()
{
    InnfosProxy::SendProxy(deviceId(),D_START_CALIBRATION);
}

void MotorData::requestSuccessfully(quint8 nDataId)
{

    if(DATA_CNT > nDataId)
    {
        qDebug() << "success" << nDataId << m_userRequestData[nDataId];
        setValue(nDataId,m_userRequestData[nDataId]);
    }
}

void MotorData::setValue(int nDataId, QVariant value, bool bEmitSignal)
{
    if(DATA_CNT > nDataId)
    {
        m_motorData[nDataId] = value;
        qDebug() << "set" << nDataId << m_motorData[nDataId];
        if(bEmitSignal)
            //emit mediator->deviceDataChange(deviceId(),(Motor_Data_Id)nDataId,value);
            motorDataMgrInstance->dataChanged(deviceId(),(Motor_Data_Id)nDataId,value);
    }
}

void MotorData::userRequestValue(int nDataId, QVariant value)
{
    if(DATA_CNT > nDataId)
    {
        m_userRequestData[nDataId] = value;
    }
}

void MotorData::initData()
{
    for(int i=CUR_IQ_SET;i<DATA_CNT;++i)
    {
        m_motorData[i] = 0;
        m_userRequestData[i] = 0;
    }
    setValue(CURRENT_SCALE,curScale);
    setValue(VELOCITY_SCALE,velScale);
    setValue(ERROR_ID,UserDefine::ERR_NONE);
    userRequestValue(CURRENT_SCALE,curScale);
    userRequestValue(VELOCITY_SCALE,velScale);
    userRequestValue(ERROR_ID,UserDefine::ERR_NONE);
}


void MotorData::activeModeSuccessfully()
{
    switch (getValue(MODE_ID).toUInt()) {
    case Mode_Cur:
        setValueByUser(CUR_IQ_SET,0,false);//reset demand
        break;
    case Mode_Vel:
        setValueByUser(VEL_SET,0,false);//reset demand
        break;
    case Mode_Pos:
        break;
    case Mode_Profile_Pos:
        break;
    case Mode_Profile_Vel:
        //setValueByUser(VEL_SET,0,false);//reset demand
        break;
    default:
        break;
    }

}

void MotorData::requestActualValue()
{
    InnfosProxy::SendProxy(deviceId(),D_READ_CUR_CURRENT);
    InnfosProxy::SendProxy(deviceId(),D_READ_CUR_VELOCITY);
    InnfosProxy::SendProxy(deviceId(),D_READ_CUR_POSITION);
    InnfosProxy::SendProxy(deviceId(),D_READ_VOLTAGE);
    InnfosProxy::SendProxy(deviceId(),D_READ_TEMP_INVERTER);
    InnfosProxy::SendProxy(deviceId(),D_READ_TEMP_MOTOR);
}

void MotorData::saveData()
{
//    QFileDialog dialog(nullptr,tr("Save as"),QDir::currentPath());
//    dialog.setFileMode(QFileDialog::AnyFile);
//    dialog.setAcceptMode(QFileDialog::AcceptSave);
//    dialog.setNameFilter(tr("Innfos(*.innfos)"));
//    if(dialog.exec() == QDialog::Accepted)
//    {
//        QString path = dialog.selectedFiles().first();
//        if(path.size() > 0)
//        {
//            saveDataToFile(path);
//        }

//    }
}

void MotorData::loadData()
{
//    QFileDialog dialog(nullptr,tr("Load"),QDir::currentPath());
//    dialog.setFileMode(QFileDialog::ExistingFile);
//    dialog.setNameFilter(tr("Innfos(*.innfos)"));
//    if(dialog.exec() == QDialog::Accepted)
//    {
//        QString path = dialog.selectedFiles().first();
//        if(path.size() > 0)
//        {
//            readDataFromFile(path);
//        }

//    }
}

void MotorData::reconnect()
{
    m_nHeartFailCnt = 0;
    if(m_pHeartTimer)
    {
        m_pHeartTimer->start();
    }
    if(m_pValueTimer)
    {
        m_pValueTimer->start();
    }
}

MotorData::Motor_Mode MotorData::currentMode() const
{
    return (Motor_Mode)(getValue(MODE_ID).toInt());
}

void MotorData::saveDataToFile(QString fileName)//todo
{
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate))
    {
        qDebug() << tr("Cannot write file %1 %2").arg(fileName).arg(file.errorString());
        return;
    }

//    QJsonArray arr;
////    for(int i=0;i<DATA_CNT;++i)
////    {
////        QJsonObject attr;
////        attr[tr("Attr%1").arg(i)] = m_motorData[i];
////        arr.append(attr);
////    }
//    QJsonDocument saveDoc(arr);
//    file.write(saveDoc.toJson());
    file.close();
}

void MotorData::readDataFromFile(QString fileName)
{
    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QXmlStreamReader reader(&file);
        while(!reader.atEnd())
        {
            QXmlStreamReader::TokenType nType = reader.readNext();
            switch (nType)
            {
            case QXmlStreamReader::StartDocument:

                break;
            case QXmlStreamReader::StartElement:
            {
                QString strElementName = reader.name().toString();
                if(strElementName == "ServoData")
                {
                    QXmlStreamAttributes attributes = reader.attributes();
                    if(attributes.hasAttribute("Version") && attributes.value("Version").toString() == "1.0")
                    {
                        readParams(&reader);
                    }
                }
            }
                break;
            default:
                break;
            }
        }
        if(reader.hasError())
        {
            qDebug() << tr("Errorinfo:%1 line:%2 column:%3 character offset:%4").arg(reader.errorString()).arg(reader.lineNumber()).arg(reader.columnNumber()).arg(reader.characterOffset());
        }
        file.close();
    }


}

void MotorData::readParams(QXmlStreamReader *reader)
{
    while (!reader->atEnd()) {
        reader->readNext();
        if(reader->isStartElement())
        {
            QString strElementName = reader->name().toString();
            int nIdx = strElementName.indexOf("Attr");
            if(nIdx >=0)
            {
                int nAttrId = strElementName.right(strElementName.size()-4).toInt();
                if(nAttrId>=0 && nAttrId<DATA_CNT)
                {
                    qreal readValue = reader->readElementText().toDouble();
                    if(m_motorData[nAttrId] != readValue)
                    {
                        m_motorData[nAttrId] = readValue;
                        //emit dataChange((Motor_Data_Id)nAttrId);to do
                    }

                }
            }
        }
    }
}




quint8 MotorData::deviceId() const
{
//    if(m_oldDeviceId >=0)
//        return m_oldDeviceId;
    return getValue(DEVICE_ID).toUInt();
}

qint8 MotorData::requestDeviceId() const
{
    return m_userRequestData[DEVICE_ID].toUInt();
}

quint32 MotorData::deviceMac() const
{
    return getValue(MAC_ID).toUInt();
}

void MotorData::setValueByProxy(const int nProxyId, QVariant value)
{
    //responseHeart(true);
    if(!isOnline())
        responseHeart(true);
    else
        m_nHeartFailCnt = 0;

    switch (nProxyId) {
    case D_CHECK_ERROR:
    {
        setValue(ERROR_ID,value);
        QMap<int,QString> errorInfo;
        errorInfo.insert(UserDefine::ERR_MOTOR_OVERVOLTAGE,QString::fromLocal8Bit("ID:%1 voltage:%2 Overvoltage error!").arg(deviceId()).arg(getValue(VOLTAGE).toDouble()));
        errorInfo.insert(UserDefine::ERR_MOTOR_UNDERVOLTAGE,QString::fromLocal8Bit("ID:%1 voltage:%2 Undervoltage error!").arg(deviceId()).arg(getValue(VOLTAGE).toDouble()));
        errorInfo.insert(UserDefine::ERR_MOTOR_LOCKED_ROTOR,QString::fromLocal8Bit("ID:%1 Locked-rotor error!").arg(deviceId()));
        errorInfo.insert(UserDefine::ERR_MOTOR_OVERHEATING,QString::fromLocal8Bit("ID:%1 Temperture:%2 Overheating error!").arg(deviceId()).arg(getValue(TEMP_MOTOR).toDouble()));
        errorInfo.insert(UserDefine::ERR_MOTOR_READ_OR_WIRTE,QString::fromLocal8Bit("ID:%1 Read or write params error!").arg(deviceId()));
        errorInfo.insert(UserDefine::ERR_MOTOR_MULTI_TURN,QString::fromLocal8Bit("ID:%1 Multi-turn count error!").arg(deviceId()));
        errorInfo.insert(UserDefine::ERR_MOTOR_TEMPERATURE_SENSOR,QString::fromLocal8Bit("ID:%1 Temperature sensor error!").arg(deviceId()));
        errorInfo.insert(UserDefine::ERR_CAN_COMMUNICATION,QString::fromLocal8Bit("ID:%1 Can communication error!").arg(deviceId()));
        errorInfo.insert(UserDefine::ERR_DRV_PROTECTION,QString::fromLocal8Bit("ID:%1 DRV protection error!").arg(deviceId()));
        QList<int> keys = errorInfo.uniqueKeys();
        QString errorStr;
        for(int i=0;i<keys.size();++i)
        {
            if(value.toUInt()&keys.at(i))
            {
                errorStr += errorInfo.value(keys.at(i));
                errorStr += "\n";
            }

        }
        if(errorStr.length() > 0)
        {
            motorDataMgrInstance->errorOccured(deviceId(),value.toUInt(),errorStr);
        }

        m_errorHistory.append(value.toUInt());
    }

        break;
    case D_READ_MOTORS_SWITCH:
        if(value.toInt() == UserDefine::MOTOR_SWITCH_ON && m_motorData[MOTOR_SWITCH].toInt()==UserDefine::MOTOR_SWITCH_OFF)
        {
            requestAllValue();
        }
        setValue(MOTOR_SWITCH,value.toInt());

        break;

    default:
        Directives proxy = (Directives)nProxyId;
        Motor_Data_Id id = DataUtil::convertToMotorDataId(proxy);
        if(id != DIRECTIVES_INVALID)
        {
            setValue(id,value);
        }    
        break;
    }
}

void MotorData::requestAllValue()
{
//    activeMode(Mode_Cur);
//    for (int i=D_READ_CUR_CURRENT;i<=D_READ_CUR_POSITION;++i)
//    {
//        InnfosProxy::SendProxy(m_deviceId,i);
//    }
    for (int i=D_READ_CUR_CURRENT;i<=D_READ_CUR_POSITION;++i)
    {
        InnfosProxy::SendProxy(deviceId(),i);
    }


    for (int i=D_READ_CUR_P;i<=D_READ_PROFILE_POS_DEC;++i)
    {
        InnfosProxy::SendProxy(deviceId(),i);
    }

    for (int i=D_READ_PROFILE_VEL_MAX_SPEED;i<=D_READ_PROFILE_VEL_DEC;++i)
    {
        InnfosProxy::SendProxy(deviceId(),i);
    }

    for (int i=D_READ_CURRENT_PID_MIN;i<=D_READ_POSITION_PID_MAX;++i)
    {
        InnfosProxy::SendProxy(deviceId(),i);
    }

    for (int i=D_READ_CHART_THRESHOLD;i<=D_READ_CHART_FREQUENCY;++i)
    {
        InnfosProxy::SendProxy(deviceId(),i);
    }
    InnfosProxy::SendProxy(deviceId(),D_READ_VOLTAGE);
    InnfosProxy::SendProxy(deviceId(),D_READ_CURRENT_SCALE);

    for (int i=D_READ_MAX_POS;i<=D_READ_MIN_POS;++i)
    {
        InnfosProxy::SendProxy(deviceId(),i);
    }

    for (int i=D_READ_POS_OFFSET;i<=D_READ_HOMING_LIMIT;++i)
    {
        InnfosProxy::SendProxy(deviceId(),i);
    }
    InnfosProxy::SendProxy(deviceId(),D_READ_LAST_STATE);
    //InnfosProxy::SendProxy(deviceId(),D_READ_VERSION);
    InnfosProxy::SendProxy(deviceId(),D_READ_HOMING_CUR_MIN);
    InnfosProxy::SendProxy(deviceId(),D_READ_HOMING_CUR_MAX);

    InnfosProxy::SendProxy(deviceId(),D_READ_FILTER_C_STATUS);
    InnfosProxy::SendProxy(deviceId(),D_READ_FILTER_C_VALUE);
    InnfosProxy::SendProxy(deviceId(),D_READ_FILTER_V_STATUS);
    InnfosProxy::SendProxy(deviceId(),D_READ_FILTER_V_VALUE);
    InnfosProxy::SendProxy(deviceId(),D_READ_FILTER_P_STATUS);
    InnfosProxy::SendProxy(deviceId(),D_READ_FILTER_P_VALUE);
    InnfosProxy::SendProxy(deviceId(),D_READ_INERTIA);
    InnfosProxy::SendProxy(deviceId(),D_READ_LOCK_ENERGY);
    InnfosProxy::SendProxy(deviceId(),D_READ_TEMP_MOTOR);
    InnfosProxy::SendProxy(deviceId(),D_READ_TEMP_INVERTER);
    InnfosProxy::SendProxy(deviceId(),D_READ_TEMP_PROTECT);
    InnfosProxy::SendProxy(deviceId(),D_READ_TEMP_RECOVERY);
    InnfosProxy::SendProxy(deviceId(),D_READ_CALIBRATION_SWITCH);
    InnfosProxy::SendProxy(deviceId(),D_READ_CALIBRATION_ANGLE);
    InnfosProxy::SendProxy(deviceId(),D_READ_CURRENT_MAXSPEED);
    InnfosProxy::SendProxy(deviceId(),D_READ_MOTOR_MODE);
    //InnfosProxy::SendProxy(deviceId(),D_READ_MOTORS_SWITCH);

#ifndef NO_HEART_BEAT
    m_pHeartTimer->start(1000);
    //m_pValueTimer->start(m_nAutoRequestInterval);
#endif
}

QVariant MotorData::getValue(MotorData::Motor_Data_Id nDataId) const
{
    Q_ASSERT(nDataId < DATA_CNT);
    if(nDataId < DATA_CNT)
    {
        return m_motorData[nDataId];
    }
    return -1;
}

QVariant MotorData::getUserRequestValue(MotorData::Motor_Data_Id nDataId) const
{
    Q_ASSERT(nDataId < DATA_CNT);
    if(nDataId < DATA_CNT)
    {
        return m_userRequestData[nDataId];
    }
    return -1;
}

void MotorData::setValueByUser(MotorData::Motor_Data_Id nDataId, QVariant value, bool bSendProxy)
{
    if (/*getValue(nDataId) != value*/true)
    {
        Directives pId = DataUtil::convertToSetProxyId(nDataId);
        //m_motorData[nDataId] = value;
        userRequestValue(nDataId,value);
        if(pId != DIRECTIVES_INVALID)
        {
            //
            if(bSendProxy)
            {
                switch (pId) {
                case D_SET_CHART_FREQUENCY:// these values are qint16
                {
                    qint16 nValue = value.toInt();
                    InnfosProxy::SendProxy(deviceId(),pId,nValue);
                }
                    break;
                case D_SET_LOCK_ENERGY://
                {
                    qint32 nValue = value.toInt()*75.225;
                    InnfosProxy::SendProxy(deviceId(),pId,nValue);
                }
                    break;
                case D_SET_FILTER_C_VALUE:
                case D_SET_FILTER_V_VALUE:
                case D_SET_FILTER_P_VALUE:
                case D_SET_TEMP_PROTECT:
                case D_SET_TEMP_RECOVERY:
                {
                    qint16 nValue = value.toInt()*(1<<8);
                    InnfosProxy::SendProxy(deviceId(),pId,nValue);
                }
                    break;
                case D_SET_HOMING_LIMIT:
                case D_SET_FILTER_C_STATUS:
                case D_SET_FILTER_V_STATUS:
                case D_SET_FILTER_P_STATUS:
                case D_SWITCH_CALIBRATION:
                case D_SET_SWITCH_MOTORS:
                case D_SET_MODE:
                case D_SET_DEVICE_ID:
                {
                    quint8 invertValue = value.toInt();
                    InnfosProxy::SendProxy(deviceId(),pId,invertValue);
                    //qDebug()<<"send" << value.toInt() << getValue(MOTOR_SWITCH).toInt();
                }
                    break;
                case D_CHECK_ERROR:
                    if(value.toInt() == UserDefine::ERR_NONE)//clear error
                    {
                        InnfosProxy::SendProxy(deviceId(),D_CLEAR_ERROR);
                    }
                    break;
                case D_CHART_CLOSE:
                case D_CHART_OPEN:
                {
                    if(value.toInt() == UserDefine::CHART_SWITCH_ON)
                    {
                        InnfosProxy::SendProxy(deviceId(),D_CHART_OPEN);
                    }
                    else
                    {
                        InnfosProxy::SendProxy(deviceId(),D_CHART_CLOSE);
                    }
                }
                    break;
                default:
                    InnfosProxy::SendQrealProxy(deviceId(),pId,value.toDouble());
                    break;
                }
            }
            else
            {
                qDebug() << "user not send" << nDataId;
                requestSuccessfully(nDataId);//if do not need to send proxy, default is request successfully.
            }
        }
    }
}

void MotorData::responseHeart(bool bSuccess)
{
    if(bSuccess && isOnline())//判断掉线以后，如果接收到心跳，不管他
    {
        m_nHeartFailCnt = 0;
        if(getValue(ONLINE_STATUS).toInt() == UserDefine::Status_Offline)//这种情况只有断线重连才会判断成功，重连后m_nHeartFailCnt，但是电机数据在线状态还是断线
            setValue(ONLINE_STATUS,UserDefine::Status_Online);//
    }

}

bool MotorData::isOnline() const
{
#ifdef MY_DEBUG
    return true;
#endif
    return m_nHeartFailCnt < 5;
}

//data manager

MotorDataMgr * MotorDataMgr::m_pMgr = nullptr;

MotorDataMgr* MotorDataMgr::getInstance()
{
    if(m_pMgr == nullptr)
        m_pMgr = new MotorDataMgr();
    return m_pMgr;
}


MotorDataMgr::MotorDataMgr()
{
    connect(this,&MotorDataMgr::dataChanged,mediator,&Mediator::motorAttrChanged);
    connect(this,&MotorDataMgr::errorOccured,mediator,&Mediator::errorOccur);
}

MotorDataMgr::~MotorDataMgr()
{
    for(int i=0;i<m_allMotorDatas.size();++i)
    {
        delete m_allMotorDatas.at(i);
    }
}

QVariant MotorDataMgr::getMotorDataAttrValue(const quint8 nDeviceId, const MotorData::Motor_Data_Id attrId) const
{
    MotorData * pData = getMotorDataById(nDeviceId);
    if(pData)
        return pData->getValue(attrId);
    qDebug()<< tr("getValue error DeviceId%1 AttrId%2").arg(nDeviceId).arg(attrId);
    return QVariant();//null
}

qreal MotorDataMgr::getMotorDataAttrValueAsDouble(const quint8 nDeviceId, const MotorData::Motor_Data_Id attrId) const
{
    bool bOK = false;
    qreal ret = getMotorDataAttrValue(nDeviceId,attrId).toDouble(&bOK);
    if(bOK)
        return ret;
    qDebug()<< tr("getValue error DeviceId%1 AttrId%2").arg(nDeviceId).arg(attrId);
    return 0;
}

qint32 MotorDataMgr::getMotorDataAttrValueAsInt(const quint8 nDeviceId, const MotorData::Motor_Data_Id attrId) const
{
    bool bOK = false;
    qint32 ret = getMotorDataAttrValue(nDeviceId,attrId).toInt(&bOK);
    if(bOK)
        return ret;
    qDebug()<< tr("getValue error DeviceId%1 AttrId%2").arg(nDeviceId).arg(attrId);
    return 0;
}

void MotorDataMgr::setMotorDataAttrByUser(const quint8 nDeviceId, const MotorData::Motor_Data_Id attrId, QVariant value, bool bSend)
{
    MotorData * pData = getMotorDataById(nDeviceId);
    if(pData)
    {
        switch (attrId) {
        case MotorData::MOTOR_SWITCH:
            if(/*pData->getValue(MotorData::MOTOR_SWITCH).toInt() != value.toInt()*/true)//if current value is equal to the target value,nothing will be done!
            {
                pData->setValueByUser(attrId,value,bSend);
            }
            break;
        default:
            pData->setValueByUser(attrId,value,bSend);
            break;
        }

    }
    else
    {
        qDebug() << tr("Set Motor %1 Attri %2 value %3 failed! ").arg(nDeviceId).arg(attrId).arg(value.toDouble());
    }
}

void MotorDataMgr::setMotorDataAttrByProxy(const quint8 nDeviceId, int proxyId, QVariant value)
{
    MotorData * pData = getMotorDataById(nDeviceId);
    if(pData)
    {
        pData->setValueByProxy(proxyId,value.toDouble());
    }
    else
    {
        qDebug() << tr("Set Motor %1 Attri %2 value %3 failed! ").arg(nDeviceId).arg(proxyId).arg(value.toDouble());
    }
}

void MotorDataMgr::setMotorDataAttrInBatch(const QList<quint8> idList, const MotorData::Motor_Data_Id attrId, QVariant value, bool bSend)
{
    for(int i=0;i<idList.size();++i)
    {
        setMotorDataAttrByUser(idList.at(i),attrId,value,bSend);
    }
}

void MotorDataMgr::AddMotorsData(QMap<quint8, quint32> dataMap)
{
    bool bUnique = checkIdUnique(dataMap);
    if(!bUnique)
    {
        handleUnuiqueError(dataMap);
        return;
    }
    QMapIterator<quint8, quint32> it(dataMap);
    while (it.hasNext())
    {
        it.next();
        qDebug() << "add" << it.key() <<it.value();
        MotorData * pData = new MotorData(it.key(),it.value(),this);
        m_allMotorDatas.append(pData);
        //pData->requestAllValue();
    }
}

QMap<quint8, quint32> MotorDataMgr::getMotorsMap() const
{
    QMap<quint8, quint32> dataMap;
    for(int i=0;i<m_allMotorDatas.size();++i)
    {
        MotorData * pData = m_allMotorDatas.at(i);
        dataMap.insert(pData->deviceId(),pData->deviceMac());
    }
    return dataMap;
}

QVector<quint16> MotorDataMgr::motorErrorHistory(const quint8 nDeviceId) const
{
    QVector<quint16> errorList;
    MotorData * pData = getMotorDataById(nDeviceId);
    if(pData)
        return pData->errorHistory();
    return errorList;
}

bool MotorDataMgr::deviceIdHasExist(quint8 nDeviceId)
{
    for(int i=0;i<m_allMotorDatas.size();++i)
    {
        MotorData * pData = m_allMotorDatas.at(i);
        if(pData->deviceId() == nDeviceId)
            return true;
    }
    return false;
}

void MotorDataMgr::activeMotorModeSuccessfully(const quint8 nDeviceId)
{
    MotorData * pData = getMotorDataById(nDeviceId);
    if(pData)
    {
        pData->activeModeSuccessfully();
        //emit dataChanged(nDeviceId,MotorData::MODE_ID,pData->getValue(MotorData::MODE_ID));
    }

}

void MotorDataMgr::activeMotorModeInBatch(const QList<quint8> idList, const MotorData::Motor_Mode mode)
{
    for(int i=0;i<idList.size();++i)
    {
        setMotorDataAttrByUser(idList.at(i),MotorData::MODE_ID,mode);
    }
}

void MotorDataMgr::regainAllData(const quint8 nDeviceId)
{
    MotorData * pData = getMotorDataById(nDeviceId);
    if(pData)
        pData->requestAllValue();
}

void MotorDataMgr::regainData(const quint8 nDeviceId, MotorData::Motor_Data_Id attrId)
{
    MotorData * pData = getMotorDataById(nDeviceId);
    if(pData)
    {
        Directives proxyId = DataUtil::convertToReadProxyId(attrId);
        if(proxyId != DIRECTIVES_INVALID)
        {
            InnfosProxy::SendProxy(nDeviceId,proxyId);
        }
    }
}

void MotorDataMgr::responseHeart(const quint8 nDeviceId, bool bSuccessfully)
{
    MotorData * pData = getMotorDataById(nDeviceId);
    if(pData)
    {
        pData->responseHeart(bSuccessfully);
    }
}

void MotorDataMgr::switchAutoRequestActual(const quint8 nDeviceId, bool bStart)
{
    MotorData * pData = getMotorDataById(nDeviceId);
    if(pData)
    {
        pData->switchAutoRequestActual(bStart);
    }
}

void MotorDataMgr::setAutoRequestInterval(const quint8 nDeviceId, quint32 mSec)
{
    MotorData * pData = getMotorDataById(nDeviceId);
    if(pData)
    {
        pData->setAutoRequestInterval(mSec);
    }
}

qint8 MotorDataMgr::getOldDeviceId(const quint8 nNewDeviceId)
{
    MotorData * pData = getMotorDataByNewId(nNewDeviceId);
    if(pData)
    {
        return pData->deviceId();
    }
    return -1;
}

void MotorDataMgr::saveAllParams(const quint8 nDeviceId)
{
    MotorData * pData = getMotorDataById(nDeviceId);
    if(pData)
    {
        pData->saveAllParams();
    }
}

void MotorDataMgr::clearHomingInfo(const quint8 nDeviceId)
{
    MotorData * pData = getMotorDataById(nDeviceId);
    if(pData)
    {
        pData->clearHomingInfo();
    }
}

void MotorDataMgr::setHomingOperationMode(const quint8 nDeviceId, const quint8 nMode)
{
    MotorData * pData = getMotorDataById(nDeviceId);
    if(pData)
    {
        pData->setHomingOperationMode(nMode);
    }
}

void MotorDataMgr::openChartChannel(quint8 nDeviceId, const int nChannelId)
{
    MotorData * pData = getMotorDataById(nDeviceId);
    if(pData)
    {
        pData->openChartChannel(nChannelId);
    }
}

void MotorDataMgr::closeChartChannel(quint8 nDeviceId, const int nChannelId)
{
    MotorData * pData = getMotorDataById(nDeviceId);
    if(pData)
    {
        pData->closeChartChannel(nChannelId);
    }
}

void MotorDataMgr::switchChartAllChannel(quint8 nDeviceId, bool bOn)
{
    MotorData * pData = getMotorDataById(nDeviceId);
    if(pData)
    {
        pData->switchChartAllChannel(bOn);
    }
}

void MotorDataMgr::switchCalibrationVel(quint8 nDeviceId, quint8 nValue)
{
    MotorData * pData = getMotorDataById(nDeviceId);
    if(pData)
    {
        pData->switchCalibrationVel(nValue);
    }
}

void MotorDataMgr::switchCalibration(quint8 nDeviceId, quint8 nValue)
{
    MotorData * pData = getMotorDataById(nDeviceId);
    if(pData)
    {
        pData->switchCalibration(nValue);
    }
}

void MotorDataMgr::startCalibration(quint8 nDeviceId)
{
    MotorData * pData = getMotorDataById(nDeviceId);
    if(pData)
    {
        pData->startCalibration();
    }
}

void MotorDataMgr::requestSuccessfully(const quint8 nDeviceId, const quint8 nProxyId)
{
    MotorData * pData = getMotorDataById(nDeviceId);

    if(pData)
    {
        quint8 nDataId = DataUtil::convertToMotorDataId((Directives)nProxyId);
        switch (nProxyId)
        {
        case D_CLEAR_HOMING:
            pData->setValueByUser(MotorData::POS_MAX_POS,0,false);
            pData->setValueByUser(MotorData::POS_MIN_POS,0,false);
            pData->setValueByUser(MotorData::POS_HOMING,0,false);
            pData->requestSuccessfully(nDataId);
            emit setProxyCallback(nProxyId,true);
            break;
        case D_CLEAR_ERROR:
            pData->setValueByUser(MotorData::ERROR_ID,UserDefine::ERR_NONE,false);
            pData->setValueByUser(MotorData::MODE_ID,MotorData::Mode_Cur,false);//mode change to current automatically
            pData->requestSuccessfully(nDataId);
            emit setProxyCallback(nProxyId,true);
            break;
        case D_SET_MODE:
            pData->requestSuccessfully(nDataId);
            pData->activeModeSuccessfully();
            emit setProxyCallback(nProxyId,true);
            break;
        case D_SET_SWITCH_MOTORS:
        {
            quint8 nRequest = pData->getUserRequestValue(MotorData::MOTOR_SWITCH).toUInt();
            quint8 nBefore = pData->getValue(MotorData::MOTOR_SWITCH).toUInt();
            if(nRequest==UserDefine::MOTOR_SWITCH_ON && nBefore==UserDefine::MOTOR_SWITCH_OFF)//user open motor
            {
                QTimer::singleShot(3000,[=]{pData->requestAllValue();});
            }
            else if(nRequest==UserDefine::MOTOR_SWITCH_OFF && nBefore==UserDefine::MOTOR_SWITCH_ON)//user close motor
            {
                pData->switchAutoRequestActual(false);
            }
            pData->requestSuccessfully(nDataId);
        }

            break;
        case D_SET_DEVICE_ID:
            pData = getMotorDataByNewId(nDeviceId);//mototor device is has changed, we need find motorData by new id.
            if(pData)
            {
                pData->requestSuccessfully(nDataId);
                emit setProxyCallback(nProxyId,true);
            }
            break;
        default:
            emit setProxyCallback(nProxyId,true);
            pData->requestSuccessfully(nDataId);
            break;
        }
    }
}
void MotorDataMgr::reconnect(quint8 nDeviceId)
{
    MotorData * pData = getMotorDataById(nDeviceId);
    if(pData)
    {
        pData->reconnect();
    }
}
void MotorDataMgr::clearError(quint8 nDeviceId)
{
    MotorData * pData = getMotorDataById(nDeviceId);
    if(pData)
    {
        pData->setValueByUser(MotorData::ERROR_ID,UserDefine::ERR_NONE);
    }
}

void MotorDataMgr::sendCmd(quint8 nDeviceId, quint16 cmdId)
{
    MotorData * pData = getMotorDataById(nDeviceId);
    if(pData)
    {
        if(cmdId < DIRECTIVES_INVALID)
        {
            InnfosProxy::SendProxy(nDeviceId,cmdId);
        }
    }
}

void MotorDataMgr::sendCmd(quint8 nDeviceId, quint16 cmdId, quint8 value)
{
    MotorData * pData = getMotorDataById(nDeviceId);
    if(pData)
    {
        if(cmdId < DIRECTIVES_INVALID)
        {
            InnfosProxy::SendProxy(nDeviceId,cmdId,value);
        }
    }
}

MotorData *MotorDataMgr::getMotorDataById(const quint8 nId) const
{
    for(int i=0;i<m_allMotorDatas.size();++i)
    {
        MotorData * pData = m_allMotorDatas.at(i);
        if(pData->deviceId() == nId)
            return pData;
    }
    return nullptr;
}

MotorData *MotorDataMgr::getMotorDataByNewId(const quint8 nId) const
{
    for(int i=0;i<m_allMotorDatas.size();++i)
    {
        MotorData * pData = m_allMotorDatas.at(i);
        if(pData->requestDeviceId() == nId)
            return pData;
    }
    return nullptr;
}

MotorData *MotorDataMgr::getMotorDataByMac(const quint32 nMac) const
{
    for(int i=0;i<m_allMotorDatas.size();++i)
    {
        MotorData * pData = m_allMotorDatas.at(i);
        if(pData->deviceMac() == nMac)
            return pData;
    }
    return nullptr;
}

bool MotorDataMgr::checkIdUnique(QMap<quint8, quint32> dataMap) const
{
    return dataMap.keys().size() == dataMap.uniqueKeys().size();
}

void MotorDataMgr::handleUnuiqueError(QMap<quint8, quint32> dataMap)
{
    QList<quint8> keys = dataMap.uniqueKeys();
    for(int i=0;i<keys.size();++i)
    {
        if(dataMap.values(keys.at(i)).size() > 1)
        {
            errorOccured(0,UserDefine::ERR_ID_UNUNIQUE,tr("There are at least two motors with the same ID:%1,\n please check and set suitably!").arg(keys.at(i)));
            break;
        }
    }
}
