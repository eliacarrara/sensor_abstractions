#include <st_sensor.h>
#include <spibus.h>


namespace Sensor{

StSensorConfig::~StSensorConfig()
{
    if(m_pcRxBuf != NULL)
        delete m_pcRxBuf;
    if(m_pcTxBuf != NULL)
        delete m_pcTxBuf;
}

Size StSensorConfig::_BufferSize()
{
    return m_unBuffSize;
}
void StSensorConfig::_BufferSize(Size unSize)
{
    m_unBuffSize = unSize;
}
void StSensorConfig::_SetupBuffers(Size BufLen)
{
    m_pcRxBuf = new Word[BufLen];
    m_pcTxBuf = new Word[BufLen];
    _BufferSize(BufLen);
}
void StSensorConfig::_PopulateBuffer(Word accessor)
{
    m_pcTxBuf[0] = accessor;
}
void StSensorConfig::_CleanBuffers()
{
    _BufferSize(0);
    delete[] m_pcRxBuf;
    delete[] m_pcTxBuf;
}
bool StSensorConfig::_BombsAway()
{
    return ((Communication::SpiBus*)m_clsBus)->Transact(m_pcRxBuf,m_pcTxBuf, m_unBuffSize);
}

}
