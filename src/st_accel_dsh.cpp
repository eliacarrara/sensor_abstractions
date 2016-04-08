#include "st_accel_dsh.h"
#include <malloc.h>
#include <spibus.h>
#include <error.h>

StAccel_dsh::StAccel_dsh()
{
    SpiBus * clsBus = new SpiBus(ACC_SPI_DEV_FILE);

    if (!clsBus->Open())
        throw SPI_FILE_OPEN_ERROR;


    spi_mode mode = SPI_MODE_3;
    hertz speed = ACC_SPI_SPEED;

    if (!clsBus->SetMode(mode))
        throw SPI_PARAM_ERROR;
    if (!clsBus->SetSpeed(speed))
        throw SPI_PARAM_ERROR;
    if(mode != SPI_MODE_3 || speed != ACC_SPI_SPEED)
        throw SPI_PARAM_ERROR;

    InitIC(NUMBER_OF_REGISTERS,clsBus);

    // When adding new registers, make sure the address is in ascending order
    m_RegOutT = AddRegister(InitRegister(READ_ACCESS, 0x0C, 0x00, 0xFF));
    m_RegInfo1 = AddRegister(InitRegister(READ_ACCESS, 0x0D, 0x21, 0xFF));
    m_RegInfo2 = AddRegister(InitRegister(READ_ACCESS, 0x0E, 0x00, 0xFF));
    m_RegWhoAmI = AddRegister(InitRegister(READ_ACCESS, 0x0F, 0x3F, 0xFF));
    m_RegOffX = AddRegister(InitRegister(FULL_ACCESS, 0x10, 0x00, 0x00));
    m_RegOffY = AddRegister(InitRegister(FULL_ACCESS, 0x11, 0x00, 0x00));
    m_RegOffZ = AddRegister(InitRegister(FULL_ACCESS, 0x12, 0x00, 0x00));
    m_RegCsX = AddRegister(InitRegister(FULL_ACCESS, 0x13, 0x00, 0x00));
    m_RegCsY = AddRegister(InitRegister(FULL_ACCESS, 0x14, 0x00, 0x00));
    m_RegCsZ = AddRegister(InitRegister(FULL_ACCESS, 0x15, 0x00, 0x00));
    m_RegLcL = AddRegister(InitRegister(FULL_ACCESS, 0x16, 0x00, 0x00));
    m_RegLcH = AddRegister(InitRegister(FULL_ACCESS, 0x17, 0x00, 0x00));
    m_RegStat = AddRegister(InitRegister(READ_ACCESS, 0x18, 0x00, 0xFF));
    m_RegVfc1 = AddRegister(InitRegister(FULL_ACCESS, 0x1B, 0x00, 0x00));
    m_RegVfc2 = AddRegister(InitRegister(FULL_ACCESS, 0x1C, 0x00, 0x00));
    m_RegVfc3 = AddRegister(InitRegister(FULL_ACCESS, 0x1D, 0x00, 0x00));
    m_RegVfc4 = AddRegister(InitRegister(FULL_ACCESS, 0x1E, 0x00, 0x00));
    m_RegThrs3 = AddRegister(InitRegister(FULL_ACCESS, 0x1F, 0x00, 0x00));
    m_RegCtrlReg4 = AddRegister(InitRegister(FULL_ACCESS, 0x20, 0x03, 0x00));
    m_RegCtrlReg3 = AddRegister(InitRegister(FULL_ACCESS, 0x23, 0x00, 0x02));
    m_RegCtrlReg5 = AddRegister(InitRegister(FULL_ACCESS, 0x24, 0x00, 0x00));
    m_RegCtrlReg6 = AddRegister(InitRegister(FULL_ACCESS, 0x25, 0x00, 0x00));
    m_RegStatus = AddRegister(InitRegister(READ_ACCESS, 0x27, 0x00, 0xFF));
    m_RegOutX_L = AddRegister(InitRegister(READ_ACCESS, 0x28, 0x00, 0x00));
    m_RegOutX_H = AddRegister(InitRegister(READ_ACCESS, 0x29, 0x00, 0x00));
    m_RegOutY_L = AddRegister(InitRegister(READ_ACCESS, 0x2A, 0x00, 0x00));
    m_RegOutY_H = AddRegister(InitRegister(READ_ACCESS, 0x2B, 0x00, 0x00));
    m_RegOutZ_L = AddRegister(InitRegister(READ_ACCESS, 0x2C, 0x00, 0x00));
    m_RegOutZ_H = AddRegister(InitRegister(READ_ACCESS, 0x2D, 0x00, 0x00));
    m_RegFifoCtrl = AddRegister(InitRegister(FULL_ACCESS, 0x2E, 0x00, 0x00));
    m_RegFifoSrc = AddRegister(InitRegister(READ_ACCESS, 0x2F, 0x00, 0xFF));

    if(!IsReadInc(m_bReadInc))
        throw REG_READ_ERROR;

}
StAccel_dsh::~StAccel_dsh()
{
    m_clsBus->Close();
    if (m_clsBus != NULL){
        SpiBus * tmp = (SpiBus *)m_clsBus;
        delete tmp;
        m_clsBus = NULL;
    }
}

void StAccel_dsh::BufferSize(unsigned int unSize)
{
    m_unBuffSize = unSize;
}
unsigned int StAccel_dsh::BufferSize()
{
    return m_unBuffSize;
}
void StAccel_dsh::SetupBuffers(unsigned int BufLen)
{
    m_pcRxBuf = (char*)calloc(BufLen, sizeof(char));
    m_pcTxbuf = (char*)calloc(BufLen, sizeof(char));
    BufferSize(BufLen);
}
void StAccel_dsh::PopulateBuffer(char accessor)
{
    m_pcTxbuf[0] = accessor;
}
void StAccel_dsh::CleanBuffers()
{
    BufferSize(0);
    free(m_pcRxBuf);
    free(m_pcTxbuf);
}
bool StAccel_dsh::BombsAway()
{
    return ((SpiBus*)m_clsBus)->Transact(m_pcRxBuf,m_pcTxbuf, m_unBuffSize);
}

eReturnCode StAccel_dsh::MultiRead(RegPtr psReg, unsigned int BytesToRead, char * pcRxData)
{
#pragma message "test this very well"
    // Checks if the access incrementing flag is set
    if (!m_bReadInc)
        return MultiAccessDisabled;

    // Creates new buffers and poplates it with data
    SetupBuffers(BytesToRead + 1);
    PopulateBuffer(psReg->cAddress);

    // Checks access rights
    for (unsigned int i = 0; i < BytesToRead; ++i)
        if (!CheckRegisterAccess(psReg++, READ_ACCESS))
            return NoAccess;

    // Executes spi tranfer
    bool rtrn = BombsAway();

    // Frees resouces
    CleanBuffers();

    if (rtrn){
        // reads data from the buffer
        pcRxData = (char*)malloc(BytesToRead);
        for (unsigned int i = 1; i < m_unBuffSize; ++i)
            pcRxData[i-1] = m_pcRxBuf[i];
        return OK;
    }else
        return TerribleError;
}
eReturnCode StAccel_dsh::MultiWrite(RegPtr psReg, unsigned int BytesToRead, char * pcTxData)
{
#pragma message "test this very well"
    // Checks if the access incrementing flag is set
    if (!m_bReadInc)
        return MultiAccessDisabled;
    if (pcTxData == NULL)
        return NothingToWrite;

    // Creates new buffers and poplates it with data
    SetupBuffers(BytesToRead + 1);
    PopulateBuffer(psReg->cAddress);

    for (unsigned int i = 0; i < BytesToRead; ++i) {
        m_pcTxbuf[i+1] = pcTxData[i];
    }

    // Checks access rights and if locked bits would get written on
    for (unsigned int i = 0; i < BytesToRead; ++i){
        if (!CheckRegisterAccess(psReg, READ_ACCESS))
            return NoAccess;
        if (!CheckLockedBits(psReg, pcTxData[i]))
            return WritingLockedBits;
        psReg++;
    }

    // Executes spi tranfer
    bool rtrn = BombsAway();

    // Frees resouces
    CleanBuffers();

    if (rtrn){
        return OK;
    }else
        return TerribleError;
}
eReturnCode StAccel_dsh::Read(RegPtr psReg, char & cValue)
{
    if (!CheckRegisterAccess(psReg, READ_ACCESS))
        return NoAccess;

    SetupBuffers(ACC_SPI_DEFAULT_BUFF_LEN);
    PopulateBuffer(psReg->cAddress);

    if (BombsAway()){
        cValue = m_pcRxBuf[1];
        return OK;
    }else
        return TerribleError;

    CleanBuffers();
}
eReturnCode StAccel_dsh::Write(RegPtr psReg, char & cValue)
{
    if (!CheckRegisterAccess(psReg, WRITE_ACCESS))
        return NoAccess;
    if (!CheckLockedBits(psReg, cValue))
        return WritingLockedBits;

    SetupBuffers(ACC_SPI_DEFAULT_BUFF_LEN);

    PopulateBuffer(psReg->cAddress);
    m_pcTxbuf[1] = cValue;

    if (BombsAway())
        return OK;
    else
        return TerribleError;

    CleanBuffers();
}

bool StAccel_dsh::SoftReset()
{
    char out;
    eReturnCode code = Read(m_RegCtrlReg3, out);
    if ( code != OK)
        return false;

    out |= 0x01;
    code = Write(m_RegCtrlReg3, out);

    if ( code != OK)
        return false;
    else
        return true;
}
bool StAccel_dsh::Reboot()
{
    char out;
    eReturnCode code = Read(m_RegCtrlReg6,out);
    if ( code != OK)
        return false;

    out |= 0x80;
    code = Write(m_RegCtrlReg6, out);

    if ( code != OK)
        return false;
    else
        return true;
}

bool StAccel_dsh::ReadInfomation(unsigned int & unInfo)
{
    char code = OK;
    char out[2];

    if(m_bReadInc){
        code = MultiRead(m_RegInfo1, 3, out);
    }else{
        code |= Read(m_RegInfo1, out[0]);
        code |= Read(m_RegInfo2, out[1]);
    }

    if ( code != OK)
        return false;

    unInfo =  *((short*)out);
    return true;
}
bool StAccel_dsh::ReadWhoAmI(char & WhoAmI)
{
    char out;
    eReturnCode code = Read(m_RegWhoAmI,out);

    if ( code != OK)
        return false;

    WhoAmI = out;
    return true;
}

bool StAccel_dsh::GetODR(StAccel_dsh::ODR & Value)
{
    char out;
    eReturnCode code = Read(m_RegCtrlReg4, out);

    if ( code != OK)
        return false;

    Value = (ODR)((out & 0xF0) >> 4);
    return true;
}
bool StAccel_dsh::SetODR(StAccel_dsh::ODR Value)
{
    char out;
    eReturnCode code = Read(m_RegCtrlReg4, out);
    if ( code != OK)
        return false;

    out = (out & 0x0F) | (Value << 4);
    code = Write(m_RegCtrlReg4, out);

    if ( code != OK)
        return false;
    else
        return true;
}

bool StAccel_dsh::GetRange(StAccel_dsh::MeasureRange & Value)
{
    char out;
    eReturnCode code = Read(m_RegCtrlReg5, out);

    if (code != OK)
        return false;

    Value = (MeasureRange)((out & 0x38) >> 3);
    return true;
}
bool StAccel_dsh::SetRange(StAccel_dsh::MeasureRange Value)
{
    char out;
    eReturnCode code = Read(m_RegCtrlReg5,out);
    if ( code != OK)
        return false;

    out = (out & 0xC7) | (Value << 3);
    code = Write(m_RegCtrlReg5, out);

    if ( code != OK)
        return false;
    else
        return true;
}

bool StAccel_dsh::IsFifoEnabled(bool & Value)
{
    char out;
    eReturnCode code = Read(m_RegCtrlReg6, out);

    if ( code != OK)
        return false;

    Value = ((out & 0x40) == 0x40);
    return true;
}
bool StAccel_dsh::UseFifo(bool Value)
{
    char out;
    eReturnCode code = Read(m_RegCtrlReg6, out);
    if ( code != OK)
        return false;

    if(Value)
        out |= 0x40;
    else
        out &= 0xBF;

    code = Write(m_RegCtrlReg6, out);
    if ( code != OK)
        return false;
    else
        return true;
}

bool StAccel_dsh::DataOverrunXYZ(bool & Value)
{
    char out;
    eReturnCode code = Read(m_RegStatus,out);
    if ( code != OK)
        return false;
    Value = ((out & 0x80) == 0x80);
    return true;
}
bool StAccel_dsh::DataOverrunX(bool & Value)
{
    char out;
    eReturnCode code = Read(m_RegStatus,out);
    if ( code != OK)
        return false;
    Value = ((out & 0x10) == 0x10);
    return true;
}
bool StAccel_dsh::DataOverrunY(bool & Value)
{
    char out;
    eReturnCode code = Read(m_RegStatus,out);
    if ( code != OK)
        return false;
    Value = ((out & 0x20) == 0x20);
    return true;
}
bool StAccel_dsh::DataOverrunZ(bool & Value)
{
    char out;
    eReturnCode code = Read(m_RegStatus,out);
    if ( code != OK)
        return false;
    Value = ((out & 0x40) == 0x40);
    return true;
}

bool StAccel_dsh::DataAvailableXYZ(bool & Value)
{
    char out;
    eReturnCode code = Read(m_RegStatus,out);
    if ( code != OK)
        return false;
    Value = ((out & 0x08) == 0x08);
    return true;
}
bool StAccel_dsh::DataAvailableX(bool & Value)
{
    char out;
    eReturnCode code = Read(m_RegStatus,out);
    if ( code != OK)
        return false;
    Value = ((out & 0x01) == 0x01);
    return true;
}
bool StAccel_dsh::DataAvailableY(bool & Value)
{
    char out;
    eReturnCode code = Read(m_RegStatus,out);
    if ( code != OK)
        return false;
    Value = ((out & 0x02) == 0x02);
    return true;
}
bool StAccel_dsh::DataAvailableZ(bool & Value)
{
    char out;
    eReturnCode code = Read(m_RegStatus,out);
    if ( code != OK)
        return false;
    Value = ((out & 0x04) == 0x04);
    return true;
}

bool StAccel_dsh::IsXAxisEnabled(bool & Value)
{
    char out;
    eReturnCode code = Read(m_RegCtrlReg4,out);
    if ( code != OK)
        return false;
    Value = ((out & 0x04) == 0x04);
    return true;
}
bool StAccel_dsh::IsYAxisEnabled(bool & Value)
{
    char out;
    eReturnCode code = Read(m_RegCtrlReg4,out);
    if ( code != OK)
        return false;
    Value = ((out & 0x02) == 0x02);
    return true;
}
bool StAccel_dsh::IsZAxisEnabled(bool & Value)
{
    char out;
    eReturnCode code = Read(m_RegCtrlReg4,out);
    if ( code != OK)
        return false;
    Value = ((out & 0x01) == 0x01);
    return true;
}

bool StAccel_dsh::UseAxisX(bool Value)
{
    char out;
    eReturnCode code = Read(m_RegCtrlReg4,out);
    if ( code != OK)
        return false;
    if(Value)
        out |= 0x04;
    else
        out &= 0xFB;

    code = Write(m_RegCtrlReg4, out);
    if ( code != OK)
        return false;
    else
        return true;
}
bool StAccel_dsh::UseAxisY(bool Value)
{
    char out;
    eReturnCode code = Read(m_RegCtrlReg4,out);
    if ( code != OK)
        return false;
    if(Value)
        out |= 0x02;
    else
        out &= 0xFD;

    code = Write(m_RegCtrlReg4, out);
    if ( code != OK)
        return false;
    else
        return true;
}
bool StAccel_dsh::UseAxisZ(bool Value)
{
    char out;
    eReturnCode code = Read(m_RegCtrlReg4,out);
    if ( code != OK)
        return false;
    if(Value)
        out |= 0x01;
    else
        out &= 0xFE;

    code = Write(m_RegCtrlReg4, out);
    if ( code != OK)
        return false;
    else
        return true;
}

bool StAccel_dsh::IsBSU(bool & Value)
{
    char out;
    eReturnCode code = Read(m_RegCtrlReg4,out);
    if ( code != OK)
        return false;
    Value = ((out & 0x08) == 0x08);
    return true;
}
bool StAccel_dsh::UseBSU(bool Value)
{
    char out;
    eReturnCode code = Read(m_RegCtrlReg4,out);
    if ( code != OK)
        return false;
    if(Value)
        out |= 0x08;
    else
        out &= 0xF7;

    code = Write(m_RegCtrlReg4, out);
    if ( code != OK)
        return false;
    else
        return true;
}

bool StAccel_dsh::IsReadInc(bool & Value)
{
    char out;
    eReturnCode code = Read(m_RegCtrlReg6,out);
    if ( code != OK)
        return false;
    Value = ((out & 0x10) == 0x10);
    m_bReadInc = Value;
    return true;
}
bool StAccel_dsh::UseReadInc(bool Value)
{
    char out;
    eReturnCode code = Read(m_RegCtrlReg6,out);
    if ( code != OK)
        return false;
    if(Value)
        out |= 0x10;
    else
        out &= 0xEF;

    code = Write(m_RegCtrlReg6, out);
    if ( code != OK)
        return false;
    m_bReadInc = Value;
    return true;
}

bool StAccel_dsh::ReadSensorDataOnce(Sensor::RawThermometerData & OutData)
{
    char value;
    eReturnCode code = Read(m_RegOutT, value);
    if ( code != OK)
        return false;
    OutData.nTemp = value;
    return true;
}
bool StAccel_dsh::ReadSensorDataOnce(Sensor::RawAcceleromterData & OutData)
{

    return false;
}

Celcius StAccel_dsh::ConvertToSIUnit(Sensor::RawThermometerData Data)
{
    return 0.0;
}
GForge StAccel_dsh::ConvertToSIUnit(Sensor::RawAcceleromterData Data)
{
    return 0.0;
}
