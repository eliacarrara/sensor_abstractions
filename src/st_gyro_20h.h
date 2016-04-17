#ifndef ST_L3GD20H
#define ST_L3GD20H

#include <gyroscope.h>
#include <thermometer.h>
#include <icdevice.h>

#define MAG_NR_OF_REGISTERS 29
#define MAG_SPI_DEV_FILE "/dev/spidev1.0"
#define MAG_SPI_SPEED 1000000

#define MAG_SPI_READ_FLAG 0x80
#define MAG_SPI_WRITE_FLAG 0x00
#define MAG_SPI_ACCES_INC_FLAG 0x40

namespace Sensor{

class StGyro_20h : public Sensor::Thermometer, public Sensor::Gyroscope, public Device::ICDevice {
    public:
        StGyro_20h();
        ~StGyro_20h();

        bool ReadSensorDataOnce(Sensor::RawGyroscopeData & OutData);
        DegreePerSecond ConvertToSIUnit(Sensor::RawGyroscopeData Data);

        bool ReadSensorDataOnce(Sensor::RawThermometerData & OutData);
        Celcius ConvertToSIUnit(Sensor::RawThermometerData Data);
    private:
        Device::RegPtr  m_RegWhoAmiI, m_RegCtrl1, m_RegCtrl2, m_RegCtrl3, m_RegCtrl4, m_RegCtrl5, m_RegRef,
                        m_RegOutT, m_RegStatus, m_RegOutX_L, m_RegOutX_H, m_RegOutY_L, m_RegOutY_H, m_RegOutZ_L,
                        m_RegOutZ_H, m_RegFifoCtrl, m_RegFifoSrc, m_RegIntCfg, m_RegIntSrc, m_RegIntThsX_H,
                        m_RegIntThsX_L, m_RegIntThsY_H, m_RegIntThsY_L, m_RegIntThsZ_H, m_RegIntThsZ_L,
                        m_RegIntDur, m_RegLowOdr;

        Device::eReturnCode MultiRead(Device::RegPtr psReg, unsigned int BytesToRead, char * pcRxData);
        Device::eReturnCode MultiWrite(Device::RegPtr psReg, unsigned int BytesToRead, char * pcTxData);
        Device::eReturnCode Read(Device::RegPtr psReg, char &cValue);
        Device::eReturnCode Write(Device::RegPtr psReg, char &cValue);
};

}

#endif
