#ifndef THERMOMETER_H
#define THERMOMETER_H

#include <sensor.h>

namespace Sensor{

struct RawThermometerData{
    int nTemp;
};
typedef float Celcius;

class Thermometer : public SensorBase<Celcius, RawThermometerData> {
public:
    virtual bool ReadSensorDataOnce(RawThermometerData & OutData)=0;
    virtual Celcius ConvertToSIUnit(RawThermometerData Data)=0;
};

}

#endif
