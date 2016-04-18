#ifndef ST_L3GD20H
#define ST_L3GD20H

#include <st_sensor.h>
#include <gyroscope.h>
#include <thermometer.h>

#define MAG_NR_OF_REGISTERS 29
#define MAG_SPI_DEV_FILE "/dev/spidev1.0"
#define MAG_SPI_SPEED 1000000

#define MAG_SPI_READ_FLAG 0x80
#define MAG_SPI_WRITE_FLAG 0x00
#define MAG_SPI_ACCES_INC_FLAG 0x40



namespace Sensor{

class StGyro_20h : public Thermometer, public Gyroscope, public StSensorConfig {
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

        Device::eReturnCode MultiRead(Device::RegPtr psReg, Size BytesToRead, Word * pcRxData);
        Device::eReturnCode MultiWrite(Device::RegPtr psReg, Size BytesToRead, Word * pcTxData);
        Device::eReturnCode Read(Device::RegPtr psReg, Word &cValue);
        Device::eReturnCode Write(Device::RegPtr psReg, Word &cValue);
};

}

#endif
