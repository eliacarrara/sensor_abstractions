#include "icdevice.h"

namespace Device{

RegPtr ICDevice::AddRegister(sRegister Reg)
{
    m_psAllRegisters[m_unCounter] = Reg;
    return &(m_psAllRegisters[m_unCounter++]);
}

void ICDevice::InitIC(Size NumberOfRegs, Communication::Bus * clsBus)
{
    m_psAllRegisters = new sRegister[NumberOfRegs];
    m_unLenRegisters = NumberOfRegs;
    m_clsBus = clsBus;
}

bool ICDevice::CheckRegisterAccess(RegPtr psReg, Word ActionCode)
{
    if(ActionCode == READ_ACCESS)
        return (psReg->cAccess & READ_ACCESS);
    else if (ActionCode == WRITE_ACCESS)
        return (psReg->cAccess & WRITE_ACCESS);
    else
        return false;
}

bool ICDevice::CheckLockedBits(RegPtr psReg, Word & tx_out)
{
    return (((psReg->cDefault & psReg->cLockedBits) | (~psReg->cLockedBits & tx_out)) == tx_out);
}

ICDevice::ICDevice()
{
    m_unCounter = 0;
}

ICDevice::~ICDevice()
{
    delete[] m_psAllRegisters;
}

}
