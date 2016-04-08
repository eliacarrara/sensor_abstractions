#ifndef MAGNETOMETER_H
#define MAGNETOMETER_H

#include <sensor.h>

namespace Sensor{

struct RawMagnetometerData{
        int nP_Data;
        int nR_Data;
        int nY_Data;
};

typedef float Gauss;

class Magnetometer : public SensorBase<Gauss, RawMagnetometerData> {
    public:
        virtual bool ReadSensorDataOnce(RawMagnetometerData & OutData)=0;
        virtual Gauss ConvertToSIUnit(RawMagnetometerData Data)=0;
};

}
#endif
