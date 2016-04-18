#ifndef ST_SENSOR
#define ST_SENSOR

#include <icdevice.h>

namespace Sensor{

class StSensorConfig : public Device::ICDevice {
    Size m_unBuffSize;
protected:
    ~StSensorConfig();

    Word * m_pcTxBuf = 0;
    Word * m_pcRxBuf = 0;
    bool m_bReadInc;

    Size _BufferSize();
    void _BufferSize(Size unSize);
    void _SetupBuffers(Size BufLen);
    void _PopulateBuffer(Word accessor);
    void _CleanBuffers();
    bool _BombsAway();

    virtual Device::eReturnCode MultiRead(Device::RegPtr psReg, Size BytesToRead, Word * pcRxData)=0;
    virtual Device::eReturnCode MultiWrite(Device::RegPtr psReg, Size BytesToRead, Word * pcTxData)=0;
    virtual Device::eReturnCode Read(Device::RegPtr psReg, Word & cValue)=0;
    virtual Device::eReturnCode Write(Device::RegPtr psReg, Word & cValue)=0;

public:
    virtual bool WhoAmI(Word & WhoAmI)=0;

    virtual bool GetBootTimeout(MicroSeconds & Value)=0;
    virtual bool SetBootTimeout(MicroSeconds Value)=0;

    virtual bool SoftReset()=0;
    virtual bool Reboot()=0;

    virtual bool IsSPI3WireMode(bool & Value)=0;
    virtual bool UseSPI3WireMode(bool Value)=0;

    virtual bool IsBDU(bool & Value)=0;
    virtual bool UseBDU(bool Value)=0;

    virtual bool IsReadInc(bool & Value)=0;
    virtual bool UseReadInc(bool Value)=0;

    virtual bool IsXAxisEnabled(bool & Value)=0;
    virtual bool IsYAxisEnabled(bool & Value)=0;
    virtual bool IsZAxisEnabled(bool & Value)=0;

    virtual bool UseAxisX(bool Value)=0;
    virtual bool UseAxisY(bool Value)=0;
    virtual bool UseAxisZ(bool Value)=0;

    virtual bool DataOverrunXYZ(bool & Value)=0;
    virtual bool DataOverrunX(bool & Value)=0;
    virtual bool DataOverrunY(bool & Value)=0;
    virtual bool DataOverrunZ(bool & Value)=0;

    virtual bool DataAvailableXYZ(bool & Value)=0;
    virtual bool DataAvailableX(bool & Value)=0;
    virtual bool DataAvailableY(bool & Value)=0;
    virtual bool DataAvailableZ(bool & Value)=0;
};

}

#endif
