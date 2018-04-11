#include "mediator.h"
#include "communication.h"
#include "EthernetAutoRecognize.h"
#include "proxyparser.h"
#include "dataUtil.h"
#include "innfosproxy.h"
#include <QDebug>
#include "motordata.h"
#include <QVersionNumber>

Mediator * Mediator::m_pInstance = nullptr;
Mediator *Mediator::getInstance()
{
    if(!m_pInstance)
    {
        m_pInstance = new Mediator;
    }

    return m_pInstance;
}

Mediator::~Mediator()
{
    delete m_pVersionMgr;
    delete m_pCommunication;
    delete m_pRecognize;
}


void Mediator::autoRecognize()
{
#ifdef TEST_DEBUG
    connect(Communication::getInstance(),&Communication::request,ProxyWatcher::getInstance(),&ProxyWatcher::addSendItem);
#endif
    m_pCommunication->stop();//断掉已有连接
    m_pRecognize->startRecognize();
}

void Mediator::onCanConnected(quint32 nCommunicationUnitId)
{
    m_pCommunication->setUnitConnectionStatus(nCommunicationUnitId,UserDefine::CAN_CONNECTED);
}

void Mediator::SendRequest(const QByteArray &buf)
{
    quint8 nId = buf.at(1);
    m_pCommunication->sendData(nId,buf);
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
        m_pCommunication->changeUnitRelateId(motorDataMgrInstance->getOldDeviceId(nDeviceId),nDeviceId);
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
    m_pRecognize->addMototInfo(nDeviceId,nDeviceMac);
    m_pCommunication->addRelateIdToUnit(communicateUnitId,nDeviceId);
    m_pCommunication->setUnitConnectionStatus(communicateUnitId,UserDefine::CAN_CONNECTED|UserDefine::MOTOR_CONNECTED);
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
    m_pCommunication->removeUnAvailablePorts();
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

QString Mediator::versionString() const
{
    return m_pVersionMgr->toString();
}

int Mediator::addCommunicationUnit(QString unitStr, quint32 unitNumber)
{
    if(m_pCommunication)
        return m_pCommunication->addCommunication(unitStr,unitNumber);
    return -1;
}

void Mediator::setUnitConnectionStatus(quint32 nUnitId, quint8 nStatus)
{
    m_pCommunication->setUnitConnectionStatus(nUnitId,nStatus);
}

void Mediator::initCommunication(int nType)
{
    m_pCommunication = new Communication(nType);
    m_pRecognize = AbstractAutoRecongnize::getInstance(nType);
    connect(m_pCommunication,&Communication::response,this,&Mediator::response);
    connect(m_pCommunication,&Communication::connectionError,this,&Mediator::errorOccur);
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
    QObject(parent),
    m_pVersionMgr(nullptr),
    m_pCommunication(nullptr),
    m_pRecognize(nullptr)
{

    m_pVersionMgr = new QVersionNumber(1,0,3);
}


