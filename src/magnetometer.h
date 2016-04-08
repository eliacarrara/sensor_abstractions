/// \file   magnetometer.h
/// \author Elia Carrara
/// \date   April, 2016
/// \brief  Contains the definition of the magnetometer-sensor base class.

#ifndef MAGNETOMETER_H
#define MAGNETOMETER_H

#include <sensor.h>
#include <si_types.h>

namespace Sensor{

struct RawMagnetometerData{
        int nP_Data;
        int nR_Data;
        int nY_Data;
};

class Magnetometer : public SensorBase<Gauss, RawMagnetometerData> {
    public:
        virtual bool ReadSensorDataOnce(RawMagnetometerData & OutData)=0;
        virtual Gauss ConvertToSIUnit(RawMagnetometerData Data)=0;
};

}
#endif
