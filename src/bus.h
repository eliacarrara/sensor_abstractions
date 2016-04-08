/// \file   bus.h
/// \author Elia Carrara
/// \date   April, 2016
/// \brief  Contains the definition of the bus base class.

#ifndef BUS_H
#define BUS_H

class Bus{
public:
	virtual bool IsOpen()=0;
	virtual bool Open()=0;
	virtual bool Close()=0;
    virtual bool Transact(const char * pcReadBuffer, const char * pcWriteBuffer, unsigned int unLength)=0;
};

#endif
