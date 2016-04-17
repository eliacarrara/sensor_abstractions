#include <st_magno_mdl.h>
#include <spibus.h>
#include <error.h>

namespace Sensor{

StMagno_mdl::StMagno_mdl()
{
    Communication::SpiBus * clsBus = new Communication::SpiBus(MAG_SPI_DEV_FILE);

    if (!clsBus->Open())
        throw SPI_FILE_OPEN_ERROR;

    Communication::spi_mode mode = SPI_MODE_3;
    Hertz speed = MAG_SPI_SPEED;

    if (!clsBus->SetMode(mode))
        throw SPI_PARAM_ERROR;
    if (!clsBus->SetSpeed(speed))
        throw SPI_PARAM_ERROR;
    if(mode != SPI_MODE_3 || speed != MAG_SPI_SPEED)
        throw SPI_PARAM_ERROR;

    InitIC(MAG_NR_OF_REGISTERS,clsBus);
}
StMagno_mdl::~StMagno_mdl()
{

}

Device::eReturnCode StMagno_mdl::MultiRead(Device::RegPtr psReg, unsigned int BytesToRead, char * pcRxData)
{

}
Device::eReturnCode StMagno_mdl::MultiWrite(Device::RegPtr psReg, unsigned int BytesToRead, char * pcTxData)
{

}
Device::eReturnCode StMagno_mdl::Read(Device::RegPtr psReg, char & cValue)
{

}
Device::eReturnCode StMagno_mdl::Write(Device::RegPtr psReg, char & cValue)
{

}

bool StMagno_mdl::Reboot()
{

}
bool StMagno_mdl::SoftReset()
{

}

bool StMagno_mdl::WhoAmI(char & Value)
{

}

bool StMagno_mdl::IsEnabledTemp(bool & Value)
{

}
bool StMagno_mdl::UseTemp(bool Value)
{

}

bool StMagno_mdl::GetPowerModeXY(StMagno_mdl::PowerMode & Value)
{

}
bool StMagno_mdl::SetPowerModeXY(StMagno_mdl::PowerMode Value)
{

}

bool StMagno_mdl::GetPowerModeZ(StMagno_mdl::PowerMode & Value)
{

}
bool StMagno_mdl::SetPowerModeZ(StMagno_mdl::PowerMode Value)
{

}

bool StMagno_mdl::GetOperatingMode(StMagno_mdl::OperatingMode & Value)
{

}
bool StMagno_mdl::SetOperatingMode(StMagno_mdl::OperatingMode Value)
{

}

bool StMagno_mdl::GetODR(StMagno_mdl::ODR & Value)
{

}
bool StMagno_mdl::SetODR(StMagno_mdl::ODR Value)
{

}

bool StMagno_mdl::GetRange(StMagno_mdl::ODR & Value)
{

}
bool StMagno_mdl::SetRange(StMagno_mdl::ODR Value)
{

}

bool StMagno_mdl::IsEnabledSelfTest(bool & Value)
{

}
bool StMagno_mdl::UseSelfTest(bool Value)
{

}

bool StMagno_mdl::IsModeLowPower(bool & Value)
{

}
bool StMagno_mdl::UseModeLowPower(bool Value)
{

}

bool StMagno_mdl::IsFastReading(bool & Value)
{

}
bool StMagno_mdl::UseFastReading(bool Value)
{

}

bool StMagno_mdl::IsBDU(bool & Value)
{

}
bool StMagno_mdl::UseBDU(bool Value)
{

}

bool StMagno_mdl::IsSPI3WireMode(bool & Value)
{

}
bool StMagno_mdl::UseSPI3WireMode(bool Value)
{

}

bool StMagno_mdl::GetEndianess(Endianess & Value)
{

}
bool StMagno_mdl::SetEndianess(Endianess Value)
{

}

bool StMagno_mdl::DataOverrunXYZ(bool & Value)
{

}
bool StMagno_mdl::DataOverrunX(bool & Value)
{

}
bool StMagno_mdl::DataOverrunY(bool & Value)
{

}
bool StMagno_mdl::DataOverrunZ(bool & Value)
{

}

bool StMagno_mdl::DataAvailableXYZ(bool & Value)
{

}
bool StMagno_mdl::DataAvailableX(bool & Value)
{

}
bool StMagno_mdl::DataAvailableY(bool & Value)
{

}
bool StMagno_mdl::DataAvailableZ(bool & Value)
{

}

bool StMagno_mdl::IsInt(bool & Value)
{

}
bool StMagno_mdl::UseInt(bool Value)
{

}

bool StMagno_mdl::IsIntX(bool & Value)
{

}
bool StMagno_mdl::UseIntX(bool Value)
{

}

bool StMagno_mdl::IsIntY(bool & Value)
{

}
bool StMagno_mdl::UseIntY(bool Value)
{

}

bool StMagno_mdl::IsIntZ(bool & Value)
{

}
bool StMagno_mdl::UseIntZ(bool Value)
{

}

bool StMagno_mdl::GetIntPolarity(LogicState & Value)
{

}
bool StMagno_mdl::SetIntPolarity(LogicState Value)
{

}

bool StMagno_mdl::GetIntType(InterruptSignalType & Value)
{

}
bool StMagno_mdl::SetIntType(InterruptSignalType Value)
{

}

bool StMagno_mdl::IsIntThsPosX(bool & Value)
{

}
bool StMagno_mdl::IsIntThsNegX(bool & Value)
{

}

bool StMagno_mdl::IsIntThsPosY(bool & Value)
{

}
bool StMagno_mdl::IsIntThsNegY(bool & Value)
{

}

bool StMagno_mdl::IsIntThsPosZ(bool & Value)
{

}
bool StMagno_mdl::IsIntThsNegXZ(bool & Value)
{

}

bool StMagno_mdl::IsIntMeasureOverflow(bool & Value)
{

}
bool StMagno_mdl::IsIntSig(bool & Value)
{

}

bool StMagno_mdl::ReadSensorDataOnce(RawMagnetometerData & OutData)
{

}
Gauss StMagno_mdl::ConvertToSIUnit(RawMagnetometerData Data)
{

}

bool StMagno_mdl::ReadSensorDataOnce(RawThermometerData & OutData)
{

}
Celcius StMagno_mdl::ConvertToSIUnit(RawThermometerData Data)
{

}



}
