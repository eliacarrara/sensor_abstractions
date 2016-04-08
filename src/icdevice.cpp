#include "icdevice.h"
#include <malloc.h>

RegPtr ICDevice::AddRegister(sRegister Reg)
{
    m_psAllRegisters[m_unCounter] = Reg;
    return &(m_psAllRegisters[m_unCounter++]);
}

void ICDevice::InitIC(unsigned int NumberOfRegs, Bus * clsBus)
{
    m_psAllRegisters = (sRegister*)malloc(NumberOfRegs * sizeof(sRegister));
    m_unLenRegisters = NumberOfRegs;
    m_clsBus = clsBus;
}

bool ICDevice::CheckRegisterAccess(RegPtr psReg, char ActionCode)
{
    if(ActionCode == READ_ACCESS)
        return (psReg->cAccess & READ_ACCESS);
    else if (ActionCode == WRITE_ACCESS)
        return (psReg->cAccess & WRITE_ACCESS);
    else
        return false;
}

bool ICDevice::CheckLockedBits(RegPtr psReg, char & tx_out)
{
    return (((psReg->cDefault & psReg->cLockedBits) | (~psReg->cLockedBits & tx_out)) == tx_out);
}

ICDevice::ICDevice()
{
    m_unCounter = 0;
}

ICDevice::~ICDevice()
{
    free(m_psAllRegisters);
}
