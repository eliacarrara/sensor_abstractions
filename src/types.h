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
/// \var typedef hertz
/// \brief Representation for a hertz unit;
///
/// \var typedef microseconds
/// \brief Representation for a microsecond unit;
///
/// \var typedef bitsperword
/// \brief Representation for bits per word.
///
/// \var typedef endianess
/// \brief Representation for the endianess.

#ifndef TYPES_H
#define TYPES_H

typedef float Celcius;
typedef float Gauss;
typedef float DegreePerSecond;
typedef float GForce;

typedef unsigned int hertz;
typedef unsigned int microseconds;
typedef unsigned char bitsperword;
typedef enum{MSB = 0, LSB = 1} endianess;

#endif
