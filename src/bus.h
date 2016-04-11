/// \file   bus.h
/// \author Elia Carrara
/// \date   April, 2016
/// \brief  Contains the definition of the bus base class.
/// \details This class represents the base of any kind of communication type.

#ifndef BUS_H
#define BUS_H

/// Includes all communication related items
namespace Communication {

/// \brief The Bus class represents the base class for any type of communication.
/// \details This class includes functions to open, close a communication method
/// and transfer data. The Transact() method was implemented to support full duplex.
class Bus{
public:
    /// \brief IsOpen should be implemented to checks if the bus is open.
    /// \return Returns true if the communication bus is open.
	virtual bool IsOpen()=0;
    /// \brief Open should be implemented to open and initialize the communication method.
    /// \return Returns true upon success.
	virtual bool Open()=0;
    /// \brief Close should be implemented to close the communication method.
    /// \return Returns true upon success.
	virtual bool Close()=0;
    /// \brief Transact should be implemented to read and/or write data.
    /// \details Implement Bus in a subclass and define a Read method
    /// and write callback to use network communication methods.
    /// \param pcReadBuffer Should be a pointer to a buffer with unLength bytes to fill
    /// the read data in.
    /// \param pcWriteBuffer Shoudl be a pointer to a buffer with unLength bytes of
    /// data to write.
    /// \param unLength The number of bytes of data to transfer.
    /// \return Returns true upon success.
    virtual bool Transact(const char * pcReadBuffer, const char * pcWriteBuffer, unsigned int unLength)=0;
};

}
#endif
