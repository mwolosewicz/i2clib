//
// Created by mw on 18.12.2020.
//

#ifndef I2CLIB_I2C_H
#define I2CLIB_I2C_H

#include <string>
#include <vector>

/**
 * Object of this class acts as interface for I2C device.
 */
class I2C {
public:

    /**
     * Constructor.
     *
     * @param i2cDevPath is path to device in /dev directory.
     * @param devAddr address of device connected to I2C bus
     */
    I2C(std::string i2cDevPath, std::byte devAddr);

    /**
     * Destructor
     */
    virtual ~I2C();

    /**
     * Read data from I2C device.
     *
     * @param addr address of register to read
     * @param size is amount of data we want to read
     * @return Data read from device.
     */
    std::vector<std::byte> read(const std::byte &addr, unsigned int size);

    /**
     * Write data to I2C device.
     *
     * @param addr address of register to write
     * @param data data array which will be written to device
     */
    void write(const std::byte &addr, const std::vector<std::byte> &data);

private:
    std::string mI2cDevPath;
    std::byte mDevAddr;
    int mI2cFileDescriptor = 0;

    std::vector<std::byte> readRaw(unsigned int size);
    void writeRaw(const std::vector<std::byte> data);
};


#endif //I2CLIB_I2C_H
