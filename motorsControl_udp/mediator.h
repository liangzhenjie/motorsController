#ifndef MEDIATOR_H
#define MEDIATOR_H
#include <QObject>
#include "motordata.h"
#include "userdefine.h"
#include <functional>
#include <string>
#include "CSignal.hpp"
//#define TEST_DEBUG
#define LOG_DEBUG
//#define NO_HEART_BEAT
//#define WARNING_DEBUG


const qreal velScale = 6000;
const qreal curScale = 8.25;


#define mediator  Mediator::getInstance()//get instance of Mediator
#define requestCallback  std::function<void (uint8_t,uint8_t,double)>
#define errorInfoFunc std::function<void (uint8_t,uint16_t,std::string)>

class Mediator : public QObject
{
    Q_OBJECT
public:
    static Mediator * getInstance();
    void autoRecognize();//auto recognize motor
    void onCanConnected(quint32 nCommunicationUnitId);
    void SendRequest(const QByteArray & buf);
    void Handshake(quint32 nDeviceId,bool bSuccess);
    void SetCurParam(const int nDeviceID,const QVariant value, const int nProxyId);//set motor param values
    void SetSucceed(const quint8 nDeviceId, const int nProxyId);//
    void SetFailed(const int nParam);//设置下位机参数fail
    //void NullChartPointer();

    void reciveMotorInfo(quint32 communicateUnitId,const quint32 nDeviceMac, const quint8 nDeviceId);
    void receiveNoDataProxy(const int nDeviceID);
    void checkServosStatus();//check servos are on or off
    void recognizeFinished(QMap<quint8,quint32> motorsInfo);
    void chartVauleChange(const int nChannelId,qreal values);//only use by chart
    void receiveQuaternion(quint8 imuId, double w,double x,double y,double z);
public slots:
    void response(quint32 nUnitId,const QByteArray buf);
    void reconnectDevice(quint8 nDeviceId);
    void errorOccur(quint8 nDeviceId,quint16 errorId, QString errorStr);
    void motorAttrChanged(quint8 nDeviceId,quint8 nAttrId,QVariant value);
protected:
    Mediator(QObject * parent=0);
private:
    static Mediator *m_pInstance;
private:
    class GC{
    public:
        ~GC()
        {
            if(m_pInstance!=nullptr)
            {
                delete m_pInstance;
                m_pInstance = nullptr;
            }
        }
        static GC gc;
    };
public:
    CSignal<> m_sRecognizeFinished;
    CSignal<uint8_t,uint8_t,double> m_sRequestBack;
    CSignal<uint8_t,uint16_t,std::string> m_sError;
    CSignal<uint8_t,uint8_t,double> m_sMotorAttrChanged;
    CSignal<> m_sNewChartStart;
    CSignal<uint8_t,double> m_sChartValueChange;
    CSignal<uint8_t,double,double,double,double> m_sQuaternion;
};

#endif // MEDIATOR_H
