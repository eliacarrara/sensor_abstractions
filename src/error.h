/// \file   error.h
/// \author Elia Carrara
/// \date   April, 2016
/// \brief  Contains error codes.
/// \details An error code is always made up of a
/// category and a specific error number. When adding new error code,
/// use a error category and "or" it with a specific error number (e.g BUS_LVL | 0x1).
/// If you add a new error category make sure its value is greater 0xFF because
/// those values are reserved for the specific error numbers. The opposite applies
/// to the specific error codes. Keep them under 0x100. 256 error values per category should be enough...
///
/// \def BUS_LVL
/// \brief Bus-Error Category.
/// \details This error category is used on all bus related errors.
/// \def SENSOR_LVL
/// \brief Sensor-Error Category.
/// \details This error category is used on all sensor related errors.
/// \def IC_DEV_LVL
/// \brief IC Device-Error Category.
/// \details This error category is used on all IC device related errors.
///
///
/// \def SPI_FILE_OPEN_ERROR
/// \brief An error occured while opening the SPI file.
/// \details This error code is used when an SPI device file could not be opened.
/// \def SPI_PARAM_ERROR
/// \brief An error occured while configuring the spi-bus.
/// \details This error code is used when the spi-bus cannot be configured.
/// \def REG_READ_ERROR
/// \brief An error occured while reading a IC device register.
/// \details This error code is used when accessing a register of an IC device object failed.

#ifndef ERROR_H
#define ERROR_H

// Error Categories
#define BUS_LVL 0x100
#define SENSOR_LVL 0x200
#define IC_DEV_LVL 0x300

// Specific Errors
#define SPI_FILE_OPEN_ERROR (BUS_LVL | 0x1)
#define SPI_PARAM_ERROR (BUS_LVL | 0x2)
#define REG_READ_ERROR (IC_DEV_LVL | 0x1)

#endif
