#include <NES/MemoryMapper.hpp>

using namespace std;

MemoryMapper::MemoryMapper(){
    CPUspace = array<byte_t, CPU_SPACE_TOTAL_MEMORY>();
    CPUspace.fill(0);
}

byte_t MemoryMapper::read(addr_t address, ADDRESS_SPACE space){
    byte_t data = CPUspace.at(address);
    dbus = data;
    addrbus = address;

    return data;
}

void MemoryMapper::write(addr_t address, ADDRESS_SPACE space, byte_t data){
    dbus = data;
    addrbus = address;

    CPUspace.at(address) = data;
}
