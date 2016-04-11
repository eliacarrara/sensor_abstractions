#include "st_accel_dsh.h"
#include <malloc.h>
#include <spibus.h>
#include <error.h>
#include <iostream>
using namespace std;

StAccel_dsh::StAccel_dsh()
{
    Communication::SpiBus * clsBus = new Communication::SpiBus(ACC_SPI_DEV_FILE);

    if (!clsBus->Open())
        throw SPI_FILE_OPEN_ERROR;


    Communication::spi_mode mode = SPI_MODE_3;
    hertz speed = ACC_SPI_SPEED;

    if (!clsBus->SetMode(mode))
        throw SPI_PARAM_ERROR;
    if (!clsBus->SetSpeed(speed))
        throw SPI_PARAM_ERROR;
    if(mode != SPI_MODE_3 || speed != ACC_SPI_SPEED)
        throw SPI_PARAM_ERROR;

    InitIC(NUMBER_OF_REGISTERS,clsBus);

    // When adding new registers, make sure the address is in ascending order
    m_RegOutT = AddRegister(Device::InitRegister(READ_ACCESS, 0x0C, 0x00, 0xFF));
    m_RegInfo1 = AddRegister(Device::InitRegister(READ_ACCESS, 0x0D, 0x21, 0xFF));
    m_RegInfo2 = AddRegister(Device::InitRegister(READ_ACCESS, 0x0E, 0x00, 0xFF));
    m_RegWhoAmI = AddRegister(Device::InitRegister(READ_ACCESS, 0x0F, 0x3F, 0xFF));
    m_RegOffX = AddRegister(Device::InitRegister(FULL_ACCESS, 0x10, 0x00, 0x00));
    m_RegOffY = AddRegister(Device::InitRegister(FULL_ACCESS, 0x11, 0x00, 0x00));
    m_RegOffZ = AddRegister(Device::InitRegister(FULL_ACCESS, 0x12, 0x00, 0x00));
    m_RegCsX = AddRegister(Device::InitRegister(FULL_ACCESS, 0x13, 0x00, 0x00));
    m_RegCsY = AddRegister(Device::InitRegister(FULL_ACCESS, 0x14, 0x00, 0x00));
    m_RegCsZ = AddRegister(Device::InitRegister(FULL_ACCESS, 0x15, 0x00, 0x00));
    m_RegLcL = AddRegister(Device::InitRegister(FULL_ACCESS, 0x16, 0x00, 0x00));
    m_RegLcH = AddRegister(Device::InitRegister(FULL_ACCESS, 0x17, 0x00, 0x00));
    m_RegStat = AddRegister(Device::InitRegister(READ_ACCESS, 0x18, 0x00, 0xFF));
    m_RegVfc1 = AddRegister(Device::InitRegister(FULL_ACCESS, 0x1B, 0x00, 0x00));
    m_RegVfc2 = AddRegister(Device::InitRegister(FULL_ACCESS, 0x1C, 0x00, 0x00));
    m_RegVfc3 = AddRegister(Device::InitRegister(FULL_ACCESS, 0x1D, 0x00, 0x00));
    m_RegVfc4 = AddRegister(Device::InitRegister(FULL_ACCESS, 0x1E, 0x00, 0x00));
    m_RegThrs3 = AddRegister(Device::InitRegister(FULL_ACCESS, 0x1F, 0x00, 0x00));
    m_RegCtrlReg4 = AddRegister(Device::InitRegister(FULL_ACCESS, 0x20, 0x03, 0x00));
    m_RegCtrlReg3 = AddRegister(Device::InitRegister(FULL_ACCESS, 0x23, 0x00, 0x02));
    m_RegCtrlReg5 = AddRegister(Device::InitRegister(FULL_ACCESS, 0x24, 0x00, 0x00));
    m_RegCtrlReg6 = AddRegister(Device::InitRegister(FULL_ACCESS, 0x25, 0x00, 0x00));
    m_RegStatus = AddRegister(Device::InitRegister(READ_ACCESS, 0x27, 0x00, 0xFF));
    m_RegOutX_L = AddRegister(Device::InitRegister(READ_ACCESS, 0x28, 0x00, 0x00));
    m_RegOutX_H = AddRegister(Device::InitRegister(READ_ACCESS, 0x29, 0x00, 0x00));
    m_RegOutY_L = AddRegister(Device::InitRegister(READ_ACCESS, 0x2A, 0x00, 0x00));
    m_RegOutY_H = AddRegister(Device::InitRegister(READ_ACCESS, 0x2B, 0x00, 0x00));
    m_RegOutZ_L = AddRegister(Device::InitRegister(READ_ACCESS, 0x2C, 0x00, 0x00));
    m_RegOutZ_H = AddRegister(Device::InitRegister(READ_ACCESS, 0x2D, 0x00, 0x00));
    m_RegFifoCtrl = AddRegister(Device::InitRegister(FULL_ACCESS, 0x2E, 0x00, 0x00));
    m_RegFifoSrc = AddRegister(Device::InitRegister(READ_ACCESS, 0x2F, 0x00, 0xFF));

    if(!IsReadInc(m_bReadInc))
        throw REG_READ_ERROR;
    if(!GetRange(m_eRange))
        throw REG_READ_ERROR;
}
StAccel_dsh::~StAccel_dsh()
{
    m_clsBus->Close();
    if (m_clsBus != NULL){
        Communication::SpiBus * tmp = (Communication::SpiBus *)m_clsBus;
        delete tmp;
        m_clsBus = NULL;
    }
}

void StAccel_dsh::_BufferSize(unsigned int unSize)
{
    m_unBuffSize = unSize;
}
unsigned int StAccel_dsh::_BufferSize()
{
    return m_unBuffSize;
}
void StAccel_dsh::_SetupBuffers(unsigned int BufLen)
{
    m_pcRxBuf = (char*)calloc(BufLen, sizeof(char));
    m_pcTxBuf = (char*)calloc(BufLen, sizeof(char));
    _BufferSize(BufLen);
}
void StAccel_dsh::_PopulateBuffer(char accessor)
{
    m_pcTxBuf[0] = accessor;
}
void StAccel_dsh::_CleanBuffers()
{
    _BufferSize(0);
    free(m_pcRxBuf);
    free(m_pcTxBuf);
}
bool StAccel_dsh::_BombsAway()
{
    return ((Communication::SpiBus*)m_clsBus)->Transact(m_pcRxBuf,m_pcTxBuf, m_unBuffSize);
}

GForce StAccel_dsh::_GetSIRange(StAccel_dsh::MeasureRange range)
{
    switch (range) {
    case Range_0:
        return 2.0f;
    case Range_1:
        return 4.0f;
    case Range_2:
        return 6.0f;
    case Range_3:
        return 8.0f;
    case Range_4:
        return 16.0f;
    default:
        return 0.0f;
    }
}

Device::eReturnCode StAccel_dsh::MultiRead(Device::RegPtr psReg, unsigned int BytesToRead, char * pcRxData)
{
    // Checks if the access incrementing flag is set
    if (!m_bReadInc)
        return Device::MultiAccessDisabled;
    if(pcRxData == NULL)
        return Device::BufferError;

    // Creates new buffers and poplates it with data
    _SetupBuffers(BytesToRead + 1);
    _PopulateBuffer(ACC_SPI_MSG(psReg->cAddress,ACC_SPI_READ_FLAG));

    // Checks access rights
    for (unsigned int i = 0; i < BytesToRead; ++i)
        if (!CheckRegisterAccess(psReg++, READ_ACCESS))
            return Device::NoAccess;

    // Executes spi tranfer
    bool rtrn = _BombsAway();

    // reads data from the buffer
    for (unsigned int i = 0; i < BytesToRead; ++i)
        pcRxData[i] = m_pcRxBuf[i+1];

    // Frees resouces
    _CleanBuffers();

    if (rtrn){
        return Device::OK;
    }else
        return Device::TerribleError;
}
Device::eReturnCode StAccel_dsh::MultiWrite(Device::RegPtr psReg, unsigned int BytesToRead, char * pcTxData)
{
#pragma message "I never tested nor used this methond - use with caution"
    // Checks if the access incrementing flag is set
    if (!m_bReadInc)
        return Device::MultiAccessDisabled;
    if (pcTxData == NULL)
        return Device::BufferError;

    // Creates new buffers and poplates it with data
    _SetupBuffers(BytesToRead + 1);
    _PopulateBuffer(ACC_SPI_MSG(psReg->cAddress,ACC_SPI_WRITE_FLAG));

    for (unsigned int i = 0; i < BytesToRead; ++i) {
        m_pcTxBuf[i+1] = pcTxData[i];
    }

    // Checks access rights and if locked bits would get written on
    for (unsigned int i = 0; i < BytesToRead; ++i){
        if (!CheckRegisterAccess(psReg, READ_ACCESS))
            return Device::NoAccess;
        if (!CheckLockedBits(psReg, pcTxData[i]))
            return Device::WritingLockedBits;
        psReg++;
    }

    // Executes spi tranfer
    bool rtrn = _BombsAway();

    // Frees resouces
    _CleanBuffers();

    if (rtrn){
        return Device::OK;
    }else
        return Device::TerribleError;
}
Device::eReturnCode StAccel_dsh::Read(Device::RegPtr psReg, char & cValue)
{
    if (!CheckRegisterAccess(psReg, READ_ACCESS))
        return Device::NoAccess;

    _SetupBuffers(ACC_SPI_DEFAULT_BUFF_LEN);
    _PopulateBuffer(ACC_SPI_MSG(psReg->cAddress,ACC_SPI_READ_FLAG));

    if (_BombsAway()){
        cValue = m_pcRxBuf[1];
        return Device::OK;
    }else
        return Device::TerribleError;

    _CleanBuffers();
}
Device::eReturnCode StAccel_dsh::Write(Device::RegPtr psReg, char & cValue)
{
    if (!CheckRegisterAccess(psReg, WRITE_ACCESS))
        return Device::NoAccess;
    if (!CheckLockedBits(psReg, cValue))
        return Device::WritingLockedBits;

    _SetupBuffers(ACC_SPI_DEFAULT_BUFF_LEN);

    _PopulateBuffer(ACC_SPI_MSG(psReg->cAddress,ACC_SPI_WRITE_FLAG));
    m_pcTxBuf[1] = cValue;

    if (_BombsAway())
        return Device::OK;
    else
        return Device::TerribleError;

    _CleanBuffers();
}

bool StAccel_dsh::SoftReset()
{
    char out;
    Device::eReturnCode code = Read(m_RegCtrlReg3, out);
    if ( code != Device::OK)
        return false;

    out |= 0x01;
    code = Write(m_RegCtrlReg3, out);

    if ( code != Device::OK)
        return false;
    else
        return true;
}
bool StAccel_dsh::Reboot()
{
    char out;
    Device::eReturnCode code = Read(m_RegCtrlReg6,out);
    if ( code != Device::OK)
        return false;

    out |= 0x80;
    code = Write(m_RegCtrlReg6, out);

    if ( code != Device::OK)
        return false;
    else
        return true;
}

bool StAccel_dsh::ReadInfomation(unsigned short & unInfo)
{
    char code = Device::OK;
    char out[2]={0,0};

    if(m_bReadInc){
        code = MultiRead(m_RegInfo1, 2, out);
    }else{
        code |= Read(m_RegInfo1, out[0]);
        code |= Read(m_RegInfo2, out[1]);
    }

    if ( code != Device::OK)
        return false;
    unInfo =  *((short*)out);
    return true;
}
bool StAccel_dsh::WhoAmI(char & WhoAmI)
{
    char out;
    Device::eReturnCode code = Read(m_RegWhoAmI, out);

    if ( code != Device::OK)
        return false;

    WhoAmI = out;
    return true;
}

bool StAccel_dsh::GetODR(StAccel_dsh::ODR & Value)
{
    char out;
    Device::eReturnCode code = Read(m_RegCtrlReg4, out);

    if ( code != Device::OK)
        return false;

    Value = (ODR)((out & 0xF0) >> 4);
    return true;
}
bool StAccel_dsh::SetODR(StAccel_dsh::ODR Value)
{
    char out;
    Device::eReturnCode code = Read(m_RegCtrlReg4, out);
    if ( code != Device::OK)
        return false;

    out = ((out & 0x0F) | ((char)Value << 4));
    code = Write(m_RegCtrlReg4, out);

    if ( code != Device::OK)
        return false;
    else
        return true;
}

bool StAccel_dsh::GetRange(StAccel_dsh::MeasureRange & Value)
{
    char out;
    Device::eReturnCode code = Read(m_RegCtrlReg5, out);

    if (code != Device::OK)
        return false;

    Value = (MeasureRange)((out & 0x38) >> 3);
    m_eRange = Value;
    return true;
}
bool StAccel_dsh::SetRange(StAccel_dsh::MeasureRange Value)
{
    char out;
    Device::eReturnCode code = Read(m_RegCtrlReg5,out);
    if ( code != Device::OK)
        return false;

    out = (out & 0xC7) | (Value << 3);
    code = Write(m_RegCtrlReg5, out);

    if ( code != Device::OK)
        return false;

    m_eRange = Value;
    return true;
}

bool StAccel_dsh::IsFifoEnabled(bool & Value)
{
    char out;
    Device::eReturnCode code = Read(m_RegCtrlReg6, out);

    if ( code != Device::OK)
        return false;

    Value = ((out & 0x40) == 0x40);
    return true;
}
bool StAccel_dsh::UseFifo(bool Value)
{
    char out;
    Device::eReturnCode code = Read(m_RegCtrlReg6, out);
    if ( code != Device::OK)
        return false;

    if(Value)
        out |= 0x40;
    else
        out &= 0xBF;

    code = Write(m_RegCtrlReg6, out);
    if ( code != Device::OK)
        return false;
    else
        return true;
}

bool StAccel_dsh::IsFifoEmpty(bool &Value)
{
    char out;
    Device::eReturnCode code = Read(m_RegFifoSrc, out);

    if ( code != Device::OK)
        return false;

    Value = ((out & 0x20) == 0x20);
    return true;
}
bool StAccel_dsh::IsFifoOverrun(bool &Value)
{
    char out;
    Device::eReturnCode code = Read(m_RegFifoSrc, out);

    if ( code != Device::OK)
        return false;

    Value = ((out & 0x40) == 0x40);
    return true;
}

bool StAccel_dsh::DataOverrunXYZ(bool & Value)
{
    char out;
    Device::eReturnCode code = Read(m_RegStatus,out);
    if ( code != Device::OK)
        return false;
    Value = ((out & 0x80) == 0x80);
    return true;
}
bool StAccel_dsh::DataOverrunX(bool & Value)
{
    char out;
    Device::eReturnCode code = Read(m_RegStatus,out);
    if ( code != Device::OK)
        return false;
    Value = ((out & 0x10) == 0x10);
    return true;
}
bool StAccel_dsh::DataOverrunY(bool & Value)
{
    char out;
    Device::eReturnCode code = Read(m_RegStatus,out);
    if ( code != Device::OK)
        return false;
    Value = ((out & 0x20) == 0x20);
    return true;
}
bool StAccel_dsh::DataOverrunZ(bool & Value)
{
    char out;
    Device::eReturnCode code = Read(m_RegStatus,out);
    if ( code != Device::OK)
        return false;
    Value = ((out & 0x40) == 0x40);
    return true;
}

bool StAccel_dsh::DataAvailableXYZ(bool & Value)
{
    char out;
    Device::eReturnCode code = Read(m_RegStatus,out);
    if ( code != Device::OK)
        return false;
    Value = ((out & 0x08) == 0x08);
    return true;
}
bool StAccel_dsh::DataAvailableX(bool & Value)
{
    char out;
    Device::eReturnCode code = Read(m_RegStatus,out);
    if ( code != Device::OK)
        return false;
    Value = ((out & 0x01) == 0x01);
    return true;
}
bool StAccel_dsh::DataAvailableY(bool & Value)
{
    char out;
    Device::eReturnCode code = Read(m_RegStatus,out);
    if ( code != Device::OK)
        return false;
    Value = ((out & 0x02) == 0x02);
    return true;
}
bool StAccel_dsh::DataAvailableZ(bool & Value)
{
    char out;
    Device::eReturnCode code = Read(m_RegStatus,out);
    if ( code != Device::OK)
        return false;
    Value = ((out & 0x04) == 0x04);
    return true;
}

bool StAccel_dsh::IsXAxisEnabled(bool & Value)
{
    char out;
    Device::eReturnCode code = Read(m_RegCtrlReg4,out);
    if ( code != Device::OK)
        return false;
    Value = ((out & 0x04) == 0x04);
    return true;
}
bool StAccel_dsh::IsYAxisEnabled(bool & Value)
{
    char out;
    Device::eReturnCode code = Read(m_RegCtrlReg4,out);
    if ( code != Device::OK)
        return false;
    Value = ((out & 0x02) == 0x02);
    return true;
}
bool StAccel_dsh::IsZAxisEnabled(bool & Value)
{
    char out;
    Device::eReturnCode code = Read(m_RegCtrlReg4,out);
    if ( code != Device::OK)
        return false;
    Value = ((out & 0x01) == 0x01);
    return true;
}

bool StAccel_dsh::UseAxisX(bool Value)
{
    char out;
    Device::eReturnCode code = Read(m_RegCtrlReg4,out);
    if ( code != Device::OK)
        return false;
    if(Value)
        out |= 0x04;
    else
        out &= 0xFB;

    code = Write(m_RegCtrlReg4, out);
    if ( code != Device::OK)
        return false;
    else
        return true;
}
bool StAccel_dsh::UseAxisY(bool Value)
{
    char out;
    Device::eReturnCode code = Read(m_RegCtrlReg4,out);
    if ( code != Device::OK)
        return false;
    if(Value)
        out |= 0x02;
    else
        out &= 0xFD;

    code = Write(m_RegCtrlReg4, out);
    if ( code != Device::OK)
        return false;
    else
        return true;
}
bool StAccel_dsh::UseAxisZ(bool Value)
{
    char out;
    Device::eReturnCode code = Read(m_RegCtrlReg4,out);
    if ( code != Device::OK)
        return false;
    if(Value)
        out |= 0x01;
    else
        out &= 0xFE;

    code = Write(m_RegCtrlReg4, out);
    if ( code != Device::OK)
        return false;
    else
        return true;
}

bool StAccel_dsh::IsBDU(bool & Value)
{
    char out;
    Device::eReturnCode code = Read(m_RegCtrlReg4,out);
    if ( code != Device::OK)
        return false;
    Value = ((out & 0x08) == 0x08);
    return true;
}
bool StAccel_dsh::UseBDU(bool Value)
{
    char out;
    Device::eReturnCode code = Read(m_RegCtrlReg4,out);
    if ( code != Device::OK)
        return false;
    if(Value)
        out |= 0x08;
    else
        out &= 0xF7;

    code = Write(m_RegCtrlReg4, out);
    if ( code != Device::OK)
        return false;
    else
        return true;
}

bool StAccel_dsh::IsReadInc(bool & Value)
{
    char out;
    Device::eReturnCode code = Read(m_RegCtrlReg6,out);

    if ( code != Device::OK)
        return false;
    Value = ((out & 0x10) == 0x10);
    m_bReadInc = Value;
    return true;
}
bool StAccel_dsh::UseReadInc(bool Value)
{
    char out;
    Device::eReturnCode code = Read(m_RegCtrlReg6,out);
    if ( code != Device::OK)
        return false;
    if(Value)
        out |= 0x10;
    else
        out &= 0xEF;

    code = Write(m_RegCtrlReg6, out);
    if ( code != Device::OK)
        return false;
    m_bReadInc = Value;
    return true;
}

bool StAccel_dsh::ReadSensorDataOnce(Sensor::RawThermometerData & OutData)
{
    char value;
    Device::eReturnCode code = Read(m_RegOutT, value);
    if ( code != Device::OK)
        return false;
    OutData.nTemp = value;
    return true;
}
bool StAccel_dsh::ReadSensorDataOnce(Sensor::RawAcceleromterData & OutData)
{
    char out[6]= {0,0,0,0,0,0};
    char code = Device::OK;

    if(m_bReadInc){
        code = MultiRead(m_RegOutX_L, 6, out);
    }else{
        code |= Read(m_RegOutX_L, out[0]);
        code |= Read(m_RegOutX_H, out[1]);
        code |= Read(m_RegOutY_L, out[2]);
        code |= Read(m_RegOutY_H, out[3]);
        code |= Read(m_RegOutZ_L, out[4]);
        code |= Read(m_RegOutZ_H, out[5]);
    }

    if ( code != Device::OK)
        return false;

    short tmp = out[1];
    tmp <<= 8;
    tmp |= out[0];
    OutData.nX_Data = tmp;

    tmp = out[3];
    tmp <<= 8;
    tmp |= out[2];
    OutData.nY_Data = tmp;

    tmp = out[5];
    tmp <<= 8;
    tmp |= out[4];
    OutData.nZ_Data = tmp;

    return true;
}

Celcius StAccel_dsh::ConvertToSIUnit(Sensor::RawThermometerData Data)
{
    signed char tmp = (signed char)Data.nTemp;
    return (float)(25 + tmp);
}
GForce * StAccel_dsh::ConvertToSIUnit(Sensor::RawAcceleromterData Data)
{
    GForce * out= (GForce*)calloc(3,sizeof(float));
    out[0] = (float)((float)Data.nX_Data / 32767.0F) * _GetSIRange(m_eRange);
    out[1] = (float)((float)Data.nY_Data / 32767.0F) * _GetSIRange(m_eRange);
    out[2] = (float)((float)Data.nZ_Data / 32767.0F) * _GetSIRange(m_eRange);
    return out;
}
