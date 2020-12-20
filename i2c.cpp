//
// Created by mw on 18.12.2020.
//

#include "i2c.h"

#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <utility>
#include <system_error>
#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>
#include <cstddef>


I2C::I2C(std::string i2cDevPath, std::byte devAddr) : mI2cDevPath(std::move(i2cDevPath)), mDevAddr(devAddr) {
    mI2cFileDescriptor = open(mI2cDevPath.c_str(), O_RDWR);
    if (mI2cFileDescriptor == 0) {
        throw std::runtime_error("Can't open I2C device.");
    }

    if (ioctl(mI2cFileDescriptor, I2C_SLAVE, mDevAddr) < 0) {
        throw std::runtime_error("Can't specify device address");
    }
}

I2C::~I2C() {
    close(mI2cFileDescriptor);
}

std::vector<std::byte> I2C::read(const std::byte &addr, unsigned int size) {
    std::vector<std::byte> addrBuff {addr};
    writeRaw(addrBuff);
    return readRaw(size);
}

void I2C::write(const std::byte &addr, const std::vector<std::byte> &data) {
    std::vector<std::byte> writeBuff;
    writeBuff.push_back(addr);
    writeBuff.insert(writeBuff.end(), data.begin(), data.end());

    std::cout << "Buffer to write: ";
    for (auto i : writeBuff) {
        std::cout << "0x" << std::hex << std::to_integer<unsigned int>(i) << " ";
    }
    std::cout << std::endl;

    writeRaw(writeBuff);
}

std::vector<std::byte> I2C::readRaw(unsigned int size) {
    std::byte buff[size];
    std::vector<std::byte> retBuff;

    int ret = ::read(mI2cFileDescriptor, reinterpret_cast<uint8_t*>(&buff[0]), size);

    if(ret == -1) {
        throw std::runtime_error("Can't read from I2C");
    }

    for (auto byte : buff) {
        retBuff.push_back(byte);
    }
    return retBuff;
}

void I2C::writeRaw(const std::vector<std::byte> buff) {
    int ret = ::write(mI2cFileDescriptor, reinterpret_cast<const uint8_t*>(buff.data()), buff.size());

    if (ret == -1) {
        throw std::runtime_error("Can't write to I2C");
    }
}