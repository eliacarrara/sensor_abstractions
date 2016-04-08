#ifndef GYROSCOPE_H
#define GYROSCOPE_H

#include <sensor.h>

namespace Sensor{

struct RawGyroscopeData{
    int nP_Data;
    int nR_Data;
    int nY_Data;
};

typedef float DPS;

class Gyroscope : public SensorBase<DPS, RawGyroscopeData> {
public:
    virtual bool ReadSensorDataOnce(RawGyroscopeData & OutData)=0;
    virtual DPS ConvertToSIUnit(RawGyroscopeData Data)=0;
};

}
#endif
