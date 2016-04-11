/// \file   sensor.h
/// \author Elia Carrara
/// \date   April, 2016
/// \brief  Contains the definition of the sensor base class.

#ifndef SENSOR_H
#define SENSOR_H

/// \brief This namespace contains all sensor related items.
namespace Sensor{

/// \brief This class represents the base class for sensors.
/// \details SensorBase represents the base of all sensor classes.
/// This class uses two template types (S,T). S should be used for
/// a type defined in types.h, T should be used with a raw data type
/// whiche are defined in classes inheriting from SensorBase.
template <class S, class T>
class SensorBase {
    public:
        /// \brief Reads sensor data.
        /// \param ReadSensorDataOnce should be implemented to read
        /// raw data from a sensor. The argument is passed by reference. This
        /// means it will store the gatherd data after it's successful completion
        /// \return Returns true when it succeeded.
        virtual bool ReadSensorDataOnce(T & OutData)=0;
        /// \brief Converts raw sensor data to a SI unit.
        /// \param ConvertToSIUnit should be implemented to convert raw data(T)
        /// provided by a sensor to an SI unit(S).
        /// \return Returns an SI unit.
        virtual S ConvertToSIUnit(T Data)=0;
};

}

#endif
