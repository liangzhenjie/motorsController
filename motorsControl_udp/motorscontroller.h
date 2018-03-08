﻿#ifndef MOTORSCONTROLLER_H
#define MOTORSCONTROLLER_H

#include "motorscontroll_global.h"
#include <functional>
#include <vector>
#include "mediator.h"
#include "innfosproxy.h"
#include <QCoreApplication>
#include <QObject>

using namespace  std;

#define contorllerInst MotorsController::getInstance()

class MOTORSCONTROLLSHARED_EXPORT MotorsController : public QObject
{
public:
    enum OperationFlags{
        Recognize_Finished,
        Launch_Finished,
        Close_Finished,
        Save_Params_Finished,
        Attribute_Change_Finished,
    };

public:
    ~MotorsController();
    MotorsController();
    /**
     * @brief initContorller 初始化控制器，使用控制器之前必须先初始化
     * @param argc 参数个数
     * @param argv 参数数组
     */
    static void initController(int &argc, char **argv);
    /**
     * @brief getInstance 获取控制器对象
     * @return 控制器对象
     */
    static MotorsController * getInstance();
    /**
     * @brief progressEvents 处理控制器事件
     */
    static void progressEvents();
/**
 * @brief 识别所有可用设备
 * @author liangzhenjie
 * @date 2018/01/09
**/
    void autoRecoginze();
    /**
 * @brief 是否有可用设备
 * @author liangzhenjie
 * @date 2018/01/15
**/
    bool hasAvailableMotor()const;
/**
 * @brief 获取所有设备Id数组
 * @author liangzhenjie
 * @date 2018/01/09
 * @return id数组
**/
    vector<uint8_t> getMotorIdArray()const;
/**
 * @brief 激活设备的指定模式
 * @author liangzhenjie
 * @date 2018/01/09
 * @param idArray 设备id数组
 * @param nMode 要激活的模式
**/
    void activeMotorsMode(vector<uint8_t> idArray, const MotorData::Motor_Mode nMode);
/**
 * @brief 启动所有设备，若设备未启动，需先调用此函数，调用后需要等待3秒后才能操作
 * @author liangzhenjie
 * @date 2018/01/09
**/
    void launchAllMotors();
    /**
 * @brief 关闭所有设备
 * @author liangzhenjie
 * @date 2018/01/15
**/
    void closeAllMotors();
    /**
 * @brief 开启或关闭设备自动刷新功能（默认启动后开启此功能）
 * @author liangzhenjie
 * @date 2018/01/09
 * @param idArray 设备id数组
 * @param bOpen 是否开启
**/
/**
 * @brief 启动指定设备，若设备未启动，需先调用此函数，调用后需要等待3秒后才能操作
 * @author liangzhenjie
 * @date 2018/01/09
 * @param id 设备id
**/
    void launchMotor(uint8_t id);
    /**
 * @brief 关闭指定设备
 * @author liangzhenjie
 * @date 2018/01/15
 * @param id 设备id
**/
    void closeMotor(uint8_t id);
    /**
 * @brief 开启或关闭设备自动刷新功能（默认启动后开启此功能）
 * @author liangzhenjie
 * @date 2018/01/09
 * @param idArray 设备id数组
 * @param bOpen 是否开启
**/
    void switchAutoRefresh(vector<uint8_t> idArray,bool bOpen);
    /**
 * @brief 设置自动刷新时间间隔
 * @author liangzhenjie
 * @date 2018/01/09
 * @param idArray 设备id数组
 * @param mSec 毫秒数
**/
    void setAutoRefreshInterval(vector<uint8_t> idArray,uint32_t mSec);
    /**
 * @brief 设置位置
 * @author liangzhenjie
 * @date 2018/01/09
 * @param id 设备Id
 * @param pos 目标位置，单位是转数
**/
    void setPosition(uint8_t id,double pos);
    /**
 * @brief 设置速度
 * @author liangzhenjie
 * @date 2018/01/09
 * @param id 设备Id
 * @param vel 目标速度，单位是转/每分钟
**/
    void setVelocity(uint8_t id,double vel);
    /**
 * @brief 设置电流
 * @author liangzhenjie
 * @date 2018/01/09
 * @param id 设备Id
 * @param current 目标电流，单位是A
**/
    void setCurrent(uint8_t id,double current);
    /**
 * @brief 获取当前位置
 * @author liangzhenjie
 * @date 2018/01/09
 * @param id 设备id
 * @param bRefresh是否需要刷新，如果为真，会自动请求一次位置读取
 * @return 当前位置，单位是转数
**/
    double getPosition(uint8_t id,bool bRefresh=false)const;
    /**
 * @brief 获取当前速度
 * @author liangzhenjie
 * @date 2018/01/09
 * @param id 设备id
 * @param bRefresh是否需要刷新，如果为真，会自动请求一次速度读取
 * @return 当前速度，单位是转/每分钟
**/
    double getVelocity(uint8_t id,bool bRefresh=false)const;
    /**
 * @brief 获取当前电流
 * @author liangzhenjie
 * @date 2018/01/09
 * @param id 设备id
 * @param bRefresh是否需要刷新，如果为真，会自动请求一次电流读取
 * @return 当前电流，单位是A
**/
    double getCurrent(uint8_t id,bool bRefresh=false)const;
    /**
 * @brief 设置电机属性
 * @author liangzhenjie
 * @date 2018/01/09
 * @param id 设备id
 * @param attrId 属性Id
 * @param value 属性设置值(目前都为double，后期会改进)
**/
    void setMotorAttribute(uint8_t id,MotorData::Motor_Data_Id attrId,double value);
    /**
 * @brief 获取电机属性值
 * @author liangzhenjie
 * @date 2018/01/09
 * @param id 设备id
 * @param attrId 属性Id
 * @return 返回属性值
**/
    double getMotorAttribute(uint8_t id,MotorData::Motor_Data_Id attrId)const;
    /**
 * @brief 设备保存当前所有参数
 * @author liangzhenjie
 * @date 2018/01/09
 * @param id 设备id
**/
    void saveAllParams(uint8_t id);
    /**
 * @brief 清除homing信息，包括左右极限和0位
 * @author liangzhenjie
 * @date 2018/01/09
 * @param id 设备id
**/
    void clearHomingInfo(uint8_t id);
    /**
 * @brief 设置homing操作模式
 * @author liangzhenjie
 * @date 2018/01/09
 * @param id 设备id
 * @param nMode 操作模式（UserDefine::Homing_Auto,UserDefine::Homing_Manual）
**/
    void setHomingOperationMode(uint8_t id,uint8_t nMode);
    /**
 * @brief 开启图表指定通道
 * @author liangzhenjie
 * @date 2018/01/09
 * @param id 设备id
 * @param nChannelId 通道id（UserDefine::channel_1到UserDefine::channel_4）
**/
    void openChartChannel(uint8_t id,uint8_t  nChannelId);
    /**
 * @brief 关闭图表指定通道
 * @author liangzhenjie
 * @date 2018/01/09
 * @param id 设备id
 * @param nChannelId 通道id（UserDefine::channel_1到UserDefine::channel_4）
**/
    void closeChartChannel(uint8_t id, uint8_t nChannelId);
    /**
 * @brief 开启或关闭图表所有通道
 * @author liangzhenjie
 * @date 2018/01/09
 * @param id 设备id
 * @param bOn 是否开启
**/
    void switchChartAllChannel(uint8_t id,bool bOn);
    /**
 * @brief 重新获取属性,将请求刷新属性
 * @author liangzhenjie
 * @date 2018/01/09
 * @param id 设备id
 * @param attrId 设备属性Id
**/
    void regainAttrbute(uint8_t id,uint8_t attrId);
    /**
 * @brief 获取设备错误历史记录
 * @author liangzhenjie
 * @date 2018/01/30
 * @param id 设备id
**/
    vector<uint16_t> getErrorHistory(uint8_t id);
    /**
 * @brief 设备掉线重连
 * @author liangzhenjie
 * @date 2018/01/31
 * @param id 设备id
**/
    void reconnect(uint8_t id);
    /**
 * @brief 设备错误清除
 * @author liangzhenjie
 * @date 2018/01/09
 * @param id 设备id
**/
    void clearError(uint8_t id);
private:
    void finishRecognizeCallback();
    void onRequestCallback(uint8_t nDeviceId, uint8_t nProxyId,double value);
    void errorOccur(uint8_t nDeviceId,uint16_t errorId, std::string errorStr);
    void motorAttrChanged(uint8_t nDeviceId,uint8_t attrId,double value);
    void startNewChart();
    void chartValueChange(uint8_t channelId,double value);
    vector<uint8_t> convertQListToVector(const QList<quint8> &qList)const;
    QList<quint8> convertVectorToQList(const vector<uint8_t> &cVector)const;
private:
    int m_nLaunchMotorsCnt;
public:
    //signals
    /**
     * @brief m_sOperationFinished 操作完成信号 <uint8_t 设备id,uint8_t 操作类型，类型枚举OperationFlags>
     */
    CSignal<uint8_t,uint8_t> m_sOperationFinished;
    /**
     * @brief m_sRequestBack 请求返回信号 <uint8_t 设备id ,uint8_t 请求协议id,double 返回值>
     */
    CSignal<uint8_t,uint8_t,double> m_sRequestBack;
    /**
     * @brief m_sError 错误信号 <uint8_t 设备id,uint16_t 错误id,std::string 错误信息>
     */
    CSignal<uint8_t,uint16_t,std::string> m_sError;
    /**
     * @brief m_sMotorAttrChanged 电机数据变化，自动刷新或者主动刷新返回以后，会触发该信号
     * <uint8_t 设备id,uint8_t 变化属性id,double 属性值>
     */
    CSignal<uint8_t,uint8_t,double> m_sMotorAttrChanged;
    /**
     * @brief m_sNewChartStart 图显信号
     */
    CSignal<> m_sNewChartStart;
    /**
     * @brief m_sChartValueChange 图显数据
     */
    CSignal<uint8_t,double> m_sChartValueChange;
private:
    vector<int> m_lConnectionIds;
    static QCoreApplication * m_pQtCore;
    static MotorsController * m_pInstance;
};

#endif // MOTORSCONTROLLER_H
