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
