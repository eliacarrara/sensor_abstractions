#include <st_accel_dsh.h>
#include <spibus.h>
#include <error.h>

namespace Sensor{

StAccel_dsh::StAccel_dsh()
{
    Communication::SpiBus * clsBus = new Communication::SpiBus(ACC_SPI_DEV_FILE);

    if (!clsBus->Open())
        throw SPI_FILE_OPEN_ERROR;

    Communication::spi_mode mode = SPI_MODE_3;
    Hertz speed = ACC_SPI_SPEED;

    if (!clsBus->SetMode(mode))
        throw SPI_PARAM_ERROR;
    if (!clsBus->SetSpeed(speed))
        throw SPI_PARAM_ERROR;
    if(mode != SPI_MODE_3 || speed != ACC_SPI_SPEED)
        throw SPI_PARAM_ERROR;

    InitIC(MAG_NR_OF_REGISTERS,clsBus);

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

GForce StAccel_dsh::_GetSIRange(StAccel_dsh::MeasureRange range)
{
    switch (range) {
    case RANGE_0:
        return 2.0f;
    case RANGE_1:
        return 4.0f;
    case RANGE_2:
        return 6.0f;
    case RANGE_3:
        return 8.0f;
    case RANGE_4:
        return 16.0f;
    default:
        return 0.0f;
    }
}

Device::eReturnCode StAccel_dsh::MultiRead(Device::RegPtr psReg, Size BytesToRead, Word * pcRxData)
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
    for (Size i = 0; i < BytesToRead; ++i)
        if (!CheckRegisterAccess(psReg++, READ_ACCESS))
            return Device::NoAccess;

    // Executes spi tranfer
    bool rtrn = _BombsAway();

    // reads data from the buffer
    for (Size i = 0; i < BytesToRead; ++i)
        pcRxData[i] = m_pcRxBuf[i+1];

    // Frees resouces
    _CleanBuffers();

    if (rtrn){
        return Device::OK;
    }else
        return Device::TerribleError;
}
Device::eReturnCode StAccel_dsh::MultiWrite(Device::RegPtr psReg, Size BytesToRead, Word * pcTxData)
{
    // Checks if the access incrementing flag is set
    if (!m_bReadInc)
        return Device::MultiAccessDisabled;
    if (pcTxData == NULL)
        return Device::BufferError;

    // Creates new buffers and poplates it with data
    _SetupBuffers(BytesToRead + 1);
    _PopulateBuffer(ACC_SPI_MSG(psReg->cAddress,ACC_SPI_WRITE_FLAG));

    for (Size i = 0; i < BytesToRead; ++i) {
        m_pcTxBuf[i+1] = pcTxData[i];
    }

    // Checks access rights and if locked bits would get written on
    for (Size i = 0; i < BytesToRead; ++i){
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
Device::eReturnCode StAccel_dsh::Read(Device::RegPtr psReg, Word & cValue)
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
Device::eReturnCode StAccel_dsh::Write(Device::RegPtr psReg, Word & cValue)
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
    Word out;
    Device::eReturnCode code = Read(m_RegCtrlReg3, out);
    if ( code != Device::OK)
        return false;

    out |= 0x01;
    code = Write(m_RegCtrlReg3, out);

    return (code == Device::OK);
}
bool StAccel_dsh::Reboot()
{
    Word out;
    Device::eReturnCode code = Read(m_RegCtrlReg6, out);
    if ( code != Device::OK)
        return false;

    out |= 0x80;
    code = Write(m_RegCtrlReg6, out);

    return (code == Device::OK);
}

bool StAccel_dsh::ReadInfomation(DoubleWord & unInfo)
{
    Word code = Device::OK;
    Word out[2]={0,0};

    if(m_bReadInc){
        code = MultiRead(m_RegInfo1, 2, out);
    }else{
        code |= Read(m_RegInfo1, out[0]);
        code |= Read(m_RegInfo2, out[1]);
    }

    unInfo =  *((DoubleWord*)out);
    return (code == Device::OK);
}
bool StAccel_dsh::WhoAmI(Word & WhoAmI)
{
    Word out;
    Device::eReturnCode code = Read(m_RegWhoAmI, out);

    WhoAmI = out;
    return (code == Device::OK);
}

bool StAccel_dsh::GetODR(StAccel_dsh::ODR & Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegCtrlReg4, out);

    Value = (ODR)((out & 0xF0) >> 4);
    return (code == Device::OK);
}
bool StAccel_dsh::SetODR(StAccel_dsh::ODR Value)
{
    Word out;
    if(Value > ODR_SPEED_8)
        return false;

    Device::eReturnCode code = Read(m_RegCtrlReg4, out);
    if (code != Device::OK)
        return false;

    out = ((out & 0x0F) | ((Word)Value << 4));
    code = Write(m_RegCtrlReg4, out);

    return (code == Device::OK);
}

bool StAccel_dsh::GetRange(StAccel_dsh::MeasureRange & Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegCtrlReg5, out);

    Value = (MeasureRange)((out & 0x38) >> 3);
    m_eRange = Value;
    return (code == Device::OK);
}
bool StAccel_dsh::SetRange(StAccel_dsh::MeasureRange Value)
{
    Word out;
    if(Value > RANGE_4)
        return false;

    Device::eReturnCode code = Read(m_RegCtrlReg5,out);
    if ( code != Device::OK)
        return false;

    out = (out & 0xC7) | (Value << 3);
    code = Write(m_RegCtrlReg5, out);

    m_eRange = Value;
    return (code == Device::OK);
}

bool StAccel_dsh::GetBootTimeout(MicroSeconds &Value)
{

}
bool StAccel_dsh::SetBootTimeout(MicroSeconds Value)
{

}

bool StAccel_dsh::IsFifoEnabled(bool & Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegCtrlReg6, out);

    if ( code != Device::OK)
        return false;

    Value = ((out & 0x40) == 0x40);
    return true;
}
bool StAccel_dsh::UseFifo(bool Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegCtrlReg6, out);
    if ( code != Device::OK)
        return false;

    if(Value)
        out |= 0x40;
    else
        out &= 0xBF;

    code = Write(m_RegCtrlReg6, out);
    return (code == Device::OK);
}

bool StAccel_dsh::IsFifoEmpty(bool & Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegFifoSrc, out);

    Value = ((out & 0x20) == 0x20);
    return (code == Device::OK);
}
bool StAccel_dsh::IsFifoOverrun(bool & Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegFifoSrc, out);

    Value = ((out & 0x40) == 0x40);
    return (code == Device::OK);
}

bool StAccel_dsh::IsIntDrdy(bool & Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegCtrlReg3, out);

    Value = ((out & 0x80) == 0x80);
    return (code == Device::OK);
}
bool StAccel_dsh::UseIntDrdy(bool Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegCtrlReg3,out);
    if ( code != Device::OK)
        return false;

    if(Value)
        out |= 0x80;
    else
        out &= 0x7F;

    code = Write(m_RegCtrlReg3, out);
    return (code == Device::OK);
}

bool StAccel_dsh::GetIntPolarity(LogicState & Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegCtrlReg3, out);

    Value = (((out & 0x40) == 0x40) ? High : Low);
    return (code == Device::OK);
}
bool StAccel_dsh::SetIntPolarity(LogicState Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegCtrlReg3,out);
    if ( code != Device::OK)
        return false;

    if(Value == High)
        out |= 0x40;
    else if(Value == Low)
        out &= 0xBF;
    else
        return false;

    code = Write(m_RegCtrlReg3, out);
    return (code == Device::OK);
}

bool StAccel_dsh::GetIntType(InterruptSignalType & Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegCtrlReg3, out);

    Value = (((out & 0x20) == 0x20) ? Pulsed : Latched);
    return (code == Device::OK);
}
bool StAccel_dsh::SetIntType(InterruptSignalType Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegCtrlReg3, out);
    if ( code != Device::OK)
        return false;

    if(Value == Pulsed)
        out |= 0x20;
    else if(Value == Latched)
        out &= 0xDF;
    else
        return false;

    code = Write(m_RegCtrlReg3, out);
    return (code == Device::OK);
}

bool StAccel_dsh::IsInterrupt1(bool & Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegCtrlReg3, out);

    Value = ((out & 0x10) == 0x10);
    return (code == Device::OK);
}
bool StAccel_dsh::UseInterrupt1(bool Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegCtrlReg3,out);
    if ( code != Device::OK)
        return false;

    if(Value)
        out |= 0x10;
    else
        out &= 0xEF;

    code = Write(m_RegCtrlReg3, out);
    return (code == Device::OK);
}

bool StAccel_dsh::IsInterrupt2(bool & Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegCtrlReg3, out);

    Value = ((out & 0x08) == 0x08);
    return (code == Device::OK);
}
bool StAccel_dsh::UseInterrupt2(bool Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegCtrlReg3,out);
    if ( code != Device::OK)
        return false;

    if(Value)
        out |= 0x08;
    else
        out &= 0xF7;

    code = Write(m_RegCtrlReg3, out);
    return (code == Device::OK);
}

bool StAccel_dsh::IsIntBoot(bool & Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegCtrlReg6, out);

    Value = ((out & 0x01) == 0x01);
    return (code == Device::OK);
}
bool StAccel_dsh::UseIntBoot(bool Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegCtrlReg6,out);
    if ( code != Device::OK)
        return false;

    if(Value)
        out |= 0x01;
    else
        out &= 0xFE;

    code = Write(m_RegCtrlReg6, out);
    return (code == Device::OK);
}
bool StAccel_dsh::IsIntFifoEmpty(bool & Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegCtrlReg6, out);

    Value = ((out & 0x08) == 0x08);
    return (code == Device::OK);
}
bool StAccel_dsh::UseIntFifoEmpty(bool Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegCtrlReg6,out);
    if ( code != Device::OK)
        return false;

    if(Value)
        out |= 0x08;
    else
        out &= 0xF7;

    code = Write(m_RegCtrlReg6, out);
    return (code == Device::OK);
}
bool StAccel_dsh::IsIntFifoWtmrk(bool & Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegCtrlReg6, out);

    Value = ((out & 0x04) == 0x04);
    return (code == Device::OK);
}
bool StAccel_dsh::UseIntFifoWtmrk(bool Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegCtrlReg6,out);
    if ( code != Device::OK)
        return false;

    if(Value)
        out |= 0x04;
    else
        out &= 0xFB;

    code = Write(m_RegCtrlReg6, out);
    return (code == Device::OK);
}
bool StAccel_dsh::IsIntFifoOverrun(bool & Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegCtrlReg6, out);

    Value = ((out & 0x02) == 0x02);
    return (code == Device::OK);
}
bool StAccel_dsh::UseIntFifoOverrun(bool Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegCtrlReg6,out);
    if ( code != Device::OK)
        return false;

    if(Value)
        out |= 0x02;
    else
        out &= 0xFD;

    code = Write(m_RegCtrlReg6, out);
    return (code == Device::OK);
}

bool StAccel_dsh::GetOffset(Sensor::RawAcceleromterData & Value)
{
    Word code = Device::OK;
    Word out[3]={0,0,0};

    if(m_bReadInc){
        code = MultiRead(m_RegOffX, 3, out);
    }else{
        code |= Read(m_RegOffX, out[0]);
        code |= Read(m_RegOffY, out[1]);
        code |= Read(m_RegOffZ, out[2]);
    }

    Value.nX_Data = out[0];
    Value.nY_Data = out[1];
    Value.nZ_Data = out[2];
    return (code == Device::OK);
}
bool StAccel_dsh::SetOffset(Sensor::RawAcceleromterData Value)
{
    Word code = Device::OK;
    Word out[3] = {0,0,0};
    out[0] = (Word)Value.nX_Data;
    out[1] = (Word)Value.nY_Data;
    out[2] = (Word)Value.nZ_Data;

    if(m_bReadInc){
        code = MultiWrite(m_RegOffX, 3, out);
    }else{
        code |= Write(m_RegOffX, out[0]);
        code |= Read(m_RegOffY, out[1]);
        code |= Read(m_RegOffZ, out[2]);
    }

    return (code == Device::OK);
}

bool StAccel_dsh::GetFifoMode(FifoMode & Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegFifoCtrl, out);

    Value = (FifoMode)((out & 0xE0) >> 5);
    return (code == Device::OK);
}
bool StAccel_dsh::SetFifoMode(FifoMode Value)
{
    Word out;
    if(Value > MODE_BYPASS_THEN_FIFO)
        return false;

    Device::eReturnCode code = Read(m_RegFifoCtrl,out);
    if ( code != Device::OK)
        return false;

    out = (out & 0x1F) | (0xE0 & ((Word)Value << 5));
    code = Write(m_RegFifoCtrl, out);
    return (code == Device::OK);
}

bool StAccel_dsh::IsSPI3WireMode(bool & Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegCtrlReg5, out);

    Value = ((out & 0x01) == 0x01);
    return (code == Device::OK);
}
bool StAccel_dsh::UseSPI3WireMode(bool Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegCtrlReg5,out);
    if ( code != Device::OK)
        return false;

    if(Value)
        out |= 0x01;
    else
        out &= 0xFE;

    code = Write(m_RegCtrlReg5, out);
    return (code == Device::OK);
}

bool StAccel_dsh::GetAntiAliasFilterBandwidth(AntiAliasingBandwidth & Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegCtrlReg5, out);

    Value = (AntiAliasingBandwidth)((out & 0xC0) >> 6);
    return (code == Device::OK);
}
bool StAccel_dsh::SetAntiAliasFilterBandwidth(AntiAliasingBandwidth Value)
{
    Word out;
    if(Value > BW_3)
        return false;

    Device::eReturnCode code = Read(m_RegCtrlReg5,out);
    if ( code != Device::OK)
        return false;

    out = (out & 0x3F) | (0xC0 & ((Word)Value << 6));
    code = Write(m_RegCtrlReg5, out);
    return (code == Device::OK);
}

bool StAccel_dsh::IsFifoWatermark(bool & Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegCtrlReg6,out);

    Value = ((out & 0x20) == 0x20);
    return (code == Device::OK);
}
bool StAccel_dsh::UseFifoWatermark(bool Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegCtrlReg6,out);
    if ( code != Device::OK)
        return false;

    if(Value)
        out |= 0x20;
    else
        out &= 0xDF;

    code = Write(m_RegCtrlReg6, out);
    return (code == Device::OK);
}
bool StAccel_dsh::GetFifoFilledLength(Word & Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegFifoSrc, out);

    Value = (out & 0x1F);
    return (code == Device::OK);
}
bool StAccel_dsh::GetFifoWatermarkStatus(bool & Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegFifoSrc, out);

    Value = ((out & 0x80) == 0x80);
    return (code == Device::OK);
}
bool StAccel_dsh::GetFifoWatermarkPointer(Word & Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegFifoCtrl, out);

    Value = (out & 0x1F);
    return (code == Device::OK);
}
bool StAccel_dsh::SetFifoWatermarkPointer(Word Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegFifoCtrl,out);
    if ( code != Device::OK)
        return false;

    out = (out & 0xE0) | (0x1F & Value);
    code = Write(m_RegFifoCtrl, out);
    return (code == Device::OK);
}

bool StAccel_dsh::GetSelfTestMode(SelfTestMode & Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegCtrlReg5, out);

    Value = (SelfTestMode)((out & 0x06) >> 1);
    return (code == Device::OK);
}
bool StAccel_dsh::SetSelfTestMode(SelfTestMode Value)
{
    Word out;
    if(Value > TEST_NEGATIVE)
        return false;

    Device::eReturnCode code = Read(m_RegCtrlReg5,out);
    if ( code != Device::OK)
        return false;

    out = (out & 0xF9) | (0x06 & ((Word)Value << 1));
    code = Write(m_RegCtrlReg5, out);
    return (code == Device::OK);
}

bool StAccel_dsh::DataOverrunXYZ(bool & Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegStatus,out);

    Value = ((out & 0x80) == 0x80);
    return (code == Device::OK);
}
bool StAccel_dsh::DataOverrunX(bool & Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegStatus,out);

    Value = ((out & 0x10) == 0x10);
    return (code == Device::OK);
}
bool StAccel_dsh::DataOverrunY(bool & Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegStatus,out);

    Value = ((out & 0x20) == 0x20);
    return (code == Device::OK);
}
bool StAccel_dsh::DataOverrunZ(bool & Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegStatus,out);

    Value = ((out & 0x40) == 0x40);
    return (code == Device::OK);
}

bool StAccel_dsh::DataAvailableXYZ(bool & Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegStatus,out);

    Value = ((out & 0x08) == 0x08);
    return (code == Device::OK);
}
bool StAccel_dsh::DataAvailableX(bool & Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegStatus,out);

    Value = ((out & 0x01) == 0x01);
    return (code == Device::OK);
}
bool StAccel_dsh::DataAvailableY(bool & Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegStatus,out);

    Value = ((out & 0x02) == 0x02);
    return (code == Device::OK);
}
bool StAccel_dsh::DataAvailableZ(bool & Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegStatus,out);

    Value = ((out & 0x04) == 0x04);
    return (code == Device::OK);
}

bool StAccel_dsh::IsXAxisEnabled(bool & Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegCtrlReg4,out);

    Value = ((out & 0x04) == 0x04);
    return (code == Device::OK);
}
bool StAccel_dsh::IsYAxisEnabled(bool & Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegCtrlReg4,out);

    Value = ((out & 0x02) == 0x02);
    return (code == Device::OK);
}
bool StAccel_dsh::IsZAxisEnabled(bool & Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegCtrlReg4,out);

    Value = ((out & 0x01) == 0x01);
    return (code == Device::OK);
}

bool StAccel_dsh::UseAxisX(bool Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegCtrlReg4,out);
    if ( code != Device::OK)
        return false;

    if(Value)
        out |= 0x04;
    else
        out &= 0xFB;

    code = Write(m_RegCtrlReg4, out);
    return (code == Device::OK);
}
bool StAccel_dsh::UseAxisY(bool Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegCtrlReg4,out);
    if ( code != Device::OK)
        return false;

    if(Value)
        out |= 0x02;
    else
        out &= 0xFD;

    code = Write(m_RegCtrlReg4, out);
    return (code == Device::OK);
}
bool StAccel_dsh::UseAxisZ(bool Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegCtrlReg4,out);
    if ( code != Device::OK)
        return false;

    if(Value)
        out |= 0x01;
    else
        out &= 0xFE;

    code = Write(m_RegCtrlReg4, out);
    return (code == Device::OK);
}

bool StAccel_dsh::IsBDU(bool & Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegCtrlReg4,out);

    Value = ((out & 0x08) == 0x08);
    return (code == Device::OK);
}
bool StAccel_dsh::UseBDU(bool Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegCtrlReg4,out);
    if ( code != Device::OK)
        return false;

    if(Value)
        out |= 0x08;
    else
        out &= 0xF7;

    code = Write(m_RegCtrlReg4, out);
    return (code == Device::OK);
}

bool StAccel_dsh::IsReadInc(bool & Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegCtrlReg6,out);

    Value = ((out & 0x10) == 0x10);
    m_bReadInc = Value;
    return (code == Device::OK);
}
bool StAccel_dsh::UseReadInc(bool Value)
{
    Word out;
    Device::eReturnCode code = Read(m_RegCtrlReg6,out);
    if ( code != Device::OK)
        return false;

    if(Value)
        out |= 0x10;
    else
        out &= 0xEF;

    code = Write(m_RegCtrlReg6, out);
    m_bReadInc = Value;
    return (code == Device::OK);
}

bool StAccel_dsh::ReadSensorDataOnce(Sensor::RawThermometerData & OutData)
{
    Word value;
    Device::eReturnCode code = Read(m_RegOutT, value);

    OutData.nTemp = value;
    return (code == Device::OK);
}
bool StAccel_dsh::ReadSensorDataOnce(Sensor::RawAcceleromterData & OutData)
{
    Word out[6]= {0,0,0,0,0,0};
    Word code = Device::OK;

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

    OutData.nX_Data = *(DoubleWord*)(out + 0) << 16;
    OutData.nY_Data = *(DoubleWord*)(out + 2) << 16;
    OutData.nZ_Data = *(DoubleWord*)(out + 4) << 16;

    return (code == Device::OK);
}

Celcius StAccel_dsh::ConvertToSIUnit(Sensor::RawThermometerData Data)
{
    WordSigned tmp = (WordSigned)Data.nTemp;
    return (float)(25 + tmp);
}
GForce * StAccel_dsh::ConvertToSIUnit(Sensor::RawAcceleromterData Data)
{
    GForce * out= new GForce[3];
    float range = _GetSIRange(m_eRange);

    out[0] = ((float)Data.nX_Data / (float)0x80000000) * range;
    out[1] = ((float)Data.nY_Data / (float)0x80000000) * range;
    out[2] = ((float)Data.nZ_Data / (float)0x80000000) * range;
    return out;
}

}
