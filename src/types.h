/// \file   types.h
/// \author Elia Carrara
/// \date   April, 2016
/// \brief  Contains typedefs with names for commonly used units.
/// \details These representations shoud make it easier to work with.
/// Rather than getting a "float" from a function, it is way
/// more simpler to know what for a unit you are dealing with.

#ifndef TYPES_H
#define TYPES_H

/// \brief Represensation for a Celcius unit.
typedef float Celcius;
/// \brief Represensation for a Gauss unit.
typedef float Gauss;
/// \brief Represensation for a degree per second unit.
typedef float DPS;
/// \brief Represensation for a g (force) unit.
typedef float GForce;

/// \brief Representation for a hertz unit;
typedef unsigned int hertz;
/// \brief Representation for a microsecond unit;
typedef unsigned int microseconds;
/// \brief Representation for bits per word.
typedef unsigned char bitsperword;
/// \brief Representation for the endianess.
typedef enum{MSB = 0, LSB = 1} endianess;

#endif
