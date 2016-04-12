/// \file   accelerometer.h
/// \author Elia Carrara
/// \date   April, 2016
/// \brief  Contains the definition of the accelerometer-sensor base class.

#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include <sensor.h>
#include <types.h>

namespace Sensor{

struct RawAcceleromterData{
    int nX_Data;
    int nY_Data;
    int nZ_Data;
};

class Accelerometer : public SensorBase<GForce *, RawAcceleromterData> {
public:    
    virtual bool ReadSensorDataOnce(RawAcceleromterData & OutData)=0;
    virtual GForce * ConvertToSIUnit(RawAcceleromterData Data)=0;
};

}
#endif
