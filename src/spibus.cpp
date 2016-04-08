#include "spibus.h"

SpiBus::SpiBus(const char * strFile){
    m_nFd = -1;
    m_strDevFile = strFile;
    m_sSpiMsg.cs_change = 0;
    m_sSpiMsg.bits_per_word = 0;
    m_sSpiMsg.delay_usecs = 0;
    m_sSpiMsg.pad = 0;
    m_sSpiMsg.len = 0;
}

SpiBus::~SpiBus(){
    m_strDevFile = "";
}

bool SpiBus::IsOpen(){
    return (m_nFd > -1);
}

bool SpiBus::Open(){
    m_nFd = open(m_strDevFile, O_RDWR);
    return IsOpen();
}

bool SpiBus::Close(){
    if(IsOpen()){
        close(m_nFd);
        m_nFd = -1;
        return true;
    }else
        return false;
}

bool SpiBus::Transact(const char *pcReadBuffer, const char *pcWriteBuffer, unsigned int nLength)
{
    if(pcWriteBuffer == NULL || pcReadBuffer == NULL)
        return false;

    m_sSpiMsg.tx_buf = (__u64)pcWriteBuffer;
    m_sSpiMsg.rx_buf = (__u64)pcReadBuffer;
    m_sSpiMsg.len = (__u32)nLength;

    return (ioctl(m_nFd,SPI_IOC_MESSAGE(1), &m_sSpiMsg) >= 0);
}

bool SpiBus::GetMode(spi_mode &ucMode)
{
    if (!IsOpen())
        return false;
    return (ioctl(m_nFd, SPI_IOC_RD_MODE, &ucMode) >= 0);
}

bool SpiBus::GetBitsPerWord(bitsperword &ucBPW)
{
    if (!IsOpen())
        return false;
    return (ioctl(m_nFd, SPI_IOC_RD_BITS_PER_WORD, &ucBPW) >= 0);
}

bool SpiBus::GetSpeed(hertz &unSpeed)
{
    if (!IsOpen())
        return false;
    return (ioctl(m_nFd, SPI_IOC_RD_MAX_SPEED_HZ, &unSpeed) >= 0);
}

bool SpiBus::GetDelay(microseconds &unDelay)
{
    if (!IsOpen())
        return false;
    unDelay = m_sSpiMsg.delay_usecs;
    return true;
}

bool SpiBus::GetEndianness(endianess &eEndianess)
{
    if (!IsOpen())
        return false;

    char endian;
    bool rtrn = ioctl(m_nFd, SPI_IOC_RD_LSB_FIRST, &endian) >= 0;
    eEndianess = (endianess)endian;
    return rtrn;
}

bool SpiBus::SetMode(spi_mode &ucMode)
{
    if ((!IsOpen() ) | (ucMode < 0 && ucMode > 3) ){
        return false;
    }

    return (ioctl(m_nFd, SPI_IOC_WR_MODE, &ucMode) >= 0);
}

bool SpiBus::SetBitsPerWord(bitsperword &ucBPW)
{
    if (!IsOpen())
        return false;

    return (ioctl(m_nFd, SPI_IOC_WR_BITS_PER_WORD, &ucBPW) >= 0);
}

bool SpiBus::SetSpeed(hertz &unSpeed)
{
    if (!IsOpen())
        return false;

    return (ioctl(m_nFd, SPI_IOC_WR_MAX_SPEED_HZ, &unSpeed) >= 0);
}

bool SpiBus::SetDelay(microseconds &unDelay)
{
    if (!IsOpen())
        return false;
    m_sSpiMsg.delay_usecs = unDelay;
    return true;
}

bool SpiBus::SetEndianness(endianess &eEndianess)
{
    if (!IsOpen()){
        return false;
    }
    return (ioctl(m_nFd, SPI_IOC_WR_LSB_FIRST, &eEndianess) >= 0);
}

