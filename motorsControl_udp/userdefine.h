#ifndef USERDEFINE_H
#define USERDEFINE_H

namespace UserDefine {
    enum{//connect status
        NO_CONNECT,
        CAN_CONNECTED=0x02,
        MOTOR_CONNECTED=0x04,
    };

    enum Channel_ID{
        channel_1=0,
        channel_2,
        channel_3,
        channel_4,
        channel_cnt
    };

    enum ErrorsDefine
    {
        ERR_NONE = 0,
        ERR_MOTOR_OVERVOLTAGE=0x01,
        ERR_MOTOR_UNDERVOLTAGE=0x02,
        ERR_MOTOR_LOCKED_ROTOR=0x04,
        ERR_MOTOR_OVERHEATING=0x08,
        ERR_MOTOR_READ_OR_WIRTE=0x10,
        ERR_MOTOR_MULTI_TURN=0x20,
        ERR_MOTOR_TEMPERATURE_SENSOR=0x40,
        ERR_CAN_COMMUNICATION=0x80,
        ERR_DRV_PROTECTION=0x400,
        ERR_ID_UNUNIQUE=0x800,
        ERR_MOTOR_DISCONNECTION=0x801,
        ERR_CAN_DISCONNECTION=0x802,
        ERR_UNKOWN=0xffff
    };

    enum Motor_status{
        Status_Online=0x00,
        Status_Offline=0x01,
    };

    enum{
        MOTOR_SWITCH_OFF=0,
        MOTOR_SWITCH_ON=1,
    };

    enum
    {
        CHART_SWITCH_OFF=0,
        CHART_SWITCH_ON=1,
    };
    enum HomingOperationMode{
        Homing_Auto=0,
        Homing_Manual,
    };
}

#endif // USERDEFINE_H
