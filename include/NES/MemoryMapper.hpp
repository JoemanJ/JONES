#pragma once
#include <array>
#include "commons.hpp"
#include "NES/IMemoryMapper.hpp"

#define CPU_SPACE_TOTAL_MEMORY 64*1024

class MemoryMapper: IMemoryMapper{
    public:
        /*
        Initializes RAM data as all 0s
        */
        MemoryMapper();

        /*
        Reads data from the given address in the given address space.
        Maps the given address to the correct address or register of
        memory or IO devices.
        Puts read data on dbus and given address on addrbus.
        */
        byte_t read(addr_t address, ADDRESS_SPACE space);
        
        /*
        Writes data to the given address in the given address space.
        Maps the given address to the correct address or register of
        memory or IO devices.
        Puts written data on dbus and given address on addrbus.
        */
        void write(addr_t address, ADDRESS_SPACE space, byte_t data);

    private:
        byte_t dbus = 0;
        byte_t addrbus = 0;
        std::array<byte_t,CPU_SPACE_TOTAL_MEMORY> CPUspace;
};