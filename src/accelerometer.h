#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include <sensor.h>
#include <si_types.h>

namespace Sensor{

struct RawAcceleromterData{
    int nX_Data;
    int nY_Data;
    int nZ_Data;
};

class Accelerometer : public SensorBase<GForge, RawAcceleromterData> {
public:
    virtual bool ReadSensorDataOnce(RawAcceleromterData & OutData)=0;
    virtual GForge ConvertToSIUnit(RawAcceleromterData Data)=0;
};

}
#endif
