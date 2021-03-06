﻿#include "motorscontroller.h"
#include <QDebug>
#include "innfosproxy.h"
#include <QTimer>

QCoreApplication * MotorsController::m_pQtCore = nullptr;
MotorsController * MotorsController::m_pInstance = nullptr;

MotorsController::MotorsController():
    QObject()
{
    m_lConnectionIds.push_back(mediator->m_sRequestBack.connect_member(this,&MotorsController::onRequestCallback));
    m_lConnectionIds.push_back(mediator->m_sRecognizeFinished.connect_member(this,&MotorsController::finishRecognizeCallback));
    m_lConnectionIds.push_back(mediator->m_sError.connect_member(this,&MotorsController::errorOccur));
    m_lConnectionIds.push_back(mediator->m_sMotorAttrChanged.connect_member(this,&MotorsController::motorAttrChanged));
    m_lConnectionIds.push_back(mediator->m_sChartValueChange.connect_member(this,&MotorsController::chartValueChange));
    m_lConnectionIds.push_back(mediator->m_sNewChartStart.connect_member(this,&MotorsController::startNewChart));
}

void MotorsController::initController(int &argc, char **argv, int nCommunicationType)
{
    if(m_pInstance != nullptr)
    {
        return;
    }
    mediator->initCommunication(nCommunicationType);
    if(QCoreApplication::instance() == nullptr)
        m_pQtCore = new QCoreApplication(argc,argv);
    m_pInstance= new MotorsController();
}

MotorsController *MotorsController::getInstance()
{
    return m_pInstance;
}

void MotorsController::progressEvents()
{
    QCoreApplication::processEvents();
}

MotorsController::~MotorsController()
{
    mediator->m_sRequestBack.s_Disconnect(m_lConnectionIds);
    mediator->m_sRecognizeFinished.s_Disconnect(m_lConnectionIds);
    mediator->m_sError.s_Disconnect(m_lConnectionIds);
    mediator->m_sMotorAttrChanged.s_Disconnect(m_lConnectionIds);
    mediator->m_sChartValueChange.s_Disconnect(m_lConnectionIds);
    mediator->m_sNewChartStart.s_Disconnect(m_lConnectionIds);
    delete m_pQtCore;
    //Mediator::destroyAllStaticObjects();
}

void MotorsController::autoRecoginze()
{
    mediator->autoRecognize();
}

bool MotorsController::hasAvailableMotor() const
{
    return getMotorIdArray().size() > 0;
}

void MotorsController::finishRecognizeCallback()
{
    qDebug()<<"finished";
    QTimer::singleShot(100,[=]{
        qDebug() << "really finished";
        m_sOperationFinished.s_Emit(0,Recognize_Finished);
    }); //delay to insure all requests have acknowledges
}

void MotorsController::onRequestCallback(uint8_t nDeviceId, uint8_t nProxyId, double value)
{
    switch (nProxyId) {

    case D_SAVE_PARAM:
        m_sOperationFinished.s_Emit(nDeviceId,Save_Params_Finished);
        break;
    case D_SET_SWITCH_MOTORS:
        if((int)value == UserDefine::MOTOR_SWITCH_ON)
        {
            QTimer::singleShot(3500,[=]{//等待3.5s,开机动作才完成
                m_sOperationFinished.s_Emit(nDeviceId,Launch_Finished);
            });
        }
        else
        {
            m_sOperationFinished.s_Emit(nDeviceId,Close_Finished);
        }
        break;
    default:
        m_sRequestBack.s_Emit(nDeviceId,nProxyId,value);
        break;
    }
}

void MotorsController::errorOccur(uint8_t nDeviceId, uint16_t errorId, string errorStr)
{
    m_sError.s_Emit(nDeviceId,errorId,errorStr);
}

void MotorsController::motorAttrChanged(uint8_t nDeviceId, uint8_t attrId, double value)
{
    m_sMotorAttrChanged.s_Emit(nDeviceId,attrId,value);
}

void MotorsController::startNewChart()
{
    m_sNewChartStart.s_Emit();
}

void MotorsController::chartValueChange(uint8_t channelId, double value)
{
    m_sChartValueChange.s_Emit(channelId,value);
}

vector<uint8_t> MotorsController::getMotorIdArray() const
{
    QList<quint8> motorKeyList = motorDataMgrInstance->getMotorsMap().uniqueKeys();
    return convertQListToVector(motorKeyList);
}

void MotorsController::activeMotorsMode(vector<uint8_t> idArray, const MotorData::Motor_Mode nMode)
{
    motorDataMgrInstance->activeMotorModeInBatch(convertVectorToQList(idArray),nMode);
}

void MotorsController::launchAllMotors()
{
    QList<quint8> motorKeyList = motorDataMgrInstance->getMotorsMap().uniqueKeys();
    for(int i=0;i<motorKeyList.size();++i)
    {
        motorDataMgrInstance->setMotorDataAttrByUser(motorKeyList[i],MotorData::MOTOR_SWITCH,UserDefine::MOTOR_SWITCH_ON);
    }

}

void MotorsController::closeAllMotors()
{
    QList<quint8> motorKeyList = motorDataMgrInstance->getMotorsMap().uniqueKeys();
    for(int i=0;i<motorKeyList.size();++i)
    {
        motorDataMgrInstance->setMotorDataAttrByUser(motorKeyList[i],MotorData::MOTOR_SWITCH,UserDefine::MOTOR_SWITCH_OFF);
    }
}

void MotorsController::launchMotor(uint8_t id)
{
    motorDataMgrInstance->setMotorDataAttrByUser(id,MotorData::MOTOR_SWITCH,UserDefine::MOTOR_SWITCH_ON);
}

void MotorsController::closeMotor(uint8_t id)
{
    motorDataMgrInstance->setMotorDataAttrByUser(id,MotorData::MOTOR_SWITCH,UserDefine::MOTOR_SWITCH_OFF);
}

void MotorsController::switchAutoRefresh(uint8_t id, bool bOpen)
{
    motorDataMgrInstance->switchAutoRequestActual(id,bOpen);
}

void MotorsController::setAutoRefreshInterval(uint8_t id, uint32_t mSec)
{
    motorDataMgrInstance->setAutoRequestInterval(id,mSec);
}

void MotorsController::setPosition(uint8_t id, double pos)
{
    motorDataMgrInstance->setMotorDataAttrByUser(id,MotorData::POS_SET,pos);
}

void MotorsController::setVelocity(uint8_t id, double vel)
{
    motorDataMgrInstance->setMotorDataAttrByUser(id,MotorData::VEL_SET,vel);
}

void MotorsController::setCurrent(uint8_t id, double current)
{
    motorDataMgrInstance->setMotorDataAttrByUser(id,MotorData::CUR_IQ_SET,current);
}

double MotorsController::getPosition(uint8_t id, bool bRefresh) const
{
    if(bRefresh)
        motorDataMgrInstance->regainData(id,MotorData::POS_ACTURAL);
    return motorDataMgrInstance->getMotorDataAttrValueAsDouble(id,MotorData::POS_ACTURAL);
}

double MotorsController::getVelocity(uint8_t id, bool bRefresh) const
{
    if(bRefresh)
        motorDataMgrInstance->regainData(id,MotorData::VEL_ACTURAL);
    return motorDataMgrInstance->getMotorDataAttrValueAsDouble(id,MotorData::VEL_ACTURAL);
}

double MotorsController::getCurrent(uint8_t id, bool bRefresh) const
{
    if(bRefresh)
        motorDataMgrInstance->regainData(id,MotorData::CUR_ACTURAL);
    return motorDataMgrInstance->getMotorDataAttrValueAsDouble(id,MotorData::CUR_ACTURAL);
}

void MotorsController::setMotorAttribute(uint8_t id, MotorData::Motor_Data_Id attrId, double value)
{
    motorDataMgrInstance->setMotorDataAttrByUser(id,attrId,value);
}

double MotorsController::getMotorAttribute(uint8_t id, MotorData::Motor_Data_Id attrId) const
{
    return motorDataMgrInstance->getMotorDataAttrValueAsDouble(id,attrId);
}

void MotorsController::saveAllParams(uint8_t id)
{
    motorDataMgrInstance->saveAllParams(id);
}

void MotorsController::clearHomingInfo(uint8_t id)
{
    motorDataMgrInstance->clearHomingInfo(id);
}

void MotorsController::setHomingOperationMode(uint8_t id, uint8_t nMode)
{
    motorDataMgrInstance->setHomingOperationMode(id,nMode);
}

void MotorsController::setMinPosLimit(uint8_t id)
{
    motorDataMgrInstance->sendCmd(id,D_SET_HOMING_MIN);
}

void MotorsController::setMaxPosLimit(uint8_t id)
{
    motorDataMgrInstance->sendCmd(id,D_SET_HOMING_MAX);
}

void MotorsController::openChartChannel(uint8_t id, uint8_t nChannelId)
{
    motorDataMgrInstance->openChartChannel(id,nChannelId);
}

void MotorsController::closeChartChannel(uint8_t id, uint8_t nChannelId)
{
    motorDataMgrInstance->closeChartChannel(id,nChannelId);
}

void MotorsController::switchChartAllChannel(uint8_t id, bool bOn)
{
    motorDataMgrInstance->switchChartAllChannel(id,bOn);
}

void MotorsController::setCurrentChartMode(uint8_t id, uint8_t mode)
{
    motorDataMgrInstance->sendCmd(id,D_SET_CUR_TRIGGER_MODE,mode);
}


void MotorsController::regainAttrbute(uint8_t id, uint8_t attrId)
{
    motorDataMgrInstance->regainData(id,(MotorData::Motor_Data_Id)attrId);
}

vector<uint16_t> MotorsController::getErrorHistory(uint8_t id)
{
    QVector<uint16_t> errors = motorDataMgrInstance->motorErrorHistory(id);
    vector<uint16_t> history;
    for(int i=0;i<errors.size();++i)
    {
        history.push_back(errors.at(i));
    }
    return history;
}
void MotorsController::reconnect(uint8_t id)
{
    motorDataMgrInstance->reconnect(id);
}
void MotorsController::clearError(uint8_t id)
{
    motorDataMgrInstance->clearError(id);
}

string MotorsController::versionString() const
{
    return mediator->versionString().toStdString();
}

vector<uint8_t> MotorsController::convertQListToVector(const QList<quint8> &qList) const
{
    vector<uint8_t> idVector;
    for(int i=0;i<qList.size();++i)
    {
        idVector.push_back(qList[i]);
    }
    return idVector;
}

QList<quint8> MotorsController::convertVectorToQList(const vector<uint8_t> &cVector) const
{
    QList<quint8> qList;
    for(int i=0;i<cVector.size();++i)
    {
        qList.push_back(cVector[i]);
    }
    return qList;
}
