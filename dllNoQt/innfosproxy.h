﻿#ifndef INNFOSPROXY_H
#define INNFOSPROXY_H


#include <QObject>
#include <QVector>
#include "idata.h"
#include <QTime>
#include <QByteArray>
#include <QMutex>

class QFile;
class QTextStream;

//协议指令符
enum Directives
{
    D_HANDSHAKE=0x00,
    D_READ_VERSION=0x01,
    D_READ_ADDRESS=0x02,
    D_READ_CONFIG=0x03,
    D_READ_CUR_CURRENT=0x04,
    D_READ_CUR_VELOCITY=0x05,
    D_READ_CUR_POSITION=0x06,
    D_SET_MODE=0x07,
    D_SET_CURRENT=0x08,//设置当前q轴电流
    D_SET_VELOCITY=0x09,
    D_SET_POSITION=0x0a,
    D_SET_PAIRS=0x0b,
    D_SET_CURRENT_ID=0x0c,//设置当前d轴电流
    D_SAVE_PARAM=0x0d,
    D_SET_CURRENT_P=0x0e,//电流环的p
    D_SET_CURRENT_I=0x0f,
    D_SET_VELOCITY_P=0x10,
    D_SET_VELOCITY_I=0x11,
    D_SET_POSITION_P=0x12,
    D_SET_POSITION_I=0x13,
    D_SET_POSITION_D=0x14,
    D_READ_CUR_P=0x15,
    D_READ_CUR_I=0x16,
    D_READ_VEL_P=0x17,
    D_READ_VEL_I=0x18,
    D_READ_POS_P=0x19,
    D_READ_POS_I=0x1a,
    D_READ_POS_D=0x1b,
    D_READ_PROFILE_POS_MAX_SPEED=0x1c,
    D_READ_PROFILE_POS_ACC=0x1d,
    D_READ_PROFILE_POS_DEC=0x1e,
    D_SET_PROFILE_POS_MAX_SPEED=0x1f,
    D_SET_PROFILE_POS_ACC=0x20,
    D_SET_PROFILE_POS_DEC=0x21,
    D_READ_PROFILE_VEL_MAX_SPEED=0x22,
    D_READ_PROFILE_VEL_ACC=0x23,
    D_READ_PROFILE_VEL_DEC=0x24,
    D_SET_PROFILE_VEL_MAX_SPEED=0x25,
    D_SET_PROFILE_VEL_ACC=0x26,
    D_SET_PROFILE_VEL_DEC=0x27,
    D_READ_CURRENT_MAXSPEED=0x28,
    D_SET_CURRENT_MAXSPEED=0x29,
    D_SET_CURRENT_PID_MIN = 0x2e,//设置电流环的pid的上下限
    D_SET_CURRENT_PID_MAX=0x2f,
    D_SET_VELOCITY_PID_MIN=0x30,
    D_SET_VELOCITY_PID_MAX=0x31,
    D_SET_POSITION_PID_MIN=0x32,
    D_SET_POSITION_PID_MAX=0x33,
    D_READ_CURRENT_PID_MIN=0x34,//读取电流环的pid的上下限
    D_READ_CURRENT_PID_MAX=0x35,
    D_READ_VELOCITY_PID_MIN=0x36,
    D_READ_VELOCITY_PID_MAX=0x37,
    D_READ_POSITION_PID_MIN=0x38,
    D_READ_POSITION_PID_MAX=0x39,
    D_READ_CHANNEL_2=0x3a,
    D_READ_CHANNEL_3=0x3b,
    D_READ_CHANNEL_4=0x3c,
    D_SET_DEVICE_ID=0x3d,
    D_SOFTWARE_CLOSE=0x3e,
    D_SET_CHART_THRESHOLD=0x3f,
    D_SET_CHART_FREQUENCY=0x40,
    D_READ_CHART_THRESHOLD=0x41,
    D_READ_CHART_FREQUENCY=0x42,
    D_CHART_DATA_STATR=0x43,
    D_CAN_CONNECT=0x44,
    D_READ_VOLTAGE=0x45,
    D_CHART_OPEN=0x46,
    D_CHART_CLOSE=0x47,
    D_CHANNEL2_OPEN=0x48,
    D_CHANNEL2_CLOSE=0x49,
    D_CHANNEL3_OPEN=0x4a,
    D_CHANNEL3_CLOSE=0x4b,
    D_CHANNEL4_OPEN=0x4c,
    D_CHANNEL4_CLOSE=0x4d,
    D_READ_CHANNEL_1=0x4e,
    D_SET_VOLTAGE=0x4f,
    D_CRC_ERROR=0x50,
    D_CHANNEL1_OPEN=0x51,
    D_CHANNEL1_CLOSE=0x52,
    D_READ_CURRENT_SCALE=0x53,
    D_SET_CUR_TRIGGER_MODE=0x54,
    D_READ_MOTOR_MODE=0x55,
    D_READ_TEMP_MOTOR=0x5f,
    D_READ_TEMP_INVERTER=0x60,
    D_SET_TEMP_PROTECT=0x61,
    D_READ_TEMP_PROTECT=0x62,
    D_SET_TEMP_RECOVERY=0x63,
    D_READ_TEMP_RECOVERY=0x64,

    D_SET_FILTER_C_STATUS=0x70,
    D_READ_FILTER_C_STATUS=0x71,
    D_SET_FILTER_C_VALUE=0x72,
    D_READ_FILTER_C_VALUE=0x73,
    D_SET_FILTER_V_STATUS=0x74,
    D_READ_FILTER_V_STATUS=0x75,
    D_SET_FILTER_V_VALUE=0x76,
    D_READ_FILTER_V_VALUE=0x77,
    D_SET_FILTER_P_STATUS=0x78,
    D_READ_FILTER_P_STATUS=0x79,
    D_SET_FILTER_P_VALUE=0x7a,
    D_READ_FILTER_P_VALUE=0x7b,
    D_SET_INERTIA = 0x7c,
    D_READ_INERTIA = 0x7d,
    D_SET_LOCK_ENERGY=0x7e,
    D_READ_LOCK_ENERGY=0x7f,

    D_SET_MAX_POS=0x83,//上下限
    D_SET_MIN_POS=0x84,
    D_READ_MAX_POS=0x85,
    D_READ_MIN_POS=0x86,
    D_SET_HOMING_POS=0x87,
    D_CLEAR_HOMING=0x88,//清除homing相关信息
    D_SET_POS_OFFSET=0x89,
    D_READ_POS_OFFSET=0x8a,
    D_READ_HOMING_LIMIT=0x8b,
    D_SET_HOMING_LIMIT=0x8c,
    D_SET_HOMING_OPERATION=0x8d,
    D_SET_HOMING_MIN=0x8e,
    D_SET_HOMING_MAX=0x8f,
    D_SET_HOMING_CUR_MIN=0x90,
    D_SET_HOMING_CUR_MAX=0x91,
    D_READ_HOMING_CUR_MIN=0x92,
    D_READ_HOMING_CUR_MAX=0x93,
    D_SWITCH_CALIBRATION=0xa0,
    D_READ_CALIBRATION_SWITCH=0xa1,
    D_START_CALIBRATION=0xa2,
    D_SET_CALIBRATION_ANGLE=0xa3,
    D_READ_CALIBRATION_ANGLE=0xa4,
    D_SWITCH_CALIBRATION_VEL=0xa5,

    D_READ_LAST_STATE=0xb0,//读取上一次状态（是否正常关机）
    D_SET_SWITCH_MOTORS=0x2a,
    D_READ_MOTORS_SWITCH=0x2b,
    D_CLEAR_ERROR=0xfe,//清理错误
    D_CHECK_ERROR=0xff,//错误提示
    DIRECTIVES_INVALID,
};


//串口协议(有数据有校验)
class InnfosProxy : public QObject
{
    Q_OBJECT
public:
    IData * m_pData;
public slots:
    void Send();
public:
    InnfosProxy(const int nId,const quint8 nDeviceId);
    virtual ~InnfosProxy();
    static void decode(quint8 communicateUnitId,QByteArray &buf);
    const int GetIdx()const { return m_nId; }
    static void SendProxy(const quint8 nDeviceId,const int nIdx);
    static void SendProxy(const quint8 nDeviceId,const quint32 nDeviceMac,const int nIdx,const quint8 newDeviceId);
    template <class T>
    static void SendProxy(const quint8 nDeviceId,const int nIdx,T data){
        InnfosProxy proxy(nIdx,nDeviceId);
        proxy.m_pData->WriteShort(sizeof(T));
        proxy.m_pData->WriteData(data);
        proxy.m_pData->AddCRCCode(sizeof(T));
        proxy.m_pData->AddProxyEnd();
        proxy.Send();
    }
    static void SendProxyScale(const quint8 nDeviceId,const int nIdx,qreal data,int nScale=(1<<24)){//actural vaue will be scale 1<<24
        int nScaleValue = data*nScale;
        SendProxy(nDeviceId,nIdx,nScaleValue);
    }
    static void SendQrealProxy(const quint8 nDeviceId,const int nIdx,qreal data);//data is qreal,will be scale
protected:
private:
    int m_nId;//协议id
protected:
};

//无数据协议，主要是为了获取一些数据
class NoDataProxy : public InnfosProxy
{
    Q_OBJECT
public slots:
public:
    NoDataProxy(const int nId, const quint8 nDeviceId);
    static void decode(quint8 communicateUnitId,QByteArray &buf);
protected:
private:
};

//有数据无校验
class NoCrcProxy : public InnfosProxy
{
    Q_OBJECT
public slots:
public:
    NoCrcProxy(const int nId, const quint8 nDeviceId);
    static void decode(quint8 communicateUnitId,QByteArray &buf);
protected:
private:
};

//proxy watcher,use to log those proxys without response
class ProxyWatcher : public QObject
{
    Q_OBJECT
public:
    static ProxyWatcher * getInstance();
    static void autoDestroy();
    void addSendItem(const QByteArray & buf);
    void reciveItem(const QByteArray & buf);
    virtual ~ProxyWatcher();
protected:
    void checkTimeout();
    ProxyWatcher(QObject *parent=Q_NULLPTR);
    struct logStruct{
        QTime sendTime;
        QByteArray sendContent;
    };
private:
    static ProxyWatcher * m_pWatcher;
    QVector<logStruct *> m_sendContents;
    QFile * m_pLogFile;
    QTextStream * m_pLogStream;
    QMutex m_qmMutex;
};
#endif // INNFOSPROXY_H
