#ifndef ST_LIS3MDL
#define ST_LIS3MDL

#include <magnetometer.h>
#include <thermometer.h>
#include <icdevice.h>

#define MAG_NR_OF_REGISTERS 19
#define MAG_SPI_DEV_FILE "/dev/spidev1.0"
#define MAG_SPI_SPEED 1000000


class StMagno_mdl : public Sensor::Thermometer, public Sensor::Magnetometer, public Device::ICDevice {
    public:
        StMagno_mdl();
        ~StMagno_mdl();

        bool ReadSensorDataOnce(Sensor::RawMagnetometerData &OutData);
        Gauss ConvertToSIUnit(Sensor::RawMagnetometerData Data);

        bool ReadSensorDataOnce(Sensor::RawThermometerData &OutData);
        Celcius ConvertToSIUnit(Sensor::RawThermometerData Data);
    private:
        Device::RegPtr  m_RegWhoAmI, m_RegCtrlReg1, m_RegCtrlReg2, m_RegCtrlReg3, m_RegCtrlReg4,
                        m_RegCtrlReg5, m_RegCtrlReg6, m_RegStatus, m_RegOutX_L, m_RegOutX_H,
                        m_RegOutY_L, m_RegOutY_H, m_RegOutZ_L, m_RegOutZ_H, m_RegOutT_L, m_RegOutT_H,
                        m_RegIntCfg, m_RegIntSrc, m_RegIntThs_L, m_RegIntThs_H;

        Device::eReturnCode MultiRead(Device::RegPtr psReg, unsigned int BytesToRead, char * pcRxData);
        Device::eReturnCode MultiWrite(Device::RegPtr psReg, unsigned int BytesToRead, char * pcTxData);
        Device::eReturnCode Read(Device::RegPtr psReg, char &cValue);
        Device::eReturnCode Write(Device::RegPtr psReg, char &cValue);
};

#endif
