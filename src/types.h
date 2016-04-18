/// \file   types.h
/// \author Elia Carrara
/// \date   April, 2016
/// \brief  Contains typedefs with names for commonly used units.
/// \details These representations shoud make it easier to work with.
/// Rather than getting a "float" from a function, it is way
/// more simpler to know what for a unit you are dealing with.
///
/// \var typedef Celcius
/// \brief Represensation for a Celcius unit.
///
/// \var typedef Gauss
/// \brief Represensation for a Gauss unit.
///
/// \var typedef DegreePerSecond
/// \brief Represensation for a degree per second unit.
///
/// \var typedef GForce
/// \brief Represensation for a g (force) unit.
///
/// \var typedef Hertz
/// \brief Representation for a hertz unit;
///
/// \var typedef MicroSeconds
/// \brief Representation for a microsecond unit;
///
/// \var typedef Size
/// \brief Representation for a size value;
///
/// \var typedef BitsPerWord
/// \brief Representation for bits per word.
///
/// \var typedef LogicState
/// \brief Representation for a digital signal state;
///
/// \var typedef InterruptSignalType
/// \brief Representation for an interrupt signal type;
///
/// \var typedef Endianess
/// \brief Representation for the endianess.
///
/// \var typedef Word
/// \brief Unsigned 8-bits.
///
/// \var typedef WordSigned
/// \brief Signed 8-bits.
///
/// \var typedef DoubleWord
/// \brief Unsigned 16-bits.
///
/// \var typedef DoubleWordSigned
/// \brief Signed 16-bits.
///
/// \var typedef QuadWord
/// \brief Unsigned 32-bits.
///
/// \var typedef QuadWordSigned
/// \brief Signed 32-bits.
///
/// \var typedef OctoWord
/// \brief Unsigned 64-bits.
///
/// \var typedef OctoWordSigned
/// \brief Signed 64-bits.


#ifndef TYPES_H
#define TYPES_H

// SI Units
typedef float Celcius;
typedef float Gauss;
typedef float DegreePerSecond;
typedef float GForce;
typedef unsigned int Hertz;
typedef unsigned int MicroSeconds;

// Words
// These typedefs make it easier to port the application to a plattform
// with a differend architecture.
typedef unsigned char       Word;               // 8 Bits
typedef signed char         WordSigned;         // 8 Bits
typedef unsigned short      DoubleWord;         // 16 Bits
typedef signed short        DoubleWordSigned;   // 16 Bits
typedef unsigned int        QuadWord;           // 32 Bits
typedef signed int          QuadWordSigned;     // 32 Bits
typedef unsigned long long  OctoWord;           // 64 Bits
typedef signed long long    OctoWordSigned;     // 64 Bits


typedef unsigned int Size;
typedef unsigned char BitsPerWord;
typedef enum{Low = 0, High = 1} LogicState;
typedef enum{Latched = 0, Pulsed = 1} InterruptSignalType;
typedef enum{MSB = 0, LSB = 1} Endianess;

#endif
