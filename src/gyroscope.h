#ifndef GYROSCOPE_H
#define GYROSCOPE_H

#include <sensor.h>
#include <si_types.h>

namespace Sensor{

struct RawGyroscopeData{
    int nP_Data;
    int nR_Data;
    int nY_Data;
};

class Gyroscope : public SensorBase<DPS, RawGyroscopeData> {
public:
    virtual bool ReadSensorDataOnce(RawGyroscopeData & OutData)=0;
    virtual DPS ConvertToSIUnit(RawGyroscopeData Data)=0;
};

}
#endif
