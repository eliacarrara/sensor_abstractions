#ifndef ST_LIS3DSH_A_H
#define ST_LIS3DSH_A_H

#include <accelerometer.h>
#include <thermometer.h>
#include <icdevice.h>
#include <stdlib.h>

#define NUMBER_OF_REGISTERS 31
#define ACC_SPI_DEV_FILE "/dev/spidev1.0"
#define ACC_SPI_SPEED 1000000

#define ACC_SPI_READ_FLAG 0x80
#define ACC_SPI_WRITE_FLAG 0x00
#define ACC_SPI_MSG(REG_ID,FLAG) ((REG_ID & 0x7F) | FLAG)
#define ACC_SPI_DEFAULT_BUFF_LEN 2

class StAccel_dsh : public Sensor::Thermometer, public Sensor::Accelerometer, public ICDevice {
        // Registers
        RegPtr m_RegInfo1, m_RegInfo2, m_RegWhoAmI, m_RegCtrlReg3, m_RegCtrlReg4, m_RegCtrlReg5, m_RegCtrlReg6,
        m_RegStatus, m_RegOutT, m_RegOffX, m_RegOffY, m_RegOffZ, m_RegCsX, m_RegCsY, m_RegCsZ,
        m_RegLcL, m_RegLcH, m_RegStat, m_RegVfc1, m_RegVfc2, m_RegVfc3, m_RegVfc4, m_RegThrs3,
        m_RegOutX_L, m_RegOutX_H, m_RegOutY_L, m_RegOutY_H, m_RegOutZ_L, m_RegOutZ_H, m_RegFifoCtrl,
        m_RegFifoSrc;

        char * m_pcTxBuf = 0;
        char * m_pcRxBuf = 0;
        unsigned int m_unBuffSize;
        bool m_bReadInc;

        eReturnCode MultiRead(RegPtr psReg, unsigned int BytesToRead, char * pcRxData);
        eReturnCode MultiWrite(RegPtr psReg, unsigned int BytesToRead, char * pcTxData);
        eReturnCode Read(RegPtr psReg, char & cValue);
        eReturnCode Write(RegPtr psReg, char & cValue);

        inline void _BufferSize(unsigned int unSize);
        inline unsigned int _BufferSize();

        inline void _SetupBuffers(unsigned int BufLen);
        inline void _PopulateBuffer(char accessor);
        inline void _PopulateBuffer(char accessor, char * param);
        inline void _CleanBuffers();
        inline bool _BombsAway();
    public:
        StAccel_dsh();
        ~StAccel_dsh();

        enum ODR{
            ODR_OFF         = 0x0,    // PowerDown
            ODR_SPEED_0     = 0x1,  // 3.125 Hz
            ODR_SPEED_1     = 0x2,  // 6.25 Hz
            ODR_SPEED_2     = 0x3,  // 12.5 Hz
            ODR_SPEED_3     = 0x4,  // 25 Hz
            ODR_SPEED_4     = 0x5,  // 50 Hz
            ODR_SPEED_5     = 0x6,  // 100 Hz
            ODR_SPEED_6     = 0x7,  // 400 Hz
            ODR_SPEED_7     = 0x8,  // 800 Hz
            ODR_SPEED_8     = 0x9   // 1600 Hz
        };
        enum MeasureRange{
            Range_0 = 0x0,  // ± 2 g
            Range_1 = 0x1,  // ± 4 g
            Range_2 = 0x2,  // ± 6 g
            Range_3 = 0x3,  // ± 8 g
            Range_4 = 0x4,  // ± 16 g
        };

        bool SoftReset();
        bool Reboot();

        bool ReadInfomation(unsigned short & unInfo);
        bool WhoAmI(char & WhoAmI);

        bool GetODR(ODR & Value);
        bool SetODR(ODR Value);

        bool GetRange(MeasureRange & Value);
        bool SetRange(MeasureRange Value);

        bool IsFifoEnabled(bool & Value);
        bool UseFifo(bool Value);

        bool DataOverrunXYZ(bool & Value);
        bool DataOverrunX(bool & Value);
        bool DataOverrunY(bool & Value);
        bool DataOverrunZ(bool & Value);

        bool DataAvailableXYZ(bool & Value);
        bool DataAvailableX(bool & Value);
        bool DataAvailableY(bool & Value);
        bool DataAvailableZ(bool & Value);

        bool IsXAxisEnabled(bool & Value);
        bool IsYAxisEnabled(bool & Value);
        bool IsZAxisEnabled(bool & Value);

        bool UseAxisX(bool Value);
        bool UseAxisY(bool Value);
        bool UseAxisZ(bool Value);

        bool IsBSU(bool & Value);
        bool UseBSU(bool Value);

        bool IsReadInc(bool & Value);
        bool UseReadInc(bool Value);

        //Get/Set Int Sig DRDY
        //Get/Set Int Sig Polarity
        //Get/Set Int Sig Latching/Pulsed
        //Get/Set Int Sig 2 Enabled/Disabled on 2
        //Get/Set Int Sig 2 Enabled/Disabled on 1
        //Get/Set Vector Filter Enabled/Disabled
        //Get/Set Anti aliasing filder bandwith
        //Get/Set Selftest
        //Get/Set SPI 3Wire Mode
        //Get/Set fifo watermark level
        //Get/Set fifo empty int
        //Get/Set fifo watermark int
        //Get/Set fifo overrun int
        //Get/Set boot int
        //Get/Set Offset (x,y,z)
        //Get/Set Constant shift (x,y,z)
        //Long counter?
        //stat reg?
        //vfc ?
        //threshold ?
        //Get out data
        //Get/Set Fifo mode
        //Get/Set Watermark pointer?
        //Get Watermark status
        //Get is fifo overrun
        //Get is fifo empty
        //fifo stored data level?

        bool ReadSensorDataOnce(Sensor::RawThermometerData & OutData);
        bool ReadSensorDataOnce(Sensor::RawAcceleromterData & OutData);

        Celcius ConvertToSIUnit(Sensor::RawThermometerData Data);
        GForge ConvertToSIUnit(Sensor::RawAcceleromterData Data);
};

#endif
