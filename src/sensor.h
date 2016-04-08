#ifndef SENSOR_H
#define SENSOR_H


/// This namespace contains all sensor related items.
namespace Sensor{

///
/// Base class for all sensors. This class represents the base class for all sensors.
///
template <class S, class T>
class SensorBase {
    public:
        ///
        /// \brief ReadSensorDataOnce
        /// \param OutData
        /// \return
        ///
        virtual bool ReadSensorDataOnce(T & OutData)=0;

        virtual S ConvertToSIUnit(T Data)=0;
};

}

#endif
