#ifndef ST_LIS3MDL
#define ST_LIS3MDL

#include <magnetometer.h>
#include <thermometer.h>
#include <st_sensor.h>

#define MAG_NR_OF_REGISTERS 19
#define MAG_SPI_DEV_FILE "/dev/spidev1.0"
#define MAG_SPI_SPEED 1000000

#define MAG_SPI_READ_FLAG 0x80
#define MAG_SPI_WRITE_FLAG 0x00
#define MAG_SPI_ACCES_INC_FLAG 0x40

namespace Sensor{

class StMagno_mdl : public Sensor::Thermometer, public Sensor::Magnetometer, public StSensorConfig {
    public:
        StMagno_mdl();
        ~StMagno_mdl();

        enum PowerMode{
            PW_LOW_POWER                = 0x0,  /// Low-power mode
            PW_MEDIUM_PERFORMANCE       = 0x1,  /// Medium-performance mode
            PW_HIGH_PERFORMANCE         = 0x2,  /// High-performance mode
            PW_ULTRA_HIGH_PERFORMANCE   = 0x3,  /// Ultra-high-performance mode
        };
        enum OperatingMode{
            OP_CONTINUOUS               = 0x0,  /// Continuous-conversion mode
            OP_SINGLE_SHOT              = 0x1,  /// Single-conversion mode
            OP_POWER_DOWN               = 0x2,  /// Power-down mode
        };
        enum ODR{
            ODR_SPEED_0     = 0x0,  /// 0.625 Hz
            ODR_SPEED_1     = 0x1,  /// 1.25 Hz
            ODR_SPEED_2     = 0x2,  /// 2.5 Hz
            ODR_SPEED_3     = 0x3,  /// 5 Hz
            ODR_SPEED_4     = 0x4,  /// 10 Hz
            ODR_SPEED_5     = 0x5,  /// 20 Hz
            ODR_SPEED_6     = 0x6,  /// 40 Hz
            ODR_SPEED_7     = 0x7,  /// 80 Hz
            ODR_SPEED_8     = 0x8,  /// 155 Hz
            ODR_SPEED_9     = 0x9,  /// 300 Hz
            ODR_SPEED_A     = 0xA,  /// 560 Hz
            ODR_SPEED_B     = 0xB  /// 1000 Hz
        };
        enum MeasureRange{
            RANGE_0 = 0x0,  /// ± 4 Gauss
            RANGE_1 = 0x1,  /// ± 8 Gauss
            RANGE_2 = 0x2,  /// ± 12 Gauss
            RANGE_3 = 0x3  /// ± 16 Gauss
        };


        bool Reboot();
        bool SoftReset();

        // IC Identification
        bool WhoAmI(Word & Value);

        // Operation Configutation
        bool IsEnabledTemp(bool & Value);
        bool UseTemp(bool Value);

        bool GetPowerModeXY(PowerMode & Value);
        bool SetPowerModeXY(PowerMode Value);

        bool GetPowerModeZ(PowerMode & Value);
        bool SetPowerModeZ(PowerMode Value);

        bool GetOperatingMode(OperatingMode & Value);
        bool SetOperatingMode(OperatingMode Value);

        bool GetODR(ODR & Value);
        bool SetODR(ODR Value);

        bool GetRange(ODR & Value);
        bool SetRange(ODR Value);

        bool IsEnabledSelfTest(bool & Value);
        bool UseSelfTest(bool Value);

        bool IsModeLowPower(bool & Value);
        bool UseModeLowPower(bool Value);

        bool IsFastReading(bool & Value);
        bool UseFastReading(bool Value);

        bool IsBDU(bool & Value);
        bool UseBDU(bool Value);

        bool IsSPI3WireMode(bool & Value);
        bool UseSPI3WireMode(bool Value);

        bool GetEndianess(Endianess & Value);
        bool SetEndianess(Endianess Value);


        // Status
        bool DataOverrunXYZ(bool & Value);
        bool DataOverrunX(bool & Value);
        bool DataOverrunY(bool & Value);
        bool DataOverrunZ(bool & Value);

        bool DataAvailableXYZ(bool & Value);
        bool DataAvailableX(bool & Value);
        bool DataAvailableY(bool & Value);
        bool DataAvailableZ(bool & Value);


        // Interrupt stuff
        bool IsInt(bool & Value);
        bool UseInt(bool Value);

        bool IsIntX(bool & Value);
        bool UseIntX(bool Value);

        bool IsIntY(bool & Value);
        bool UseIntY(bool Value);

        bool IsIntZ(bool & Value);
        bool UseIntZ(bool Value);

        bool GetIntPolarity(LogicState & Value);
        bool SetIntPolarity(LogicState Value);

        bool GetIntType(InterruptSignalType & Value);
        bool SetIntType(InterruptSignalType Value);

        bool IsIntThsPosX(bool & Value);
        bool IsIntThsNegX(bool & Value);

        bool IsIntThsPosY(bool & Value);
        bool IsIntThsNegY(bool & Value);

        bool IsIntThsPosZ(bool & Value);
        bool IsIntThsNegXZ(bool & Value);

        bool IsIntMeasureOverflow(bool & Value);
        bool IsIntSig(bool & Value);

        bool ReadSensorDataOnce(Sensor::RawMagnetometerData &OutData);
        Gauss ConvertToSIUnit(Sensor::RawMagnetometerData Data);

        bool ReadSensorDataOnce(Sensor::RawThermometerData &OutData);
        Celcius ConvertToSIUnit(Sensor::RawThermometerData Data);
    private:
        Device::RegPtr  m_RegWhoAmI, m_RegCtrlReg1, m_RegCtrlReg2, m_RegCtrlReg3, m_RegCtrlReg4,
                        m_RegCtrlReg5, m_RegCtrlReg6, m_RegStatus, m_RegOutX_L, m_RegOutX_H,
                        m_RegOutY_L, m_RegOutY_H, m_RegOutZ_L, m_RegOutZ_H, m_RegOutT_L, m_RegOutT_H,
                        m_RegIntCfg, m_RegIntSrc, m_RegIntThs_L, m_RegIntThs_H;

        Device::eReturnCode MultiRead(Device::RegPtr psReg, Size BytesToRead, Word * pcRxData);
        Device::eReturnCode MultiWrite(Device::RegPtr psReg, Size BytesToRead, Word * pcTxData);
        Device::eReturnCode Read(Device::RegPtr psReg, Word & cValue);
        Device::eReturnCode Write(Device::RegPtr psReg, Word & cValue);
};

}

#endif
