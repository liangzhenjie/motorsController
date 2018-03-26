#include "mediator.h"
#include "communication.h"
#include "autorecoginze.h"
#include "proxyparser.h"
#include "dataUtil.h"
#include "innfosproxy.h"
#include <QDebug>
#include "motordata.h"

Mediator * Mediator::m_pInstance = nullptr;
Mediator *Mediator::getInstance()
{
    if(!m_pInstance)
    {
        m_pInstance = new Mediator;
    }

    return m_pInstance;
}


void Mediator::autoRecognize()
{
#ifdef TEST_DEBUG
    connect(Communication::getInstance(),&Communication::request,ProxyWatcher::getInstance(),&ProxyWatcher::addSendItem);
#endif
    AutoRecognize::getInstance()->startRecognize(true);
}

void Mediator::onCanConnected(quint32 nCommunicationUnitId)
{
    Communication::getInstance()->setUnitConnectionStatus(nCommunicationUnitId,UserDefine::CAN_CONNECTED);
}

void Mediator::SendRequest(const QByteArray &buf)
{
    quint8 nId = buf.at(1);
    Communication::getInstance()->sendData(nId,buf);
}

void Mediator::Handshake(quint32 nDeviceId, bool bSuccess)
{
    motorDataMgrInstance->responseHeart(nDeviceId,bSuccess);
}

void Mediator::SetCurParam(const int nDeviceID, const QVariant value, const int nProxyId)
{
    motorDataMgrInstance->setMotorDataAttrByProxy(nDeviceID,nProxyId,value);//data from device
}

void Mediator::SetSucceed(const quint8 nDeviceId, const int nProxyId)
{

    switch (nProxyId) {
    case D_SET_DEVICE_ID:
        Communication::getInstance()->changeUnitRelateId(motorDataMgrInstance->getOldDeviceId(nDeviceId),nDeviceId);
        motorDataMgrInstance->requestSuccessfully(nDeviceId,nProxyId);
        break;
    default:
        motorDataMgrInstance->requestSuccessfully(nDeviceId,nProxyId);
        break;
    }
    m_sRequestBack.s_Emit(nDeviceId,nProxyId,1);
}

void Mediator::SetFailed(const int nParam)
{
    Q_UNUSED(nParam);
}

void Mediator::reciveMotorInfo(quint32 communicateUnitId, const quint32 nDeviceMac, const quint8 nDeviceId)
{
    qDebug() << "MotorInfo" << communicateUnitId << nDeviceId;
    AutoRecognize::getInstance()->addMototInfo(nDeviceId,nDeviceMac);
    Communication::getInstance()->addRelateIdToUnit(communicateUnitId,nDeviceId);
    Communication::getInstance()->setUnitConnectionStatus(communicateUnitId,UserDefine::CAN_CONNECTED|UserDefine::MOTOR_CONNECTED);
}

void Mediator::receiveNoDataProxy(const int nDeviceID)
{
    switch (nDeviceID) {
    case D_CHART_DATA_STATR:
        m_sNewChartStart.s_Emit();
        break;
    default:
        break;
    }
}

//void Mediator::saveDataToDevice()
//{
//    MotorData * pMotor = MotorMgr::getInstance()->getMotorByDeviceId(nDeviceId);
//    if(pMotor)
//        InnfosProxy::SendProxy(pMotor->deviceId(),D_SAVE_PARAM);
//    QVector<MotorData *> selMotors = MotorMgr::getInstance()->allSelectedMotors();
//    foreach (MotorData * pMotor, selMotors)
//    {
//       InnfosProxy::SendProxy(pMotor->deviceId(),D_SAVE_PARAM);
//    }
//}


void Mediator::recognizeFinished(QMap<quint8, quint32> motorsInfo)
{
    motorDataMgrInstance->AddMotorsData(motorsInfo);//add to logic manager
    Communication::getInstance()->removeUnAvailablePorts();
    m_sRecognizeFinished.s_Emit();
}

void Mediator::chartVauleChange(const int nChannelId, qreal values)
{
    m_sChartValueChange.s_Emit(nChannelId,values);
}

void Mediator::receiveQuaternion(quint8 imuId, double w, double x, double y, double z)
{
    m_sQuaternion.s_Emit(imuId,w,x,y,z);
}

void Mediator::checkServosStatus()
{
    //MotorMgr::getInstance()->CheckServosSwitch(); //to do
}

void Mediator::response(quint32 nUnitId, const QByteArray buf)
{
    ProxyParser::getInstance()->parse(nUnitId,buf);
}

void Mediator::reconnectDevice(quint8 nDeviceId)
{
    motorDataMgrInstance->setMotorDataAttrByUser(nDeviceId,MotorData::ONLINE_STATUS,UserDefine::Status_Online);
}

void Mediator::errorOccur(quint8 nDeviceId, quint16 errorId, QString errorStr)
{
    m_sError.s_Emit(nDeviceId,errorId,errorStr.toStdString());
}

void Mediator::motorAttrChanged(quint8 nDeviceId, quint8 nAttrId, QVariant value)
{
    m_sMotorAttrChanged.s_Emit(nDeviceId,nAttrId,value.toDouble());
}

Mediator::Mediator(QObject *parent):
    QObject(parent)
{
    connect(Communication::getInstance(),&Communication::response,this,&Mediator::response);
    connect(Communication::getInstance(),&Communication::connectionError,this,&Mediator::errorOccur);

}


