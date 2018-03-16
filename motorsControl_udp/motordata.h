#ifndef MOTORDATA_H
#define MOTORDATA_H

#include <QObject>
#include <QMap>
#include <QVector>
#include <QVariant>
#include <QTimer>
#include <QXmlStreamReader>
#include <QFile>
#include <QList>

class MotorData : public QObject
{
    Q_OBJECT

public:
    enum Motor_Mode{
        Mode_None,
        Mode_Cur,
        Mode_Vel,
        Mode_Pos,
        Mode_Teaching,
        Mode_Profile_Pos=6,
        Mode_Profile_Vel,
        Mode_Homing,
    };

    enum Motor_Data_Id{
        CUR_IQ_SET,//电流IQ值
        CUR_PROPORTIONAL,//电流比例
        CUR_INTEGRAL,//电流积分
        CUR_ID_SET,//电流微分
        CUR_MINIMAL,//预留
        CUR_MAXIMUM,//预留
        CUR_NOMINAL,//预留
        CUR_OUTPUT,//预留
        CUR_MAXSPEED,//电流环最大速度
        CUR_ACTURAL,//当前电流值
        CUR_DEMAND,//预留
        VEL_SET,//速度设置
        VEL_PROPORTIONAL,//速度比例
        VEL_INTEGRAL,//速度积分
        VEL_MINIMAL,//速度环最小电流比例
        VEL_MAXIMUM,//速度环最大电流比例
        VEL_ACTURAL,//速度值
        VEL_DEMAND,//预留
        POS_SET,//位置设置
        POS_PROPORTIONAL,//位置比例
        POS_INTEGRAL,//位置积分
        POS_DIFFERENTIAL,//位置微分
        POS_MINIMAL,//位置环最小速度比例
        POS_MAXIMUM,//位置环最大速度比例
        POS_STEP_ADD,//预留
        POS_MIN_POS,//最小位置限制
        POS_MAX_POS,//最大位置限制
        POS_HOMING,//归零位置
        POS_ACTURAL,//当前位置
        POS_DEMAND,//预留
        PROFILE_POS_MAX_SPEED,//profile position模式最大速度
        PROFILE_POS_ACC,//profile position模式加速度
        PROFILE_POS_DEC,//profile position模式减速速度
        PROFILE_VEL_MAX_SPEED,//profile velocity模式最大速度
        PROFILE_VEL_ACC,//profile velocity模式加速度
        PROFILE_VEL_DEC,//profile velocity模式减速速度
        CHART_FREQUENCY,//图像频率
        CHART_THRESHOLD,//图像阈值
        CHART_SWITCH,//图像开关
        POS_OFFSET,//位置偏移
        VOLTAGE,//电压
        HOMING_LIMIT,//开启或关闭位置限制
        HOMING_CUR_MAX,//归零最大电流
        HOMING_CUR_MIN,//归零最小小电流
        CURRENT_SCALE,//物理最大电流值
        VELOCITY_SCALE,//速度最大电流值
        FILTER_C_STATUS,//电流环滤波是否开启
        FILTER_C_VALUE,//电流环滤波值
        FILTER_V_STATUS,//速度环滤波是否开启
        FILTER_V_VALUE,//速度环滤波值
        FILTER_P_STATUS,//位置环滤波是否开启
        FILTER_P_VALUE,//位置环滤波值
        INERTIA,//惯量
        LOCK_ENERGY,//堵转保护能力
        TEMP_MOTOR,//执行器温度
        TEMP_INVERTER,//逆变器温度
        TEMP_PROTECT,//保护温度
        TEMP_RECOVERY,//恢复温度
        CALIBRATION_SWITCH,//预留
        CALIBRATION_ANGLE,//预留
        MOTOR_SWITCH,//执行器开关机
        MOTOR_VERSION,//执行器固件版本
        ONLINE_STATUS,//执行器是否在线
        DEVICE_ID,//执行器Id
        MAC_ID,//执行器SN号
        MODE_ID,//执行器当前模式
        ERROR_ID,//错误代码
        DATA_CNT,
        DATA_CHART,//预留
        DATA_INVALID,
    };

    explicit MotorData(const quint8 nDeviceId,const quint32 nDeviceMac,QObject *parent = 0);
    quint8 deviceId() const;
    qint8 requestDeviceId()const;
    quint32 deviceMac()const;
    void setValueByProxy(const int nProxyId,QVariant value);//save value that recive
    void requestAllValue();
    QVariant getValue(Motor_Data_Id nDataId)const;
    QVariant getUserRequestValue(Motor_Data_Id nDataId)const;
    void setValueByUser(Motor_Data_Id nDataId,QVariant value,bool bSendProxy=true);//set value by user, will send proxy or not
    void responseHeart(bool bSuccess);
    bool isOnline()const;
    QVector<quint16> errorHistory()const{
        return m_errorHistory;
    }
    Motor_Mode currentMode()const;
    ~MotorData();

    bool deviceIdIsAvailable()const;
    void changeDemand(qreal value);//demand value change
    void switchAutoRequestActual(bool bStart);
    void setAutoRequestInterval(quint32 mSec);

    void saveAllParams();
    void clearHomingInfo();
    void setHomingOperationMode(const quint8 nMode);
    void openChartChannel(const int nChannelId);
    void closeChartChannel(const int nChannelId);
    void switchChartAllChannel(bool bOn);
    void switchCalibrationVel(quint8 nValue);
    void switchCalibration(quint8 nValue);
    void startCalibration();
    void requestSuccessfully(quint8 nDataId);
protected:
    void setValue(int nDataId,QVariant value,bool bEmitSignal = true);
    void userRequestValue(int nDataId,QVariant value);
private:
    void initData();
public slots:
    void activeModeSuccessfully();
    void saveData();
    void loadData();
    void reconnect();

private slots:
    void requestActualValue();//request value initiatively
private:
    void saveDataToFile(QString fileName);
    void readDataFromFile(QString fileName);
    void readParams(QXmlStreamReader *reader);
signals:
    void acturalVauleChange(Motor_Mode Id,QVector<qreal> values);//only use by chart
    void currentErrorChange(const int nErrorId);
private:
private:
    QVariant m_motorData[DATA_CNT];
    QVariant m_userRequestData[DATA_CNT];
    int m_nHeartFailCnt;//
    QTimer * m_pHeartTimer;
    QTimer * m_pValueTimer;
    QVector<quint16> m_errorHistory;

    quint32 m_nAutoRequestInterval;
    //log file
    QFile * m_pFileLog;
};


//motordata manger
#define motorDataMgrInstance MotorDataMgr::getInstance()

class MotorDataMgr : public QObject
{
    Q_OBJECT
public:
    static MotorDataMgr * getInstance();
    virtual ~MotorDataMgr();

    qreal getMotorDataAttrValueAsDouble(const quint8 nDeviceId,const MotorData::Motor_Data_Id attrId)const;
    qint32 getMotorDataAttrValueAsInt(const quint8 nDeviceId,const MotorData::Motor_Data_Id attrId)const;
    void setMotorDataAttrByUser(const quint8 nDeviceId,const MotorData::Motor_Data_Id attrId,QVariant value,bool bSend=true);
    void setMotorDataAttrByProxy(const quint8 nDeviceId,int proxyId,QVariant value);//data from proxy
    void setMotorDataAttrInBatch(const QList<quint8> idList,const MotorData::Motor_Data_Id attrId,QVariant value,bool bSend=true);
    void AddMotorsData(QMap<quint8,quint32> dataMap);
    QMap<quint8,quint32> getMotorsMap()const;
    QVector<quint16> motorErrorHistory(const quint8 nDeviceId)const;
    bool deviceIdHasExist(quint8 nDeviceId);
    void activeMotorModeSuccessfully(const quint8 nDeviceId);
    void activeMotorModeInBatch(const QList<quint8> idList,const MotorData::Motor_Mode mode);
    void regainAllData(const quint8 nDeviceId);
    void regainData(const quint8 nDeviceId,MotorData::Motor_Data_Id attrId);
    void responseHeart(const quint8 nDeviceId,bool bSuccessfully);

    void switchAutoRequestActual(const quint8 nDeviceId,bool bStart);
    void setAutoRequestInterval(const quint8 nDeviceId,quint32 mSec);
    qint8 getOldDeviceId(const quint8 nNewDeviceId);

    void saveAllParams(const quint8 nDeviceId);
    void clearHomingInfo(const quint8 nDeviceId);
    void setHomingOperationMode(const quint8 nDeviceId,const quint8 nMode);
    void openChartChannel(quint8 nDeviceId, const int nChannelId);
    void closeChartChannel(quint8 nDeviceId, const int nChannelId);
    void switchChartAllChannel(quint8 nDeviceId,bool bOn);
    void switchCalibrationVel(quint8 nDeviceId,quint8 nValue);
    void switchCalibration(quint8 nDeviceId,quint8 nValue);
    void startCalibration(quint8 nDeviceId);
    void requestSuccessfully(const quint8 nDeviceId,const quint8 nProxyId);
    void reconnect(quint8 nDeviceId);
    void clearError(quint8 nDeviceId);
    void sendCmd(quint8 nDeviceId,quint16 cmdId);
private:
    MotorDataMgr();
signals:
    void dataChanged(const quint8 nDeviceId,const MotorData::Motor_Data_Id attrId,QVariant value);
    void errorOccured(const quint8 nDeviceId,const quint16 erroId,QString errorInfo);
    void setProxyCallback(const quint8 nProxyId,bool bSuccess);
private:
    class GC{
    public:
        ~GC()
        {
            if(m_pMgr!=nullptr)
            {
                delete m_pMgr;
                m_pMgr = nullptr;
            }
        }
        static GC gc;
    };
private:
    MotorData * getMotorDataById(const quint8 nId)const;
    MotorData * getMotorDataByNewId(const quint8 nId)const;//if device id changed by user, sometimes we need to find the motorData using NewId we set before.
    MotorData * getMotorDataByMac(const quint32 nMac)const;
    bool checkIdUnique(QMap<quint8,quint32> dataMap)const;//check all motors's id si unique or not, if not warning and exit the app
    void handleUnuiqueError(QMap<quint8,quint32> dataMap);
    QVariant getMotorDataAttrValue(const quint8 nDeviceId,const MotorData::Motor_Data_Id attrId)const;
private:
    QList<MotorData *> m_allMotorDatas;
    static MotorDataMgr * m_pMgr;
};


#endif // MOTORDATA_H
