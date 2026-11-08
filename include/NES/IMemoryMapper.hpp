#pragma once

enum ADDRESS_SPACE{
    CPU,
    PPU
};

class IMemoryMapper {
public:
    virtual ~IMemoryMapper() = default;

    virtual byte_t read(addr_t address, ADDRESS_SPACE space) = 0;
    virtual void write(addr_t address, ADDRESS_SPACE space, byte_t data) = 0;
};