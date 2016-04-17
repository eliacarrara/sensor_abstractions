#ifndef ST_LIS3DSH_A_H
#define ST_LIS3DSH_A_H

#include <accelerometer.h>
#include <thermometer.h>
#include <icdevice.h>

#define MAG_NR_OF_REGISTERS 31
#define ACC_SPI_DEV_FILE "/dev/spidev1.0"
#define ACC_SPI_SPEED 1000000

#define ACC_SPI_READ_FLAG 0x80
#define ACC_SPI_WRITE_FLAG 0x00
#define ACC_SPI_MSG(REG_ID, FLAG) ((REG_ID & 0x7F) | FLAG)
#define ACC_SPI_DEFAULT_BUFF_LEN 2

namespace Sensor{

class StAccel_dsh : public Sensor::Thermometer, public Sensor::Accelerometer, public Device::ICDevice {
    public:
        StAccel_dsh();
        ~StAccel_dsh();

        enum ODR{
            ODR_OFF         = 0x0,  /// PowerDown
            ODR_SPEED_0     = 0x1,  /// 3.125 Hz
            ODR_SPEED_1     = 0x2,  /// 6.25 Hz
            ODR_SPEED_2     = 0x3,  /// 12.5 Hz
            ODR_SPEED_3     = 0x4,  /// 25 Hz
            ODR_SPEED_4     = 0x5,  /// 50 Hz
            ODR_SPEED_5     = 0x6,  /// 100 Hz
            ODR_SPEED_6     = 0x7,  /// 400 Hz
            ODR_SPEED_7     = 0x8,  /// 800 Hz
            ODR_SPEED_8     = 0x9   /// 1600 Hz
        };
        enum MeasureRange{
            RANGE_0 = 0x0,  /// ± 2 g
            RANGE_1 = 0x1,  /// ± 4 g
            RANGE_2 = 0x2,  /// ± 6 g
            RANGE_3 = 0x3,  /// ± 8 g
            RANGE_4 = 0x4   /// ± 16 g
        };
        enum AntiAliasingBandwidth{
            BW_0 = 0x0, /// 800Hz
            BW_1 = 0x1, /// 200Hz
            BW_2 = 0x2, /// 400Hz
            BW_3 = 0x3  /// 50Hz
        };
        enum FifoMode{
            MODE_BYPASS             = 0x0,  /// Bypass mode. FIFO turned off.
            MODE_STOP_WHEN_FULL     = 0x1,  /// FIFO mode. Stops collecting data when FIFO is full.
            MODE_STREAM             = 0x2,  /// Stream mode. If the FIFO is full, the new sample overwrites the older one (circular buffer).
            MODE_STREAM_THEN_FIFO   = 0x3,  /// Stream mode until trigger is de-asserted, then FIFO mode.
            MODE_BYPASS_THEN_STREAM = 0x4,  /// Bypass mode until trigger is de-asserted, then Stream mode.
            MODE_BYPASS_THEN_FIFO   = 0x7   /// Bypass mode until trigger is de-asserted, then FIFO mode.
        };
        enum SelfTestMode{
            TEST_OFF        = 0x0, /// Normal Mode
            TEST_POSITIVE   = 0x1, /// Positive sign Mode
            TEST_NEGATIVE   = 0x2  /// Negative sign Mode
        };

        bool SoftReset();
        bool Reboot();

        // IC Identification
        bool ReadInfomation(unsigned short & unInfo);
        bool WhoAmI(char & WhoAmI);

        // Operation Configuration
        bool GetODR(ODR & Value);
        bool SetODR(ODR Value);

        bool GetRange(MeasureRange & Value);
        bool SetRange(MeasureRange Value);

        bool IsBDU(bool & Value);
        bool UseBDU(bool Value);

        bool IsReadInc(bool & Value);
        bool UseReadInc(bool Value);

        bool IsXAxisEnabled(bool & Value);
        bool IsYAxisEnabled(bool & Value);
        bool IsZAxisEnabled(bool & Value);

        bool UseAxisX(bool Value);
        bool UseAxisY(bool Value);
        bool UseAxisZ(bool Value);

        bool GetOffset(Sensor::RawAcceleromterData & Value);
        bool SetOffset(Sensor::RawAcceleromterData Value);

        bool GetSelfTestMode(SelfTestMode & Value);
        bool SetSelfTestMode(SelfTestMode Value);

        bool GetAntiAliasFilterBandwidth(AntiAliasingBandwidth & Value);
        bool SetAntiAliasFilterBandwidth(AntiAliasingBandwidth Value);

        bool IsSPI3WireMode(bool & Value);
        bool UseSPI3WireMode(bool Value);

        // Status
        bool DataOverrunXYZ(bool & Value);
        bool DataOverrunX(bool & Value);
        bool DataOverrunY(bool & Value);
        bool DataOverrunZ(bool & Value);

        bool DataAvailableXYZ(bool & Value);
        bool DataAvailableX(bool & Value);
        bool DataAvailableY(bool & Value);
        bool DataAvailableZ(bool & Value);


        // Fifo Stuff
        bool IsFifoEnabled(bool & Value);
        bool UseFifo(bool Value);

        bool IsFifoEmpty(bool & Value);
        bool IsFifoOverrun(bool & Value);

        bool GetFifoMode(FifoMode & Value);
        bool SetFifoMode(FifoMode Value);

        bool IsFifoWatermark(bool & Value);
        bool UseFifoWatermark(bool Value);

        bool GetFifoWatermarkStatus(bool & Value);
        bool GetFifoFilledLength(char & Value);

        bool GetFifoWatermarkPointer(char & Value);
        bool SetFifoWatermarkPointer(char Value);

        //Interrupt stuff
        bool IsIntDrdy(bool & Value);
        bool UseIntDrdy(bool Value);

        bool GetIntPolarity(LogicState & Value);
        bool SetIntPolarity(LogicState Value);

        bool GetIntType(InterruptSignalType & Value);
        bool SetIntType(InterruptSignalType Value);

        bool IsInterrupt1(bool & Value);
        bool UseInterrupt1(bool Value);
        bool IsInterrupt2(bool & Value);
        bool UseInterrupt2(bool Value);

        bool IsIntBoot(bool & Value);
        bool UseIntBoot(bool Value);
        bool IsIntFifoEmpty(bool & Value);
        bool UseIntFifoEmpty(bool Value);
        bool IsIntFifoWtmrk(bool & Value);
        bool UseIntFifoWtmrk(bool Value);
        bool IsIntFifoOverrun(bool & Value);
        bool UseIntFifoOverrun(bool Value);

        // State Machine

        // Data gathering
        bool ReadSensorDataOnce(Sensor::RawThermometerData & OutData);
        bool ReadSensorDataOnce(Sensor::RawAcceleromterData & OutData);

        Celcius ConvertToSIUnit(Sensor::RawThermometerData Data);
        GForce * ConvertToSIUnit(Sensor::RawAcceleromterData Data);
private:
        Device::RegPtr m_RegInfo1, m_RegInfo2, m_RegWhoAmI, m_RegCtrlReg3, m_RegCtrlReg4, m_RegCtrlReg5, m_RegCtrlReg6,
        m_RegStatus, m_RegOutT, m_RegOffX, m_RegOffY, m_RegOffZ, m_RegCsX, m_RegCsY, m_RegCsZ,
        m_RegLcL, m_RegLcH, m_RegStat, m_RegVfc1, m_RegVfc2, m_RegVfc3, m_RegVfc4, m_RegThrs3,
        m_RegOutX_L, m_RegOutX_H, m_RegOutY_L, m_RegOutY_H, m_RegOutZ_L, m_RegOutZ_H, m_RegFifoCtrl,
        m_RegFifoSrc;

        char * m_pcTxBuf = 0;
        char * m_pcRxBuf = 0;
        unsigned int m_unBuffSize;
        bool m_bReadInc;
        MeasureRange m_eRange;

        Device::eReturnCode MultiRead(Device::RegPtr psReg, unsigned int BytesToRead, char * pcRxData);
        Device::eReturnCode MultiWrite(Device::RegPtr psReg, unsigned int BytesToRead, char * pcTxData);
        Device::eReturnCode Read(Device::RegPtr psReg, char & cValue);
        Device::eReturnCode Write(Device::RegPtr psReg, char & cValue);

        inline void _BufferSize(unsigned int unSize);
        inline unsigned int _BufferSize();

        inline void _SetupBuffers(unsigned int BufLen);
        inline void _PopulateBuffer(char accessor);
        inline void _PopulateBuffer(char accessor, char * param);
        inline void _CleanBuffers();
        inline bool _BombsAway();
        inline GForce _GetSIRange(MeasureRange range);
};

}

#endif
