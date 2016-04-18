/// \file   icdevice.h
/// \author Elia Carrara
/// \date   April, 2016
/// \brief  Contains the definition of the IC device class.
/// \details This is a representation of a IC device. I implemented this class
/// with readable and/or writable registers. The registers are represented
/// by the sRegister struct.

#ifndef ICDEVICE_H
#define ICDEVICE_H

#include <bus.h>
#include <types.h>

#define READ_ACCESS 0x1
#define WRITE_ACCESS 0x2
#define NO_ACCESS 0x0
#define FULL_ACCESS (READ_ACCESS | WRITE_ACCESS)

/// Encloses all devices related to the skywalker
namespace Device {

struct sRegister{
    Word cAccess;
    Word cAddress;
    Word cDefault;
    Word cLockedBits;
};

inline sRegister InitRegister(Word AccessLevel, Word Address, Word Default, Word LockedBits){
    sRegister reg;
    reg.cAccess = AccessLevel;
    reg.cAddress = Address;
    reg.cDefault = Default;
    reg.cLockedBits = LockedBits;
    return reg;
}

enum eReturnCode{
    OK                  = 0,
    NoAccess            = 1,
    WritingLockedBits   = 2,
    InvalidRegister     = 4,
    BufferError         = 8,
    MultiAccessDisabled = 16,
    TerribleError       = 32,
};

typedef sRegister * RegPtr;

class ICDevice{
    sRegister * m_psAllRegisters = 0;
    Size m_unLenRegisters;
    Size m_unCounter;
protected:
    ICDevice();
    ~ICDevice();

    Communication::Bus * m_clsBus = 0;
    RegPtr AddRegister(sRegister Reg);

    virtual eReturnCode Read(RegPtr psReg, Word & cValue)=0;
    virtual eReturnCode Write(RegPtr psReg, Word & cValue)=0;

    bool CheckRegisterAccess(RegPtr psReg, Word ActionCode);
    bool CheckLockedBits(RegPtr psReg, Word & tx_out);

    void InitIC(Size NumberOfRegs, Communication::Bus * clsBus);

};

}
#endif
