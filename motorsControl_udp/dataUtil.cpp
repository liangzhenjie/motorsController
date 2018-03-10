﻿#include "dataUtil.h"
DataUtil * DataUtil::m_pInstance = nullptr;

Directives DataUtil::convertToReadProxyId(MotorData::Motor_Data_Id id)
{
    if(!m_pInstance)
        m_pInstance = new DataUtil();
    QList<Directives> keys = m_pInstance->readDataMap.keys(id);

    if(keys.size() > 0)
        return keys.at(0);

    return DIRECTIVES_INVALID;
}

Directives DataUtil::convertToSetProxyId(MotorData::Motor_Data_Id id)
{
    if(!m_pInstance)
        m_pInstance = new DataUtil();
    QList<Directives> keys = m_pInstance->setDataMap.keys(id);

    if(keys.size() > 0)
        return keys.at(0);

    return DIRECTIVES_INVALID;
}


MotorData::Motor_Data_Id DataUtil::convertToMotorDataId(Directives id)
{
    if(!m_pInstance)
        m_pInstance = new DataUtil();

    if(m_pInstance->readDataMap.contains(id))
        return m_pInstance->readDataMap.value(id);

    if(m_pInstance->setDataMap.contains(id))
        return m_pInstance->setDataMap.value(id);

    return MotorData::DATA_INVALID;
}

DataUtil::DataUtil()
{
    readDataMap.insert(D_READ_CUR_CURRENT,MotorData::CUR_ACTURAL);
    readDataMap.insert(D_READ_CUR_VELOCITY,MotorData::VEL_ACTURAL);
    readDataMap.insert(D_READ_CUR_POSITION,MotorData::POS_ACTURAL);

    readDataMap.insert(D_READ_CUR_P,MotorData::CUR_PROPORTIONAL);
    readDataMap.insert(D_READ_CUR_I,MotorData::CUR_INTEGRAL);
    readDataMap.insert(D_READ_VEL_P,MotorData::VEL_PROPORTIONAL);
    readDataMap.insert(D_READ_VEL_I,MotorData::VEL_INTEGRAL);
    readDataMap.insert(D_READ_POS_P,MotorData::POS_PROPORTIONAL);
    readDataMap.insert(D_READ_POS_I,MotorData::POS_INTEGRAL);
    readDataMap.insert(D_READ_POS_D,MotorData::POS_DIFFERENTIAL);

    readDataMap.insert(D_READ_PROFILE_POS_MAX_SPEED,MotorData::PROFILE_POS_MAX_SPEED);
    readDataMap.insert(D_READ_PROFILE_POS_ACC,MotorData::PROFILE_POS_ACC);
    readDataMap.insert(D_READ_PROFILE_POS_DEC,MotorData::PROFILE_POS_DEC);

    readDataMap.insert(D_READ_PROFILE_VEL_MAX_SPEED,MotorData::PROFILE_VEL_MAX_SPEED);
    readDataMap.insert(D_READ_PROFILE_VEL_ACC,MotorData::PROFILE_VEL_ACC);
    readDataMap.insert(D_READ_PROFILE_VEL_DEC,MotorData::PROFILE_VEL_DEC);

    readDataMap.insert(D_READ_CURRENT_PID_MIN,MotorData::CUR_MINIMAL);
    readDataMap.insert(D_READ_CURRENT_PID_MAX,MotorData::CUR_MAXIMUM);
    readDataMap.insert(D_READ_VELOCITY_PID_MIN,MotorData::VEL_MINIMAL);
    readDataMap.insert(D_READ_VELOCITY_PID_MAX,MotorData::VEL_MAXIMUM);
    readDataMap.insert(D_READ_POSITION_PID_MIN,MotorData::POS_MINIMAL);
    readDataMap.insert(D_READ_POSITION_PID_MAX,MotorData::POS_MAXIMUM);

    setDataMap.insert(D_SET_CURRENT,MotorData::CUR_IQ_SET);
    setDataMap.insert(D_SET_VELOCITY,MotorData::VEL_SET);
    setDataMap.insert(D_SET_POSITION,MotorData::POS_SET);
    setDataMap.insert(D_SET_CURRENT_ID,MotorData::CUR_ID_SET);
    setDataMap.insert(D_SET_CURRENT_P,MotorData::CUR_PROPORTIONAL);
    setDataMap.insert(D_SET_CURRENT_I,MotorData::CUR_INTEGRAL);
    setDataMap.insert(D_SET_VELOCITY_P,MotorData::VEL_PROPORTIONAL);
    setDataMap.insert(D_SET_VELOCITY_I,MotorData::VEL_INTEGRAL);
    setDataMap.insert(D_SET_POSITION_P,MotorData::POS_PROPORTIONAL);
    setDataMap.insert(D_SET_POSITION_I,MotorData::POS_INTEGRAL);
    setDataMap.insert(D_SET_POSITION_D,MotorData::POS_DIFFERENTIAL);

    setDataMap.insert(D_SET_PROFILE_POS_MAX_SPEED,MotorData::PROFILE_POS_MAX_SPEED);
    setDataMap.insert(D_SET_PROFILE_POS_ACC,MotorData::PROFILE_POS_ACC);
    setDataMap.insert(D_SET_PROFILE_POS_DEC,MotorData::PROFILE_POS_DEC);

    setDataMap.insert(D_SET_PROFILE_VEL_MAX_SPEED,MotorData::PROFILE_VEL_MAX_SPEED);
    setDataMap.insert(D_SET_PROFILE_VEL_ACC,MotorData::PROFILE_VEL_ACC);
    setDataMap.insert(D_SET_PROFILE_VEL_DEC,MotorData::PROFILE_VEL_DEC);

    setDataMap.insert(D_SET_CURRENT_PID_MIN,MotorData::CUR_MINIMAL);
    setDataMap.insert(D_SET_CURRENT_PID_MAX,MotorData::CUR_MAXIMUM);
    setDataMap.insert(D_SET_VELOCITY_PID_MIN,MotorData::VEL_MINIMAL);
    setDataMap.insert(D_SET_VELOCITY_PID_MAX,MotorData::VEL_MAXIMUM);
    setDataMap.insert(D_SET_POSITION_PID_MIN,MotorData::POS_MINIMAL);
    setDataMap.insert(D_SET_POSITION_PID_MAX,MotorData::POS_MAXIMUM);

    setDataMap.insert(D_SET_CHART_FREQUENCY,MotorData::CHART_FREQUENCY);
    setDataMap.insert(D_SET_CHART_THRESHOLD,MotorData::CHART_THRESHOLD);

    readDataMap.insert(D_READ_CHART_FREQUENCY,MotorData::CHART_FREQUENCY);
    readDataMap.insert(D_READ_CHART_THRESHOLD,MotorData::CHART_THRESHOLD);

    readDataMap.insert(D_READ_CHANNEL_2,MotorData::DATA_CHART);
    readDataMap.insert(D_READ_CHANNEL_3,MotorData::DATA_CHART);
    readDataMap.insert(D_READ_CHANNEL_4,MotorData::DATA_CHART);

    readDataMap.insert(D_READ_MIN_POS,MotorData::POS_MIN_POS);
    readDataMap.insert(D_READ_MAX_POS,MotorData::POS_MAX_POS);

    setDataMap.insert(D_SET_MIN_POS,MotorData::POS_MIN_POS);
    setDataMap.insert(D_SET_MAX_POS,MotorData::POS_MAX_POS);

    setDataMap.insert(D_SET_HOMING_POS,MotorData::POS_HOMING);

    setDataMap.insert(D_SET_POS_OFFSET,MotorData::POS_OFFSET);
    readDataMap.insert(D_READ_POS_OFFSET,MotorData::POS_OFFSET);

    readDataMap.insert(D_READ_VOLTAGE,MotorData::VOLTAGE);

    readDataMap.insert(D_READ_HOMING_LIMIT,MotorData::HOMING_LIMIT);
    setDataMap.insert(D_SET_HOMING_LIMIT,MotorData::HOMING_LIMIT);

    setDataMap.insert(D_SET_HOMING_CUR_MIN,MotorData::HOMING_CUR_MIN);
    readDataMap.insert(D_READ_HOMING_CUR_MIN,MotorData::HOMING_CUR_MIN);
    setDataMap.insert(D_SET_HOMING_CUR_MAX,MotorData::HOMING_CUR_MAX);
    readDataMap.insert(D_READ_HOMING_CUR_MAX,MotorData::HOMING_CUR_MAX);

    readDataMap.insert(D_READ_CURRENT_SCALE,MotorData::CURRENT_SCALE);

    setDataMap.insert(D_SET_FILTER_C_STATUS,MotorData::FILTER_C_STATUS);
    readDataMap.insert(D_READ_FILTER_C_STATUS,MotorData::FILTER_C_STATUS);
    setDataMap.insert(D_SET_FILTER_C_VALUE,MotorData::FILTER_C_VALUE);
    readDataMap.insert(D_READ_FILTER_C_VALUE,MotorData::FILTER_C_VALUE);

    setDataMap.insert(D_SET_FILTER_V_STATUS,MotorData::FILTER_V_STATUS);
    readDataMap.insert(D_READ_FILTER_V_STATUS,MotorData::FILTER_V_STATUS);
    setDataMap.insert(D_SET_FILTER_V_VALUE,MotorData::FILTER_V_VALUE);
    readDataMap.insert(D_READ_FILTER_V_VALUE,MotorData::FILTER_V_VALUE);

    setDataMap.insert(D_SET_FILTER_P_STATUS,MotorData::FILTER_P_STATUS);
    readDataMap.insert(D_READ_FILTER_P_STATUS,MotorData::FILTER_P_STATUS);
    setDataMap.insert(D_SET_FILTER_P_VALUE,MotorData::FILTER_P_VALUE);
    readDataMap.insert(D_READ_FILTER_P_VALUE,MotorData::FILTER_P_VALUE);

    setDataMap.insert(D_SET_INERTIA,MotorData::INERTIA);
    readDataMap.insert(D_READ_INERTIA,MotorData::INERTIA);

    setDataMap.insert(D_SET_LOCK_ENERGY,MotorData::LOCK_ENERGY);
    readDataMap.insert(D_READ_LOCK_ENERGY,MotorData::LOCK_ENERGY);

    readDataMap.insert(D_READ_TEMP_MOTOR,MotorData::TEMP_MOTOR);
    readDataMap.insert(D_READ_TEMP_INVERTER,MotorData::TEMP_INVERTER);
    readDataMap.insert(D_READ_TEMP_PROTECT,MotorData::TEMP_PROTECT);
    readDataMap.insert(D_READ_TEMP_RECOVERY,MotorData::TEMP_RECOVERY);

    setDataMap.insert(D_SET_TEMP_PROTECT,MotorData::TEMP_PROTECT);
    setDataMap.insert(D_SET_TEMP_RECOVERY,MotorData::TEMP_RECOVERY);

    setDataMap.insert(D_SWITCH_CALIBRATION,MotorData::CALIBRATION_SWITCH);
    setDataMap.insert(D_SET_CALIBRATION_ANGLE,MotorData::CALIBRATION_ANGLE);
    readDataMap.insert(D_READ_CALIBRATION_SWITCH,MotorData::CALIBRATION_SWITCH);
    readDataMap.insert(D_READ_CALIBRATION_ANGLE,MotorData::CALIBRATION_ANGLE);

    setDataMap.insert(D_SET_SWITCH_MOTORS,MotorData::MOTOR_SWITCH);
    readDataMap.insert(D_READ_MOTORS_SWITCH,MotorData::MOTOR_SWITCH);

    readDataMap.insert(D_READ_VERSION,MotorData::MOTOR_VERSION);
	
	
    setDataMap.insert(D_SET_MODE,MotorData::MODE_ID);
    setDataMap.insert(D_SET_DEVICE_ID,MotorData::DEVICE_ID);
    readDataMap.insert(D_CHECK_ERROR,MotorData::ERROR_ID);
    setDataMap.insert(D_CHECK_ERROR,MotorData::ERROR_ID);

    readDataMap.insert(D_READ_CURRENT_MAXSPEED,MotorData::CUR_MAXSPEED);
    setDataMap.insert(D_SET_CURRENT_MAXSPEED,MotorData::CUR_MAXSPEED);

    readDataMap.insert(D_READ_MOTOR_MODE,MotorData::MODE_ID);
    //这个比较特殊 两条协议对应一个属性，处理时候要判断是开还是关
    setDataMap.insert(D_CHART_OPEN,MotorData::CHART_SWITCH);
    setDataMap.insert(D_CHART_CLOSE,MotorData::CHART_SWITCH);
}


