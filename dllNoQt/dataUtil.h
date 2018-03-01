#ifndef DATAUTIL_H
#define DATAUTIL_H
#include <QMap>
#include "innfosproxy.h"
#include "motordata.h"


class DataUtil
{
public:
    static Directives convertToReadProxyId(MotorData::Motor_Data_Id id);
    static Directives convertToSetProxyId(MotorData::Motor_Data_Id id);
    static MotorData::Motor_Data_Id convertToMotorDataId(Directives id);
    static void autoDestroy();
private:
    DataUtil();
    QMap<Directives,MotorData::Motor_Data_Id> readDataMap;
    QMap<Directives,MotorData::Motor_Data_Id> setDataMap;
    static DataUtil * m_pInstance;
};
#endif // DATAUTIL_H
