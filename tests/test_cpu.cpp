#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <array>
#include <filesystem>
#include "mocks.hpp"

#include "commons.hpp"
#include "NES/6502.hpp"
#include "NES/MemoryMapper.hpp"

using namespace std;
using namespace testing;

class CPUTest : public Test {
    protected:

        addr_t& getPC(){return cpu.PC;}
        byte_t& getA(){return cpu.A;}
        byte_t& getX(){return cpu.X;}
        byte_t& getY(){return cpu.Y;}
        byte_t& getIAL(){return cpu.IAL;}
        byte_t& getIDL(){return cpu.IDL;}
        byte_t& getAddrL(){return cpu.addrL;}
        byte_t& getAddrH(){return cpu.addrH;}
        byte_t& getSP(){return cpu.SP;}
        byte_t& getStatus(){return cpu.status;}
    
        sptr<NiceMock<MockMemoryMapper>> memoryMapper;
        CPU6502 cpu;
    public:
        CPUTest()
            : memoryMapper(make_shared<NiceMock<MockMemoryMapper>>()),
              cpu(memoryMapper)
        {}

};

// TEST_F(CPUTest, InstructionLDAImmediate){
//     byte_t opcodeLDAIM = 0b10101001;
//     getPC() = 0x0;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeLDAIM));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0x42);
// }

// TEST_F(CPUTest, InstructionLDAZeroPage){
//     byte_t opcodeLDAZP = 0b10100101;
//     getPC() = 0x0;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeLDAZP));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0x10));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0x10);
// }

// TEST_F(CPUTest, InstructionLDAZeroPageX){
//     byte_t opcodeLDAZPX = 0b10110101;
//     // No overflow on AddrL + X
//     getPC() = 0x0;
//     getX() = 0x10;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeLDAZPX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x52);
//     EXPECT_CALL(*memoryMapper, read(0x52, ADDRESS_SPACE::CPU)).WillOnce(Return(0x10));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0x10);

//     // Overflow on AddrL + X
//     getPC() = 0x0;
//     getX() = 0xFF;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeLDAZPX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x1));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x00);
//     EXPECT_CALL(*memoryMapper, read(0x00, ADDRESS_SPACE::CPU)).WillOnce(Return(0x10));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0x10);
// }

// TEST_F(CPUTest, InstructionLDAAbsolute){
//     byte_t opcodeLDAABS = 0b10101101;
//     getPC() = 0x0;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeLDAABS));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x10));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0x10);
// }

// TEST_F(CPUTest, InstructionLDAAbsoluteX){
//     byte_t opcodeLDAABSX = 0b10111101;
//     // No "oops" cycle (addrL + X does not cross a page)
//     getPC() = 0x0;
//     getX() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeLDAABSX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCC));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCC);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_EQ(getIAL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x10));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0x10);

//     // "oops" cycle (addrL + X does cross a page)
//     getPC() = 0x0;
//     getX() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeLDAABSX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFF));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xFF);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFE));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xFE);
//     EXPECT_EQ(getIAL(), 0x00);
//     EXPECT_CALL(*memoryMapper, read(0xFE00, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xFF);
//     EXPECT_EQ(getIAL(), 0x00);
//     EXPECT_CALL(*memoryMapper, read(0xFF00, ADDRESS_SPACE::CPU)).WillOnce(Return(0x10));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0x10);
// }

// TEST_F(CPUTest, InstructionLDAAbsoluteY){
//     byte_t opcodeLDAABSY = 0b10111001;
//     // No "oops" cycle (addrL + Y does not cross a page)
//     getPC() = 0x0;
//     getY() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeLDAABSY));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCC));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCC);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_EQ(getIAL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x10));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0x10);

//     // "oops" cycle (addrL + Y does cross a page)
//     getPC() = 0x0;
//     getY() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeLDAABSY));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFF));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xFF);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFE));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xFE);
//     EXPECT_EQ(getIAL(), 0x00);
//     EXPECT_CALL(*memoryMapper, read(0xFE00, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xFF);
//     EXPECT_EQ(getIAL(), 0x00);
//     EXPECT_CALL(*memoryMapper, read(0xFF00, ADDRESS_SPACE::CPU)).WillOnce(Return(0x10));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0x10);
// }

// TEST_F(CPUTest, InstructionLDAIndirectX){
//     byte_t opcodeLDAINDX = 0b10100001;
//     // No page crossing on (pointer + X)
//     getPC() = 0x0;
//     getX() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeLDAINDX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0x42);
//     EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x43);
//     EXPECT_CALL(*memoryMapper, read(0x0043, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0x0044, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x10));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0x10);

//     // Page crossing on (pointer + X)
//     getPC() = 0x0;
//     getX() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeLDAINDX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFF));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xFF);
//     EXPECT_CALL(*memoryMapper, read(0xFF, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x00);
//     EXPECT_CALL(*memoryMapper, read(0x0000, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0x0001, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x10));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0x10);

//     // Page crossing on (pointer + X + 1)
//     getPC() = 0x0;
//     getX() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeLDAINDX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFE));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xFE);
//     EXPECT_CALL(*memoryMapper, read(0xFE, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0xFF);
//     EXPECT_CALL(*memoryMapper, read(0x00FF, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0x0000, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x10));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0x10);
// }

// TEST_F(CPUTest, InstructionLDAIndirectY){
//     byte_t opcodeLDAINDY = 0b10110001;
//     // No page crossing on (pointer + Y)
//     getPC() = 0x0;
//     getY() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeLDAINDY));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0x42);
//     EXPECT_CALL(*memoryMapper, read(0x0042, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCC));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x43);
//     EXPECT_EQ(getAddrL(), 0xCC);
//     EXPECT_CALL(*memoryMapper, read(0x0043, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_EQ(getAddrL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x10));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0x10);

//     // Page crossing on (pointer + Y)
//     getPC() = 0x0;
//     getY() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeLDAINDY));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0x42);
//     EXPECT_CALL(*memoryMapper, read(0x0042, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFF));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x43);
//     EXPECT_EQ(getAddrL(), 0xFF);
//     EXPECT_CALL(*memoryMapper, read(0x0043, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_EQ(getAddrL(), 0x00);
//     EXPECT_CALL(*memoryMapper, read(0xAB00, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAC);
//     EXPECT_CALL(*memoryMapper, read(0xAC00, ADDRESS_SPACE::CPU)).WillOnce(Return(0x10));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0x10);
// }

// TEST_F(CPUTest, InstructionLDXImmediate){
//     byte_t opcodeLDXIM = 0b10100010;
//     getPC() = 0x0;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeLDXIM));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_EQ(getX(), 0x42);
// }

// TEST_F(CPUTest, InstructionLDXZeroPage){
//     byte_t opcodeLDXZP = 0b10100110;
//     getPC() = 0x0;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeLDXZP));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0x10));
//     cpu.clkstep();
//     EXPECT_EQ(getX(), 0x10);
// }

// TEST_F(CPUTest, InstructionLDXZeroPageY){
//     byte_t opcodeLDXZPY = 0b10110110;
//     // No overflow on AddrL + Y
//     getPC() = 0x0;
//     getY() = 0x10;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeLDXZPY));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x52);
//     EXPECT_CALL(*memoryMapper, read(0x52, ADDRESS_SPACE::CPU)).WillOnce(Return(0x10));
//     cpu.clkstep();
//     EXPECT_EQ(getX(), 0x10);

//     // Overflow on AddrL + Y
//     getPC() = 0x0;
//     getY() = 0xFF;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeLDXZPY));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x1));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x00);
//     EXPECT_CALL(*memoryMapper, read(0x00, ADDRESS_SPACE::CPU)).WillOnce(Return(0x10));
//     cpu.clkstep();
//     EXPECT_EQ(getX(), 0x10);
// }

// TEST_F(CPUTest, InstructionLDXAbsolute){
//     byte_t opcodeLDXABS = 0b10101110;
//     getPC() = 0x0;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeLDXABS));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x10));
//     cpu.clkstep();
//     EXPECT_EQ(getX(), 0x10);
// }

// TEST_F(CPUTest, InstructionLDXAbsoluteY){
//     byte_t opcodeLDXABSY = 0b10111110;
//     // No "oops" cycle (addrL + Y does not cross a page)
//     getPC() = 0x0;
//     getY() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeLDXABSY));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCC));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCC);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_EQ(getIAL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x10));
//     cpu.clkstep();
//     EXPECT_EQ(getX(), 0x10);

//     // "oops" cycle (addrL + X does cross a page)
//     getPC() = 0x0;
//     getY() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeLDXABSY));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFF));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xFF);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFE));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xFE);
//     EXPECT_EQ(getIAL(), 0x00);
//     EXPECT_CALL(*memoryMapper, read(0xFE00, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xFF);
//     EXPECT_EQ(getIAL(), 0x00);
//     EXPECT_CALL(*memoryMapper, read(0xFF00, ADDRESS_SPACE::CPU)).WillOnce(Return(0x10));
//     cpu.clkstep();
//     EXPECT_EQ(getX(), 0x10);
// }

// TEST_F(CPUTest, InstructionLDYImmediate){
//     byte_t opcodeLDYIM = 0b10100000;
//     getPC() = 0x0;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeLDYIM));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_EQ(getY(), 0x42);
// }

// TEST_F(CPUTest, InstructionLDYZeroPage){
//     byte_t opcodeLDYZP = 0b10100100;
//     getPC() = 0x0;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeLDYZP));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0x10));
//     cpu.clkstep();
//     EXPECT_EQ(getY(), 0x10);
// }

// TEST_F(CPUTest, InstructionLDYZeroPageX){
//     byte_t opcodeLDYZPX = 0b10110100;
//     // No overflow on AddrL + X
//     getPC() = 0x0;
//     getX() = 0x10;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeLDYZPX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x52);
//     EXPECT_CALL(*memoryMapper, read(0x52, ADDRESS_SPACE::CPU)).WillOnce(Return(0x10));
//     cpu.clkstep();
//     EXPECT_EQ(getY(), 0x10);

//     // Overflow on AddrL + X
//     getPC() = 0x0;
//     getX() = 0xFF;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeLDYZPX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x1));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x00);
//     EXPECT_CALL(*memoryMapper, read(0x00, ADDRESS_SPACE::CPU)).WillOnce(Return(0x10));
//     cpu.clkstep();
//     EXPECT_EQ(getY(), 0x10);
// }

// TEST_F(CPUTest, InstructionLDYAbsolute){
//     byte_t opcodeLDYABS = 0b10101100;
//     getPC() = 0x0;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeLDYABS));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x10));
//     cpu.clkstep();
//     EXPECT_EQ(getY(), 0x10);
// }

// TEST_F(CPUTest, InstructionLDYAbsoluteX){
//     byte_t opcodeLDYABSX = 0b10111100;
//     // No "oops" cycle (addrL + X does not cross a page)
//     getPC() = 0x0;
//     getX() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeLDYABSX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCC));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCC);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_EQ(getIAL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x10));
//     cpu.clkstep();
//     EXPECT_EQ(getY(), 0x10);

//     // "oops" cycle (addrL + X does cross a page)
//     getPC() = 0x0;
//     getX() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeLDYABSX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFF));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xFF);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFE));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xFE);
//     EXPECT_EQ(getIAL(), 0x00);
//     EXPECT_CALL(*memoryMapper, read(0xFE00, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xFF);
//     EXPECT_EQ(getIAL(), 0x00);
//     EXPECT_CALL(*memoryMapper, read(0xFF00, ADDRESS_SPACE::CPU)).WillOnce(Return(0x10));
//     cpu.clkstep();
//     EXPECT_EQ(getY(), 0x10);
// }

// TEST_F(CPUTest, InstructionSTAZeroPage){
//     getPC() = 0x0;
//     getA() = 0x10;
//     byte_t opcodeSTAZP = 0b10000101;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeSTAZP));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, write(0x42, ADDRESS_SPACE::CPU, 0x10));
//     cpu.clkstep();
// }

// TEST_F(CPUTest, InstructionSTAZeroPageX){
//     getPC() = 0x0;
//     getA() = 0x10;
//     getX() = 0x40;
//     byte_t opcodeSTAZPX = 0b10010101;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeSTAZPX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x02));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x02, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x42);
//     EXPECT_CALL(*memoryMapper, write(0x42, ADDRESS_SPACE::CPU, 0x10));
//     cpu.clkstep();
// }

// TEST_F(CPUTest, InstructionSTAAbsolute){
//     getPC() = 0x0;
//     getA() = 0x10;
//     byte_t opcodeSTAABS = 0b10001101;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeSTAABS));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, write(0xABCD, ADDRESS_SPACE::CPU, 0x10));
//     cpu.clkstep();
// }

// TEST_F(CPUTest, InstructionSTAAbsoluteX){
//     byte_t opcodeSTAABSX = 0b10011101;
//     // No page crossing
//     getPC() = 0x0;
//     getA() = 0x10;
//     getX() = 0xCD;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeSTAABSX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x00));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, write(0xABCD, ADDRESS_SPACE::CPU, 0x10));
//     cpu.clkstep();

//     // Page crossing
//     getPC() = 0x0;
//     getA() = 0x10;
//     getX() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeSTAABSX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFF));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x00);
//     EXPECT_CALL(*memoryMapper, read(0xAB00, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAC);
//     EXPECT_EQ(getIAL(), 0x00);
//     EXPECT_CALL(*memoryMapper, write(0xAC00, ADDRESS_SPACE::CPU, 0x10));
//     cpu.clkstep();
// }

// TEST_F(CPUTest, InstructionSTAAbsoluteY){
//     byte_t opcodeSTAABSY = 0b10011001;
//     // No page crossing
//     getPC() = 0x0;
//     getA() = 0x10;
//     getY() = 0xCD;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeSTAABSY));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x00));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, write(0xABCD, ADDRESS_SPACE::CPU, 0x10));
//     cpu.clkstep();

//     // Page crossing
//     getPC() = 0x0;
//     getA() = 0x10;
//     getY() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeSTAABSY));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFF));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x00);
//     EXPECT_CALL(*memoryMapper, read(0xAB00, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAC);
//     EXPECT_EQ(getIAL(), 0x00);
//     EXPECT_CALL(*memoryMapper, write(0xAC00, ADDRESS_SPACE::CPU, 0x10));
//     cpu.clkstep();
// }

// TEST_F(CPUTest, InstructionSTAIndirectX){
//     byte_t opcodeSTAINDX = 0b10000001;
//     // No page crossing on (pointer + X)
//     getPC() = 0x0;
//     getA() = 0x10;
//     getX() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeSTAINDX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0x42);
//     EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x43);
//     EXPECT_CALL(*memoryMapper, read(0x0043, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0x0044, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_CALL(*memoryMapper, write(0xABCD, ADDRESS_SPACE::CPU, 0x10));
//     cpu.clkstep();

//     // Page crossing on (pointer + X)
//     getPC() = 0x0;
//     getA() = 0x10;
//     getX() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeSTAINDX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFF));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xFF);
//     EXPECT_CALL(*memoryMapper, read(0xFF, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x00);
//     EXPECT_CALL(*memoryMapper, read(0x0000, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0x0001, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_CALL(*memoryMapper, write(0xABCD, ADDRESS_SPACE::CPU, 0x10));
//     cpu.clkstep();

//     // Page crossing on (pointer + X + 1)
//     getPC() = 0x0;
//     getA() = 0x10;
//     getX() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeSTAINDX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFE));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xFE);
//     EXPECT_CALL(*memoryMapper, read(0xFE, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0xFF);
//     EXPECT_CALL(*memoryMapper, read(0x00FF, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0x0000, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_CALL(*memoryMapper, write(0xABCD, ADDRESS_SPACE::CPU, 0x10));
//     cpu.clkstep();
// }

// TEST_F(CPUTest, InstructionSTAIndirectY){
//     byte_t opcodeSTAINDY = 0b10010001;
//     // No page crossing on (pointer + Y)
//     getPC() = 0x0;
//     getA() = 0x10;
//     getY() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeSTAINDY));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0x42);
//     EXPECT_CALL(*memoryMapper, read(0x0042, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCC));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x43);
//     EXPECT_EQ(getAddrL(), 0xCC);
//     EXPECT_CALL(*memoryMapper, read(0x0043, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_EQ(getAddrL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, write(0xABCD, ADDRESS_SPACE::CPU, 0x10));
//     cpu.clkstep();

//     // Page crossing on (pointer + Y)
//     getPC() = 0x0;
//     getA() = 0x10;
//     getY() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeSTAINDY));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0x42);
//     EXPECT_CALL(*memoryMapper, read(0x0042, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFF));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x43);
//     EXPECT_EQ(getAddrL(), 0xFF);
//     EXPECT_CALL(*memoryMapper, read(0x0043, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_EQ(getAddrL(), 0x00);
//     EXPECT_CALL(*memoryMapper, read(0xAB00, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAC);
//     EXPECT_CALL(*memoryMapper, write(0xAC00, ADDRESS_SPACE::CPU, 0x10));
//     cpu.clkstep();
// }

// TEST_F(CPUTest, InstructionSTXZeroPage){
//     getPC() = 0x0;
//     getX() = 0x10;
//     byte_t opcodeSTXZP = 0b10000110;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeSTXZP));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, write(0x42, ADDRESS_SPACE::CPU, 0x10));
//     cpu.clkstep();
// }

// TEST_F(CPUTest, InstructionSTXZeroPageY){
//     getPC() = 0x0;
//     getX() = 0x10;
//     getY() = 0x40;
//     byte_t opcodeSTXZPY = 0b10010110;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeSTXZPY));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x02));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x02, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x42);
//     EXPECT_CALL(*memoryMapper, write(0x42, ADDRESS_SPACE::CPU, 0x10));
//     cpu.clkstep();
// }

// TEST_F(CPUTest, InstructionSTXAbsolute){
//     getPC() = 0x0;
//     getX() = 0x10;
//     byte_t opcodeSTXABS = 0b10001110;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeSTXABS));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, write(0xABCD, ADDRESS_SPACE::CPU, 0x10));
//     cpu.clkstep();
// }

// TEST_F(CPUTest, InstructionSTYZeroPage){
//     getPC() = 0x0;
//     getY() = 0x10;
//     byte_t opcodeSTYZP = 0b10000100;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeSTYZP));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, write(0x42, ADDRESS_SPACE::CPU, 0x10));
//     cpu.clkstep();
// }

// TEST_F(CPUTest, InstructionSTYZeroPageX){
//     getPC() = 0x0;
//     getY() = 0x10;
//     getX() = 0x40;
//     byte_t opcodeSTXZPY = 0b10010100;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeSTXZPY));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x02));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x02, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x42);
//     EXPECT_CALL(*memoryMapper, write(0x42, ADDRESS_SPACE::CPU, 0x10));
//     cpu.clkstep();
// }

// TEST_F(CPUTest, InstructionSTYAbsolute){
//     getPC() = 0x0;
//     getY() = 0x10;
//     byte_t opcodeSTXABS = 0b10001100;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeSTXABS));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, write(0xABCD, ADDRESS_SPACE::CPU, 0x10));
//     cpu.clkstep();
// }

// TEST_F(CPUTest, InstructionTAX){
//     getPC() = 0x0;
//     getA() = 0x10;
//     getX() = 0x00;
//     byte_t opcodeTAX = 0b10101010;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeTAX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getX(), 0x10);
// }

// TEST_F(CPUTest, InstructionTAY){
//     getPC() = 0x0;
//     getA() = 0x10;
//     getY() = 0x00;
//     byte_t opcodeTAY = 0b10101000;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeTAY));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getY(), 0x10);
// }

// TEST_F(CPUTest, InstructionTXA){
//     getPC() = 0x0;
//     getX() = 0x10;
//     getA() = 0x00;
//     byte_t opcodeTXA = 0b10001010;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeTXA));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0x10);
// }

// TEST_F(CPUTest, InstructionTYA){
//     getPC() = 0x0;
//     getY() = 0x10;
//     getA() = 0x00;
//     byte_t opcodeTYA = 0b10011000;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeTYA));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0x10);
// }

// TEST_F(CPUTest, InstructionNOP){
//     getPC() = 0x0;
//     byte_t opcodeNOP = 0b11101010;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeNOP));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
// }

// TEST_F(CPUTest, InstructionPHA){
//     getPC() = 0x0;
//     getSP() = 0xFF;
//     getA() = 0x10;
//     byte_t opcodePHA = 0b01001000;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodePHA));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, write(0x01FF, ADDRESS_SPACE::CPU, 0x10));
//     cpu.clkstep();
//     EXPECT_EQ(getSP(), 0xFE);
// }

// TEST_F(CPUTest, InstructionPLA){
//     getPC() = 0x0;
//     getSP() = 0x41;
//     getA() = 0x00;
//     byte_t opcodePLA = 0b01101000;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodePLA));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     cpu.clkstep();
//     EXPECT_EQ(getSP(), 0x42);
//     EXPECT_CALL(*memoryMapper, read(0x142, ADDRESS_SPACE::CPU)).WillOnce(Return(0x10));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0x10);
// }

// TEST_F(CPUTest, InstructionPHP){
//     getPC() = 0x0;
//     getSP() = 0xFF;
//     getStatus() = 0b11101111;
//     byte_t opcodePHP = 0b00001000;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodePHP));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, write(0x01FF, ADDRESS_SPACE::CPU, 0b11111111));
//     cpu.clkstep();
//     EXPECT_EQ(getSP(), 0xFE);
// }

// TEST_F(CPUTest, InstructionPLP){
//     getPC() = 0x0;
//     getSP() = 0x41;
//     getStatus() = 0x00;
//     byte_t opcodePLP = 0b00101000;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodePLP));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     cpu.clkstep();
//     EXPECT_EQ(getSP(), 0x42);
//     EXPECT_CALL(*memoryMapper, read(0x142, ADDRESS_SPACE::CPU)).WillOnce(Return(0b11111111));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus(), 0b11001111);
// }

// TEST_F(CPUTest, InstructionTSX){
//     getPC() = 0x0;
//     getSP() = 0x10;
//     getX() = 0x00;
//     byte_t opcodeTSX = 0b10111010;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeTSX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getX(), 0x10);
// }

// TEST_F(CPUTest, InstructionTXS){
//     getPC() = 0x0;
//     getX() = 0x10;
//     getSP() = 0x00;
//     byte_t opcodeTXS = 0b10011010;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeTXS));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getSP(), 0x10);
// }

// TEST_F(CPUTest, InstructionCLC){
//     getPC() = 0x0;
//     getStatus() = 0b11111111;
//     byte_t opcodeCLC = 0b00011000;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeCLC));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus(), 0b11111110);
// }

// TEST_F(CPUTest, InstructionSEC){
//     getPC() = 0x0;
//     getStatus() = 0b00000000;
//     byte_t opcodeSEC = 0b00111000;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeSEC));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus(), 0b00000001);
// }

// TEST_F(CPUTest, InstructionCLI){
//     getPC() = 0x0;
//     getStatus() = 0b11111111;
//     byte_t opcodeCLI = 0b01011000;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeCLI));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus(), 0b11111011);
// }

// TEST_F(CPUTest, InstructionSEI){
//     getPC() = 0x0;
//     getStatus() = 0b00000000;
//     byte_t opcodeSEI = 0b01111000;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeSEI));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus(), 0b00000100);
// }

// TEST_F(CPUTest, InstructionCLD){
//     getPC() = 0x0;
//     getStatus() = 0b11111111;
//     byte_t opcodeCLD = 0b11011000;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeCLD));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus(), 0b11110111);
// }

// TEST_F(CPUTest, InstructionSED){
//     getPC() = 0x0;
//     getStatus() = 0b00000000;
//     byte_t opcodeSED = 0b11111000;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeSED));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus(), 0b00001000);
// }

// TEST_F(CPUTest, InstructionCLV){
//     getPC() = 0x0;
//     getStatus() = 0b11111111;
//     byte_t opcodeCLV = 0b10111000;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeCLV));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus(), 0b10111111);
// }

// TEST_F(CPUTest, InstructionINX){
//     getPC() = 0x0;
//     getX() = 0;
//     byte_t opcodeINX = 0b11101000;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeINX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getX(), 1);
// }

// TEST_F(CPUTest, InstructionINY){
//     getPC() = 0x0;
//     getY() = 0;
//     byte_t opcodeINY = 0b11001000;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeINY));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getY(), 1);
// }

// TEST_F(CPUTest, InstructionDEX){
//     getPC() = 0x0;
//     getX() = 1;
//     byte_t opcodeDEX = 0b11001010;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeDEX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getX(), 0);
// }

// TEST_F(CPUTest, InstructionDEY){
//     getPC() = 0x0;
//     getY() = 1;
//     byte_t opcodeDEY = 0b10001000;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeDEY));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getY(), 0);
// }

// TEST_F(CPUTest, InstructionINCZeroPage){
//     getPC() = 0x0;
//     byte_t opcodeINCZP = 0b11100110;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeINCZP));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0x10));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0x10);
//     EXPECT_CALL(*memoryMapper, write(0x42, ADDRESS_SPACE::CPU, 0x10));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0x11);
//     EXPECT_CALL(*memoryMapper, write(0x42, ADDRESS_SPACE::CPU, 0x11));
//     cpu.clkstep();
// }

// TEST_F(CPUTest, InstructionINCZeroPageX){
//     getPC() = 0x0;
//     getX() = 0x2;
//     byte_t opcodeINCZPX = 0b11110110;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeINCZPX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x40));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x40, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x42);
//     EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0x10));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0x10);
//     EXPECT_CALL(*memoryMapper, write(0x42, ADDRESS_SPACE::CPU, 0x10));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0x11);
//     EXPECT_CALL(*memoryMapper, write(0x42, ADDRESS_SPACE::CPU, 0x11));
//     cpu.clkstep();
// }

// TEST_F(CPUTest, InstructionINCAbsolute){
//     getPC() = 0x0;
//     byte_t opcodeINCABS = 0b11101110;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeINCABS));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x10));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0x10);
//     EXPECT_CALL(*memoryMapper, write(0xABCD, ADDRESS_SPACE::CPU, 0x10));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0x11);
//     EXPECT_CALL(*memoryMapper, write(0xABCD, ADDRESS_SPACE::CPU, 0x11));
//     cpu.clkstep();
// }

// TEST_F(CPUTest, InstructionINCAbsoluteX){
//     byte_t opcodeINCABSX = 0b11111110;
    
//     // No page crossing
//     getPC() = 0x0;
//     getX() = 0x1;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeINCABSX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCC));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x10));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x10));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0x10);
//     EXPECT_CALL(*memoryMapper, write(0xABCD, ADDRESS_SPACE::CPU, 0x10));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0x11);
//     EXPECT_CALL(*memoryMapper, write(0xABCD, ADDRESS_SPACE::CPU, 0x11));
//     cpu.clkstep();

//     // Page crossing
//     getPC() = 0x0;
//     getX() = 0x1;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeINCABSX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFF));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x0);
//     EXPECT_CALL(*memoryMapper, read(0xAB00, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0xAC00, ADDRESS_SPACE::CPU)).WillOnce(Return(0x10));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0x10);
//     EXPECT_CALL(*memoryMapper, write(0xAC00, ADDRESS_SPACE::CPU, 0x10));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0x11);
//     EXPECT_CALL(*memoryMapper, write(0xAC00, ADDRESS_SPACE::CPU, 0x11));
//     cpu.clkstep();
// }

// TEST_F(CPUTest, InstructionDECZeroPage){
//     getPC() = 0x0;
//     byte_t opcodeDECZP = 0b11000110;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeDECZP));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0x10));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0x10);
//     EXPECT_CALL(*memoryMapper, write(0x42, ADDRESS_SPACE::CPU, 0x10));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0x0F);
//     EXPECT_CALL(*memoryMapper, write(0x42, ADDRESS_SPACE::CPU, 0x0F));
//     cpu.clkstep();
// }

// TEST_F(CPUTest, InstructionDECZeroPageX){
//     getPC() = 0x0;
//     getX() = 0x2;
//     byte_t opcodeDECZPX = 0b11010110;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeDECZPX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x40));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x40, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x42);
//     EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0x10));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0x10);
//     EXPECT_CALL(*memoryMapper, write(0x42, ADDRESS_SPACE::CPU, 0x10));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0x0F);
//     EXPECT_CALL(*memoryMapper, write(0x42, ADDRESS_SPACE::CPU, 0x0F));
//     cpu.clkstep();
// }

// TEST_F(CPUTest, InstructionDECAbsolute){
//     getPC() = 0x0;
//     byte_t opcodeDECABS = 0b11001110;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeDECABS));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x10));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0x10);
//     EXPECT_CALL(*memoryMapper, write(0xABCD, ADDRESS_SPACE::CPU, 0x10));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0x0F);
//     EXPECT_CALL(*memoryMapper, write(0xABCD, ADDRESS_SPACE::CPU, 0x0F));
//     cpu.clkstep();
// }

// TEST_F(CPUTest, InstructionDECAbsoluteX){
//     byte_t opcodeDECABSX = 0b11011110;
    
//     // No page crossing
//     getPC() = 0x0;
//     getX() = 0x1;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeDECABSX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCC));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x10));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x10));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0x10);
//     EXPECT_CALL(*memoryMapper, write(0xABCD, ADDRESS_SPACE::CPU, 0x10));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0x0F);
//     EXPECT_CALL(*memoryMapper, write(0xABCD, ADDRESS_SPACE::CPU, 0x0F));
//     cpu.clkstep();

//     // Page crossing
//     getPC() = 0x0;
//     getX() = 0x1;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeDECABSX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFF));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x0);
//     EXPECT_CALL(*memoryMapper, read(0xAB00, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0xAC00, ADDRESS_SPACE::CPU)).WillOnce(Return(0x10));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0x10);
//     EXPECT_CALL(*memoryMapper, write(0xAC00, ADDRESS_SPACE::CPU, 0x10));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0x0F);
//     EXPECT_CALL(*memoryMapper, write(0xAC00, ADDRESS_SPACE::CPU, 0x0F));
//     cpu.clkstep();
// }

// TEST_F(CPUTest, InstructionANDImmediate){
//     byte_t opcodeANDIM = 0b00101001;
//     getPC() = 0x0;
//     getA() = 0b11110000;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeANDIM));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00111111));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0b00110000);
// }

// TEST_F(CPUTest, InstructionANDZeroPage){
//     byte_t opcodeANDZP = 0b00100101;
//     getPC() = 0x0;
//     getA() = 0b11110000;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeANDZP));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00111111));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0b00110000);
// }

// TEST_F(CPUTest, InstructionANDZeroPageX){
//     byte_t opcodeANDZPX = 0b00110101;
//     // No overflow on AddrL + X
//     getPC() = 0x0;
//     getA() = 0b11110000;
//     getX() = 0x10;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeANDZPX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x52);
//     EXPECT_CALL(*memoryMapper, read(0x52, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00111111));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0b00110000);

//     // Overflow on AddrL + X
//     getPC() = 0x0;
//     getA() = 0b11110000;
//     getX() = 0xFF;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeANDZPX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x1));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x00);
//     EXPECT_CALL(*memoryMapper, read(0x00, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00111111));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0b00110000);
// }

// TEST_F(CPUTest, InstructionANDAbsolute){
//     byte_t opcodeANDABS = 0b00101101;
//     getA() = 0b11110000;
//     getPC() = 0x0;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeANDABS));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00111111));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0b00110000);
// }

// TEST_F(CPUTest, InstructionANDAbsoluteX){
//     byte_t opcodeANDABSX = 0b00111101;
//     // No "oops" cycle (addrL + X does not cross a page)
//     getPC() = 0x0;
//     getA() = 0b11110000;
//     getX() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeANDABSX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCC));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCC);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_EQ(getIAL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00111111));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0b00110000);

//     // "oops" cycle (addrL + X does cross a page)
//     getPC() = 0x0;
//     getA() = 0b11110000;
//     getX() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeANDABSX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFF));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xFF);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFE));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xFE);
//     EXPECT_EQ(getIAL(), 0x00);
//     EXPECT_CALL(*memoryMapper, read(0xFE00, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xFF);
//     EXPECT_EQ(getIAL(), 0x00);
//     EXPECT_CALL(*memoryMapper, read(0xFF00, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00111111));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0b00110000);
// }

// TEST_F(CPUTest, InstructionANDAbsoluteY){
//     byte_t opcodeANDABSY = 0b00111001;
//     // No "oops" cycle (addrL + Y does not cross a page)
//     getPC() = 0x0;
//     getA() = 0b11110000;
//     getY() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeANDABSY));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCC));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCC);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_EQ(getIAL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00111111));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0b00110000);

//     // "oops" cycle (addrL + Y does cross a page)
//     getPC() = 0x0;
//     getA() = 0b11110000;
//     getY() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeANDABSY));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFF));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xFF);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFE));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xFE);
//     EXPECT_EQ(getIAL(), 0x00);
//     EXPECT_CALL(*memoryMapper, read(0xFE00, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xFF);
//     EXPECT_EQ(getIAL(), 0x00);
//     EXPECT_CALL(*memoryMapper, read(0xFF00, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00111111));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0b00110000);
// }

// TEST_F(CPUTest, InstructionANDIndirectX){
//     byte_t opcodeANDINDX = 0b00100001;
//     // No page crossing on (pointer + X)
//     getPC() = 0x0;
//     getA() = 0b11110000;
//     getX() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeANDINDX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0x42);
//     EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x43);
//     EXPECT_CALL(*memoryMapper, read(0x0043, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0x0044, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00111111));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0b00110000);

//     // Page crossing on (pointer + X)
//     getPC() = 0x0;
//     getA() = 0b11110000;
//     getX() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeANDINDX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFF));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xFF);
//     EXPECT_CALL(*memoryMapper, read(0xFF, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x00);
//     EXPECT_CALL(*memoryMapper, read(0x0000, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0x0001, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00111111));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0b00110000);

//     // Page crossing on (pointer + X + 1)
//     getPC() = 0x0;
//     getA() = 0b11110000;
//     getX() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeANDINDX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFE));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xFE);
//     EXPECT_CALL(*memoryMapper, read(0xFE, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0xFF);
//     EXPECT_CALL(*memoryMapper, read(0x00FF, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0x0000, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00111111));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0b00110000);
// }

// TEST_F(CPUTest, InstructionANDIndirectY){
//     byte_t opcodeANDINDY = 0b00110001;
//     // No page crossing on (pointer + Y)
//     getPC() = 0x0;
//     getA() = 0b11110000;
//     getY() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeANDINDY));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0x42);
//     EXPECT_CALL(*memoryMapper, read(0x0042, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCC));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x43);
//     EXPECT_EQ(getAddrL(), 0xCC);
//     EXPECT_CALL(*memoryMapper, read(0x0043, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_EQ(getAddrL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00111111));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0b00110000);

//     // Page crossing on (pointer + Y)
//     getPC() = 0x0;
//     getA() = 0b11110000;
//     getY() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeANDINDY));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0x42);
//     EXPECT_CALL(*memoryMapper, read(0x0042, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFF));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x43);
//     EXPECT_EQ(getAddrL(), 0xFF);
//     EXPECT_CALL(*memoryMapper, read(0x0043, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_EQ(getAddrL(), 0x00);
//     EXPECT_CALL(*memoryMapper, read(0xAB00, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAC);
//     EXPECT_CALL(*memoryMapper, read(0xAC00, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00111111));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0b00110000);
// }

// TEST_F(CPUTest, InstructionORAImmediate){
//     byte_t opcodeORAIM = 0b00001001;
//     getPC() = 0x0;
//     getA() = 0b11110000;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeORAIM));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00111111));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0b11111111);
// }

// TEST_F(CPUTest, InstructionORAZeroPage){
//     byte_t opcodeORAZP = 0b00000101;
//     getPC() = 0x0;
//     getA() = 0b11110000;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeORAZP));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00111111));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0b11111111);
// }

// TEST_F(CPUTest, InstructionORAZeroPageX){
//     byte_t opcodeORAZPX = 0b00010101;
//     // No overflow on AddrL + X
//     getPC() = 0x0;
//     getA() = 0b11110000;
//     getX() = 0x10;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeORAZPX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x52);
//     EXPECT_CALL(*memoryMapper, read(0x52, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00111111));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0b11111111);

//     // Overflow on AddrL + X
//     getPC() = 0x0;
//     getA() = 0b11110000;
//     getX() = 0xFF;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeORAZPX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x1));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x00);
//     EXPECT_CALL(*memoryMapper, read(0x00, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00111111));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0b11111111);
// }

// TEST_F(CPUTest, InstructionORAAbsolute){
//     byte_t opcodeORAABS = 0b00001101;
//     getA() = 0b11110000;
//     getPC() = 0x0;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeORAABS));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00111111));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0b11111111);
// }

// TEST_F(CPUTest, InstructionORAAbsoluteX){
//     byte_t opcodeORAABSX = 0b00011101;
//     // No "oops" cycle (addrL + X does not cross a page)
//     getPC() = 0x0;
//     getA() = 0b11110000;
//     getX() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeORAABSX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCC));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCC);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_EQ(getIAL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00111111));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0b11111111);

//     // "oops" cycle (addrL + X does cross a page)
//     getPC() = 0x0;
//     getA() = 0b11110000;
//     getX() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeORAABSX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFF));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xFF);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFE));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xFE);
//     EXPECT_EQ(getIAL(), 0x00);
//     EXPECT_CALL(*memoryMapper, read(0xFE00, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xFF);
//     EXPECT_EQ(getIAL(), 0x00);
//     EXPECT_CALL(*memoryMapper, read(0xFF00, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00111111));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0b11111111);
// }

// TEST_F(CPUTest, InstructionORAAbsoluteY){
//     byte_t opcodeORAABSY = 0b00011001;
//     // No "oops" cycle (addrL + Y does not cross a page)
//     getPC() = 0x0;
//     getA() = 0b11110000;
//     getY() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeORAABSY));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCC));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCC);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_EQ(getIAL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00111111));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0b11111111);

//     // "oops" cycle (addrL + Y does cross a page)
//     getPC() = 0x0;
//     getA() = 0b11110000;
//     getY() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeORAABSY));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFF));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xFF);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFE));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xFE);
//     EXPECT_EQ(getIAL(), 0x00);
//     EXPECT_CALL(*memoryMapper, read(0xFE00, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xFF);
//     EXPECT_EQ(getIAL(), 0x00);
//     EXPECT_CALL(*memoryMapper, read(0xFF00, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00111111));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0b11111111);
// }

// TEST_F(CPUTest, InstructionORAIndirectX){
//     byte_t opcodeORAINDX = 0b00000001;
//     // No page crossing on (pointer + X)
//     getPC() = 0x0;
//     getA() = 0b11110000;
//     getX() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeORAINDX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0x42);
//     EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x43);
//     EXPECT_CALL(*memoryMapper, read(0x0043, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0x0044, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00111111));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0b11111111);

//     // Page crossing on (pointer + X)
//     getPC() = 0x0;
//     getA() = 0b11110000;
//     getX() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeORAINDX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFF));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xFF);
//     EXPECT_CALL(*memoryMapper, read(0xFF, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x00);
//     EXPECT_CALL(*memoryMapper, read(0x0000, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0x0001, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00111111));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0b11111111);

//     // Page crossing on (pointer + X + 1)
//     getPC() = 0x0;
//     getA() = 0b11110000;
//     getX() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeORAINDX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFE));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xFE);
//     EXPECT_CALL(*memoryMapper, read(0xFE, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0xFF);
//     EXPECT_CALL(*memoryMapper, read(0x00FF, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0x0000, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00111111));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0b11111111);
// }

// TEST_F(CPUTest, InstructionORAIndirectY){
//     byte_t opcodeORAINDY = 0b00010001;
//     // No page crossing on (pointer + Y)
//     getPC() = 0x0;
//     getA() = 0b11110000;
//     getY() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeORAINDY));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0x42);
//     EXPECT_CALL(*memoryMapper, read(0x0042, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCC));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x43);
//     EXPECT_EQ(getAddrL(), 0xCC);
//     EXPECT_CALL(*memoryMapper, read(0x0043, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_EQ(getAddrL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00111111));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0b11111111);

//     // Page crossing on (pointer + Y)
//     getPC() = 0x0;
//     getA() = 0b11110000;
//     getY() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeORAINDY));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0x42);
//     EXPECT_CALL(*memoryMapper, read(0x0042, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFF));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x43);
//     EXPECT_EQ(getAddrL(), 0xFF);
//     EXPECT_CALL(*memoryMapper, read(0x0043, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_EQ(getAddrL(), 0x00);
//     EXPECT_CALL(*memoryMapper, read(0xAB00, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAC);
//     EXPECT_CALL(*memoryMapper, read(0xAC00, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00111111));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0b11111111);
// }

// TEST_F(CPUTest, InstructionEORImmediate){
//     byte_t opcodeEORIM = 0b01001001;
//     getPC() = 0x0;
//     getA() = 0b11110000;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeEORIM));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00111111));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0b11001111);
// }

// TEST_F(CPUTest, InstructionEORZeroPage){
//     byte_t opcodeEORZP = 0b01000101;
//     getPC() = 0x0;
//     getA() = 0b11110000;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeEORZP));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00111111));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0b11001111);
// }

// TEST_F(CPUTest, InstructionEORZeroPageX){
//     byte_t opcodeEORZPX = 0b01010101;
//     // No overflow on AddrL + X
//     getPC() = 0x0;
//     getA() = 0b11110000;
//     getX() = 0x10;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeEORZPX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x52);
//     EXPECT_CALL(*memoryMapper, read(0x52, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00111111));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0b11001111);

//     // Overflow on AddrL + X
//     getPC() = 0x0;
//     getA() = 0b11110000;
//     getX() = 0xFF;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeEORZPX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x1));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x00);
//     EXPECT_CALL(*memoryMapper, read(0x00, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00111111));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0b11001111);
// }

// TEST_F(CPUTest, InstructionEORAbsolute){
//     byte_t opcodeEORABS = 0b01001101;
//     getA() = 0b11110000;
//     getPC() = 0x0;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeEORABS));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00111111));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0b11001111);
// }

// TEST_F(CPUTest, InstructionEORAbsoluteX){
//     byte_t opcodeEORABSX = 0b01011101;
//     // No "oops" cycle (addrL + X does not cross a page)
//     getPC() = 0x0;
//     getA() = 0b11110000;
//     getX() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeEORABSX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCC));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCC);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_EQ(getIAL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00111111));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0b11001111);

//     // "oops" cycle (addrL + X does cross a page)
//     getPC() = 0x0;
//     getA() = 0b11110000;
//     getX() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeEORABSX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFF));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xFF);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFE));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xFE);
//     EXPECT_EQ(getIAL(), 0x00);
//     EXPECT_CALL(*memoryMapper, read(0xFE00, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xFF);
//     EXPECT_EQ(getIAL(), 0x00);
//     EXPECT_CALL(*memoryMapper, read(0xFF00, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00111111));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0b11001111);
// }

// TEST_F(CPUTest, InstructionEORAbsoluteY){
//     byte_t opcodeEORABSY = 0b01011001;
//     // No "oops" cycle (addrL + Y does not cross a page)
//     getPC() = 0x0;
//     getA() = 0b11110000;
//     getY() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeEORABSY));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCC));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCC);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_EQ(getIAL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00111111));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0b11001111);

//     // "oops" cycle (addrL + Y does cross a page)
//     getPC() = 0x0;
//     getA() = 0b11110000;
//     getY() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeEORABSY));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFF));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xFF);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFE));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xFE);
//     EXPECT_EQ(getIAL(), 0x00);
//     EXPECT_CALL(*memoryMapper, read(0xFE00, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xFF);
//     EXPECT_EQ(getIAL(), 0x00);
//     EXPECT_CALL(*memoryMapper, read(0xFF00, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00111111));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0b11001111);
// }

// TEST_F(CPUTest, InstructionEORIndirectX){
//     byte_t opcodeEORINDX = 0b01000001;
//     // No page crossing on (pointer + X)
//     getPC() = 0x0;
//     getA() = 0b11110000;
//     getX() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeEORINDX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0x42);
//     EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x43);
//     EXPECT_CALL(*memoryMapper, read(0x0043, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0x0044, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00111111));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0b11001111);

//     // Page crossing on (pointer + X)
//     getPC() = 0x0;
//     getA() = 0b11110000;
//     getX() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeEORINDX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFF));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xFF);
//     EXPECT_CALL(*memoryMapper, read(0xFF, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x00);
//     EXPECT_CALL(*memoryMapper, read(0x0000, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0x0001, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00111111));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0b11001111);

//     // Page crossing on (pointer + X + 1)
//     getPC() = 0x0;
//     getA() = 0b11110000;
//     getX() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeEORINDX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFE));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xFE);
//     EXPECT_CALL(*memoryMapper, read(0xFE, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0xFF);
//     EXPECT_CALL(*memoryMapper, read(0x00FF, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0x0000, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00111111));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0b11001111);
// }

// TEST_F(CPUTest, InstructionEORIndirectY){
//     byte_t opcodeEORINDY = 0b01010001;
//     // No page crossing on (pointer + Y)
//     getPC() = 0x0;
//     getA() = 0b11110000;
//     getY() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeEORINDY));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0x42);
//     EXPECT_CALL(*memoryMapper, read(0x0042, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCC));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x43);
//     EXPECT_EQ(getAddrL(), 0xCC);
//     EXPECT_CALL(*memoryMapper, read(0x0043, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_EQ(getAddrL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00111111));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0b11001111);

//     // Page crossing on (pointer + Y)
//     getPC() = 0x0;
//     getA() = 0b11110000;
//     getY() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeEORINDY));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0x42);
//     EXPECT_CALL(*memoryMapper, read(0x0042, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFF));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x43);
//     EXPECT_EQ(getAddrL(), 0xFF);
//     EXPECT_CALL(*memoryMapper, read(0x0043, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_EQ(getAddrL(), 0x00);
//     EXPECT_CALL(*memoryMapper, read(0xAB00, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAC);
//     EXPECT_CALL(*memoryMapper, read(0xAC00, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00111111));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0b11001111);
// }

// TEST_F(CPUTest, InstructionBITZeroPage){
//     byte_t opcodeBITZP = 0b00100100;

//     // Zero
//     getPC() = 0x0;
//     getA() = 0x0;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeBITZP));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0x0));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0x0);
//     EXPECT_EQ(getStatus() & 0b00000010, 0b00000010);

//     // N
//     getPC() = 0x0;
//     getA() = 0x0;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeBITZP));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0b10000000));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b10000000, 0b10000000);

//     // V
//     getPC() = 0x0;
//     getA() = 0x0;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeBITZP));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0b01000000));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b01000000, 0b01000000);
// }

// TEST_F(CPUTest, InstructionBITAbsolute){
//     byte_t opcodeBITABS = 0b00101100;

//     // Zero
//     getA() = 0x0;
//     getPC() = 0x0;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeBITABS));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x0));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b00000010, 0b00000010);

//     // N
//     getA() = 0x0;
//     getPC() = 0x0;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeBITABS));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0b10000000));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b10000000, 0b10000000);

//     // V
//     getA() = 0x0;
//     getPC() = 0x0;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeBITABS));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0b01000000));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b01000000, 0b01000000);
// }

// TEST_F(CPUTest, InstructionCMPImmediate){
//     byte_t opcodeCMPIM = 0b11001001;

//     // Zero
//     getPC() = 0x0;
//     getA() = 0x42;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeCMPIM));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b00000010, 0b00000010);

//     // C
//     getPC() = 0x0;
//     getA() = 0x42;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeCMPIM));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x41));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b00000001, 0b00000001);

//     // N
//     getPC() = 0x0;
//     getA() = 0x42;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeCMPIM));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x43));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b10000000, 0b10000000);
// }

// TEST_F(CPUTest, InstructionCMPZeroPage){
//     byte_t opcodeCMPZP = 0b11000101;

//     // Zero
//     getPC() = 0x0;
//     getA() = 0x42;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeCMPZP));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b00000010, 0b00000010);

//     // C
//     getPC() = 0x0;
//     getA() = 0x42;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeCMPZP));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0x41));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b00000001, 0b00000001);

//     // N
//     getPC() = 0x0;
//     getA() = 0x42;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeCMPZP));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0x43));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b10000000, 0b10000000);
// }

// TEST_F(CPUTest, InstructionCMPZeroPageX){
//     byte_t opcodeCMPZPX = 0b11010101;

//     // Zero
//     getPC() = 0x0;
//     getA() = 0x42;
//     getX() = 0x10;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeCMPZPX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x52);
//     EXPECT_CALL(*memoryMapper, read(0x52, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b00000010, 0b00000010);

//     // C
//     getPC() = 0x0;
//     getA() = 0x42;
//     getX() = 0x10;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeCMPZPX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x52);
//     EXPECT_CALL(*memoryMapper, read(0x52, ADDRESS_SPACE::CPU)).WillOnce(Return(0x41));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b00000001, 0b00000001);

//     // N
//     getPC() = 0x0;
//     getA() = 0x42;
//     getX() = 0x10;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeCMPZPX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x52);
//     EXPECT_CALL(*memoryMapper, read(0x52, ADDRESS_SPACE::CPU)).WillOnce(Return(0x43));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b10000000, 0b10000000);
// }

// TEST_F(CPUTest, InstructionCMPAbsolute){
//     byte_t opcodeCMPABS = 0b11001101;

//     // Zero
//     getA() = 0x42;
//     getPC() = 0x0;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeCMPABS));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b00000010, 0b00000010);

//     // C
//     getA() = 0x42;
//     getPC() = 0x0;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeCMPABS));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x41));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b00000001, 0b00000001);

//     // N
//     getA() = 0x42;
//     getPC() = 0x0;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeCMPABS));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x43));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b10000000, 0b10000000);
// }

// TEST_F(CPUTest, InstructionCMPAbsoluteX){
//     byte_t opcodeCMPABSX = 0b11011101;
    
//     // Zero
//     getPC() = 0x0;
//     getA() = 0x42;
//     getX() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeCMPABSX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCC));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCC);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_EQ(getIAL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b00000010, 0b00000010);

//     // C
//     getPC() = 0x0;
//     getA() = 0x42;
//     getX() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeCMPABSX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCC));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCC);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_EQ(getIAL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x41));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b00000001, 0b00000001);

//     // N
//     getPC() = 0x0;
//     getA() = 0x42;
//     getX() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeCMPABSX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCC));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCC);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_EQ(getIAL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x43));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b10000000, 0b10000000);
// }

// TEST_F(CPUTest, InstructionCMPAbsoluteY){
//     byte_t opcodeCMPABSY = 0b11011001;
    
//     // Zero
//     getPC() = 0x0;
//     getA() = 0x42;
//     getY() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeCMPABSY));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCC));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCC);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_EQ(getIAL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b00000010, 0b00000010);

//     // C
//     getPC() = 0x0;
//     getA() = 0x42;
//     getY() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeCMPABSY));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCC));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCC);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_EQ(getIAL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x41));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b00000001, 0b00000001);

//     // N
//     getPC() = 0x0;
//     getA() = 0x42;
//     getY() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeCMPABSY));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCC));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCC);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_EQ(getIAL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x43));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b10000000, 0b10000000);
// }

// TEST_F(CPUTest, InstructionCMPIndirectX){
//     byte_t opcodeCMPINDX = 0b11000001;
    
//     // Zero
//     getPC() = 0x0;
//     getA() = 0x42;
//     getX() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeCMPINDX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0x42);
//     EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x43);
//     EXPECT_CALL(*memoryMapper, read(0x0043, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0x0044, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b00000010, 0b00000010);

//     // C
//     getPC() = 0x0;
//     getA() = 0x42;
//     getX() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeCMPINDX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0x42);
//     EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x43);
//     EXPECT_CALL(*memoryMapper, read(0x0043, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0x0044, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x41));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b00000001, 0b00000001);

//     // N
//     getPC() = 0x0;
//     getA() = 0x42;
//     getX() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeCMPINDX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0x42);
//     EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x43);
//     EXPECT_CALL(*memoryMapper, read(0x0043, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0x0044, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x43));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b10000000, 0b10000000);
// }

// TEST_F(CPUTest, InstructionCMPIndirectY){
//     byte_t opcodeCMPINDY = 0b11010001;
    
//     // Zero
//     getPC() = 0x0;
//     getA() = 0x42;
//     getY() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeCMPINDY));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0x42);
//     EXPECT_CALL(*memoryMapper, read(0x0042, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCC));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x43);
//     EXPECT_EQ(getAddrL(), 0xCC);
//     EXPECT_CALL(*memoryMapper, read(0x0043, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_EQ(getAddrL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b00000010, 0b00000010);

//     // C
//     getPC() = 0x0;
//     getA() = 0x42;
//     getY() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeCMPINDY));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0x42);
//     EXPECT_CALL(*memoryMapper, read(0x0042, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCC));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x43);
//     EXPECT_EQ(getAddrL(), 0xCC);
//     EXPECT_CALL(*memoryMapper, read(0x0043, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_EQ(getAddrL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x41));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b00000001, 0b00000001);

//     // N
//     getPC() = 0x0;
//     getA() = 0x42;
//     getY() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeCMPINDY));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0x42);
//     EXPECT_CALL(*memoryMapper, read(0x0042, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCC));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x43);
//     EXPECT_EQ(getAddrL(), 0xCC);
//     EXPECT_CALL(*memoryMapper, read(0x0043, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_EQ(getAddrL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x43));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b10000000, 0b10000000);
// }

// TEST_F(CPUTest, InstructionCPXImmediate){
//     byte_t opcodeCPXIM = 0b11100000;

//     // Zero
//     getPC() = 0x0;
//     getX() = 0x42;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeCPXIM));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b00000010, 0b00000010);

//     // C
//     getPC() = 0x0;
//     getX() = 0x42;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeCPXIM));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x41));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b00000001, 0b00000001);

//     // N
//     getPC() = 0x0;
//     getX() = 0x42;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeCPXIM));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x43));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b10000000, 0b10000000);
// }

// TEST_F(CPUTest, InstructionCPXZeroPage){
//     byte_t opcodeCPXZP = 0b11100100;

//     // Zero
//     getPC() = 0x0;
//     getX() = 0x42;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeCPXZP));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b00000010, 0b00000010);

//     // C
//     getPC() = 0x0;
//     getX() = 0x42;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeCPXZP));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0x41));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b00000001, 0b00000001);

//     // N
//     getPC() = 0x0;
//     getX() = 0x42;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeCPXZP));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0x43));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b10000000, 0b10000000);
// }

// TEST_F(CPUTest, InstructionCPXAbsolute){
//     byte_t opcodeCPXABS = 0b11101100;

//     // Zero
//     getX() = 0x42;
//     getPC() = 0x0;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeCPXABS));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b00000010, 0b00000010);

//     // C
//     getX() = 0x42;
//     getPC() = 0x0;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeCPXABS));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x41));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b00000001, 0b00000001);

//     // N
//     getX() = 0x42;
//     getPC() = 0x0;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeCPXABS));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x43));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b10000000, 0b10000000);
// }

// TEST_F(CPUTest, InstructionCPYImmediate){
//     byte_t opcodeCPYIM = 0b11000000;

//     // Zero
//     getPC() = 0x0;
//     getY() = 0x42;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeCPYIM));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b00000010, 0b00000010);

//     // C
//     getPC() = 0x0;
//     getY() = 0x42;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeCPYIM));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x41));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b00000001, 0b00000001);

//     // N
//     getPC() = 0x0;
//     getY() = 0x42;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeCPYIM));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x43));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b10000000, 0b10000000);
// }

// TEST_F(CPUTest, InstructionCPYZeroPage){
//     byte_t opcodeCPYZP = 0b11000100;

//     // Zero
//     getPC() = 0x0;
//     getY() = 0x42;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeCPYZP));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b00000010, 0b00000010);

//     // C
//     getPC() = 0x0;
//     getY() = 0x42;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeCPYZP));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0x41));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b00000001, 0b00000001);

//     // N
//     getPC() = 0x0;
//     getY() = 0x42;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeCPYZP));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0x43));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b10000000, 0b10000000);
// }

// TEST_F(CPUTest, InstructionCPYAbsolute){
//     byte_t opcodeCPYABS = 0b11001100;

//     // Zero
//     getY() = 0x42;
//     getPC() = 0x0;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeCPYABS));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b00000010, 0b00000010);

//     // C
//     getY() = 0x42;
//     getPC() = 0x0;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeCPYABS));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x41));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b00000001, 0b00000001);

//     // N
//     getY() = 0x42;
//     getPC() = 0x0;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeCPYABS));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x43));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b10000000, 0b10000000);
// }

// TEST_F(CPUTest, InstructionJMPAbsolute){
//     byte_t opcodeJMPABS = 0b01001100;

//     getPC() = 0x0;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeJMPABS));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0xABCD);
// }

// TEST_F(CPUTest, InstructionJMPIND){
//     byte_t opcodeJMPIND = 0b01101100;

//     // No page crossing
//     getPC() = 0x0;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeJMPIND));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x34));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0x34);
//     EXPECT_CALL(*memoryMapper, read(0xABCE, ADDRESS_SPACE::CPU)).WillOnce(Return(0x12));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x1234);

//     // Page crossing
//     getPC() = 0x0;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeJMPIND));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFF));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xFF);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0xABFF, ADDRESS_SPACE::CPU)).WillOnce(Return(0x34));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0xAB00, ADDRESS_SPACE::CPU)).WillOnce(Return(0x12));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x1234);
// }

// TEST_F(CPUTest, InstructionJSR){
//     byte_t opcodeJSR = 0b00100000;

//     getPC() = 0x0;
//     getSP() = 0xFF;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeJSR));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCD);
//     cpu.clkstep(); // Unknown
//     EXPECT_CALL(*memoryMapper, write(0x01FF, ADDRESS_SPACE::CPU, 0x00));
//     cpu.clkstep();
//     EXPECT_EQ(getSP(), 0xFE);
//     EXPECT_CALL(*memoryMapper, write(0x01FE, ADDRESS_SPACE::CPU, 0x02));
//     cpu.clkstep();
//     EXPECT_EQ(getSP(), 0xFD);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0xABCD);
// }

// TEST_F(CPUTest, InstructionRTS){
//     byte_t opcodeRTS = 0b01100000;

//     getPC() = 0x0;
//     getSP() = 0xFD;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeRTS));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     cpu.clkstep();
//     EXPECT_EQ(getSP(), 0xFE);
//     EXPECT_CALL(*memoryMapper, read(0x01FE, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x00CD);
//     EXPECT_EQ(getSP(), 0xFF);
//     EXPECT_CALL(*memoryMapper, read(0x01FF, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0xABCD);
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0xABCE);
// }

// TEST_F(CPUTest, InstructionBCC){
//     byte_t opcodeBCC = 0b10010000;

//     // No branch
//     getPC() = 0x0;
//     getStatus() = 0b00000001;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeBCC));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x2));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x2);

//     // Branch. No page cross
//     getPC() = 0x0;
//     getStatus() = 0b00000000;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeBCC));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x2));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x2);
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x4);

//     // Branch. Page cross upwards
//     getPC() = 0xFD;
//     getStatus() = 0b00000000;
//     EXPECT_CALL(*memoryMapper, read(0xFD, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeBCC));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0xFE, ADDRESS_SPACE::CPU)).WillOnce(Return(0x1));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0xFF);
//     EXPECT_CALL(*memoryMapper, read(0x00, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x00);
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x0100);

//     // Branch. Page cross downwards
//     getPC() = 0x0100;
//     getStatus() = 0b00000000;
//     EXPECT_CALL(*memoryMapper, read(0x0100, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeBCC));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x0101, ADDRESS_SPACE::CPU)).WillOnce(Return(-3));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x0102);
//     EXPECT_CALL(*memoryMapper, read(0x01FF, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x01FF);
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x00FF);
// }

// TEST_F(CPUTest, InstructionBCS){
//     byte_t opcodeBCS = 0b10110000;

//     // No branch
//     getPC() = 0x0;
//     getStatus() = 0b00000000;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeBCS));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x2));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x2);

//     // Branch. No page cross
//     getPC() = 0x0;
//     getStatus() = 0b00000001;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeBCS));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x2));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x2);
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x4);

//     // Branch. Page cross upwards
//     getPC() = 0xFD;
//     getStatus() = 0b00000001;
//     EXPECT_CALL(*memoryMapper, read(0xFD, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeBCS));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0xFE, ADDRESS_SPACE::CPU)).WillOnce(Return(0x1));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0xFF);
//     EXPECT_CALL(*memoryMapper, read(0x00, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x00);
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x0100);

//     // Branch. Page cross downwards
//     getPC() = 0x0100;
//     getStatus() = 0b00000001;
//     EXPECT_CALL(*memoryMapper, read(0x0100, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeBCS));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x0101, ADDRESS_SPACE::CPU)).WillOnce(Return(-3));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x0102);
//     EXPECT_CALL(*memoryMapper, read(0x01FF, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x01FF);
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x00FF);
// }

// TEST_F(CPUTest, InstructionBEQ){
//     byte_t opcodeBEQ = 0b11110000;

//     // No branch
//     getPC() = 0x0;
//     getStatus() = 0b00000000;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeBEQ));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x2));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x2);

//     // Branch. No page cross
//     getPC() = 0x0;
//     getStatus() = 0b00000010;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeBEQ));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x2));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x2);
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x4);

//     // Branch. Page cross upwards
//     getPC() = 0xFD;
//     getStatus() = 0b00000010;
//     EXPECT_CALL(*memoryMapper, read(0xFD, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeBEQ));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0xFE, ADDRESS_SPACE::CPU)).WillOnce(Return(0x1));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0xFF);
//     EXPECT_CALL(*memoryMapper, read(0x00, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x00);
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x0100);

//     // Branch. Page cross downwards
//     getPC() = 0x0100;
//     getStatus() = 0b00000010;
//     EXPECT_CALL(*memoryMapper, read(0x0100, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeBEQ));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x0101, ADDRESS_SPACE::CPU)).WillOnce(Return(-3));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x0102);
//     EXPECT_CALL(*memoryMapper, read(0x01FF, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x01FF);
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x00FF);
// }

// TEST_F(CPUTest, InstructionBMI){
//     byte_t opcodeBMI = 0b00110000;

//     // No branch
//     getPC() = 0x0;
//     getStatus() = 0b00000000;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeBMI));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x2));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x2);

//     // Branch. No page cross
//     getPC() = 0x0;
//     getStatus() = 0b10000000;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeBMI));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x2));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x2);
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x4);

//     // Branch. Page cross upwards
//     getPC() = 0xFD;
//     getStatus() = 0b10000000;
//     EXPECT_CALL(*memoryMapper, read(0xFD, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeBMI));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0xFE, ADDRESS_SPACE::CPU)).WillOnce(Return(0x1));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0xFF);
//     EXPECT_CALL(*memoryMapper, read(0x00, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x00);
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x0100);

//     // Branch. Page cross downwards
//     getPC() = 0x0100;
//     getStatus() = 0b10000000;
//     EXPECT_CALL(*memoryMapper, read(0x0100, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeBMI));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x0101, ADDRESS_SPACE::CPU)).WillOnce(Return(-3));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x0102);
//     EXPECT_CALL(*memoryMapper, read(0x01FF, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x01FF);
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x00FF);
// }

// TEST_F(CPUTest, InstructionBNE){
//     byte_t opcodeBNE = 0b11010000;

//     // No branch
//     getPC() = 0x0;
//     getStatus() = 0b00000010;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeBNE));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x2));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x2);

//     // Branch. No page cross
//     getPC() = 0x0;
//     getStatus() = 0b00000000;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeBNE));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x2));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x2);
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x4);

//     // Branch. Page cross upwards
//     getPC() = 0xFD;
//     getStatus() = 0b00000000;
//     EXPECT_CALL(*memoryMapper, read(0xFD, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeBNE));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0xFE, ADDRESS_SPACE::CPU)).WillOnce(Return(0x1));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0xFF);
//     EXPECT_CALL(*memoryMapper, read(0x00, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x00);
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x0100);

//     // Branch. Page cross downwards
//     getPC() = 0x0100;
//     getStatus() = 0b00000000;
//     EXPECT_CALL(*memoryMapper, read(0x0100, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeBNE));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x0101, ADDRESS_SPACE::CPU)).WillOnce(Return(-3));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x0102);
//     EXPECT_CALL(*memoryMapper, read(0x01FF, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x01FF);
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x00FF);
// }

// TEST_F(CPUTest, InstructionBPL){
//     byte_t opcodeBPL = 0b00010000;

//     // No branch
//     getPC() = 0x0;
//     getStatus() = 0b10000000;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeBPL));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x2));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x2);

//     // Branch. No page cross
//     getPC() = 0x0;
//     getStatus() = 0b00000000;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeBPL));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x2));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x2);
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x4);

//     // Branch. Page cross upwards
//     getPC() = 0xFD;
//     getStatus() = 0b00000000;
//     EXPECT_CALL(*memoryMapper, read(0xFD, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeBPL));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0xFE, ADDRESS_SPACE::CPU)).WillOnce(Return(0x1));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0xFF);
//     EXPECT_CALL(*memoryMapper, read(0x00, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x00);
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x0100);

//     // Branch. Page cross downwards
//     getPC() = 0x0100;
//     getStatus() = 0b00000000;
//     EXPECT_CALL(*memoryMapper, read(0x0100, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeBPL));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x0101, ADDRESS_SPACE::CPU)).WillOnce(Return(-3));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x0102);
//     EXPECT_CALL(*memoryMapper, read(0x01FF, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x01FF);
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x00FF);
// }

// TEST_F(CPUTest, InstructionBVC){
//     byte_t opcodeBVC = 0b01010000;

//     // No branch
//     getPC() = 0x0;
//     getStatus() = 0b01000000;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeBVC));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x2));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x2);

//     // Branch. No page cross
//     getPC() = 0x0;
//     getStatus() = 0b00000000;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeBVC));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x2));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x2);
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x4);

//     // Branch. Page cross upwards
//     getPC() = 0xFD;
//     getStatus() = 0b00000000;
//     EXPECT_CALL(*memoryMapper, read(0xFD, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeBVC));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0xFE, ADDRESS_SPACE::CPU)).WillOnce(Return(0x1));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0xFF);
//     EXPECT_CALL(*memoryMapper, read(0x00, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x00);
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x0100);

//     // Branch. Page cross downwards
//     getPC() = 0x0100;
//     getStatus() = 0b00000000;
//     EXPECT_CALL(*memoryMapper, read(0x0100, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeBVC));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x0101, ADDRESS_SPACE::CPU)).WillOnce(Return(-3));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x0102);
//     EXPECT_CALL(*memoryMapper, read(0x01FF, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x01FF);
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x00FF);
// }

// TEST_F(CPUTest, InstructionBVS){
//     byte_t opcodeBVS = 0b01110000;

//     // No branch
//     getPC() = 0x0;
//     getStatus() = 0b00000000;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeBVS));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x2));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x2);

//     // Branch. No page cross
//     getPC() = 0x0;
//     getStatus() = 0b01000000;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeBVS));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x2));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x2);
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x4);

//     // Branch. Page cross upwards
//     getPC() = 0xFD;
//     getStatus() = 0b01000000;
//     EXPECT_CALL(*memoryMapper, read(0xFD, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeBVS));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0xFE, ADDRESS_SPACE::CPU)).WillOnce(Return(0x1));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0xFF);
//     EXPECT_CALL(*memoryMapper, read(0x00, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x00);
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x0100);

//     // Branch. Page cross downwards
//     getPC() = 0x0100;
//     getStatus() = 0b01000000;
//     EXPECT_CALL(*memoryMapper, read(0x0100, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeBVS));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x0101, ADDRESS_SPACE::CPU)).WillOnce(Return(-3));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x0102);
//     EXPECT_CALL(*memoryMapper, read(0x01FF, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x01FF);
//     cpu.clkstep();
//     EXPECT_EQ(getPC(), 0x00FF);
// }

// TEST_F(CPUTest, InstructionASLACC){
//     byte_t opcodeASLACC = 0b00001010;

//     // No Carry set
//     getPC() = 0x0;
//     getA() = 0b01000000;
//     getStatus() = 0b00000000;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeASLACC));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeASLACC));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0b10000000);
//     EXPECT_EQ(getStatus() & 0b00000000, 0b00000000);

//     // Carry set
//     getPC() = 0x0;
//     getA() = 0b11110000;
//     getStatus() = 0x0;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeASLACC));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeASLACC));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0b11100000);
//     EXPECT_EQ(getStatus() & 0b00000001, 0b00000001);
// }

// TEST_F(CPUTest, InstructionASLZeroPage){
//     getPC() = 0x0;
//     getStatus() = 0b00000000;
//     byte_t opcodeASLZP = 0b00000110;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeASLZP));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0b10000000));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0b10000000);
//     EXPECT_CALL(*memoryMapper, write(0x42, ADDRESS_SPACE::CPU, 0b10000000));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0b00000000);
//     EXPECT_CALL(*memoryMapper, write(0x42, ADDRESS_SPACE::CPU, 0b00000000));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b00000001, 0b00000001);
// }

// TEST_F(CPUTest, InstructionASLZeroPageX){
//     getPC() = 0x0;
//     getStatus() = 0b00000000;
//     getX() = 0x2;
//     byte_t opcodeASLZPX = 0b00010110;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeASLZPX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x40));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x40, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x42);
//     EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0b10000000));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0b10000000);
//     EXPECT_CALL(*memoryMapper, write(0x42, ADDRESS_SPACE::CPU, 0b10000000));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0b00000000);
//     EXPECT_CALL(*memoryMapper, write(0x42, ADDRESS_SPACE::CPU, 0b00000000));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b00000001, 0b00000001);
// }

// TEST_F(CPUTest, InstructionASLAbsolute){
//     getPC() = 0x0;
//     getStatus() = 0b00000000;
//     byte_t opcodeASLABS = 0b00001110;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeASLABS));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0b10000000));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0b10000000);
//     EXPECT_CALL(*memoryMapper, write(0xABCD, ADDRESS_SPACE::CPU, 0b10000000));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0b00000000);
//     EXPECT_CALL(*memoryMapper, write(0xABCD, ADDRESS_SPACE::CPU, 0b00000000));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b00000001, 0b00000001);
// }

// TEST_F(CPUTest, InstructionASLAbsoluteX){
//     byte_t opcodeASLABSX = 0b00011110;
    
//     // No page crossing
//     getPC() = 0x0;
//     getStatus() = 0b00000000;
//     getX() = 0x1;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeASLABSX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCC));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0b10000000));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0b10000000));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0b10000000);
//     EXPECT_CALL(*memoryMapper, write(0xABCD, ADDRESS_SPACE::CPU, 0b10000000));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0b00000000);
//     EXPECT_CALL(*memoryMapper, write(0xABCD, ADDRESS_SPACE::CPU, 0b00000000));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b00000001, 0b00000001);

//     // Page crossing
//     getPC() = 0x0;
//     getStatus() = 0b00000000;
//     getX() = 0x1;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeASLABSX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFF));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x0);
//     EXPECT_CALL(*memoryMapper, read(0xAB00, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0xAC00, ADDRESS_SPACE::CPU)).WillOnce(Return(0b10000000));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0b10000000);
//     EXPECT_CALL(*memoryMapper, write(0xAC00, ADDRESS_SPACE::CPU, 0b10000000));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0b00000000);
//     EXPECT_CALL(*memoryMapper, write(0xAC00, ADDRESS_SPACE::CPU, 0b00000000));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b00000001, 0b00000001);
// }

// TEST_F(CPUTest, InstructionLSRACC){
//     byte_t opcodeLSRACC = 0b01001010;

//     // No Carry set
//     getPC() = 0x0;
//     getA() = 0b01000000;
//     getStatus() = 0b00000000;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeLSRACC));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeLSRACC));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0b00100000);
//     EXPECT_EQ(getStatus() & 0b00000000, 0b00000000);

//     // Carry set
//     getPC() = 0x0;
//     getA() = 0b00001111;
//     getStatus() = 0x0;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeLSRACC));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeLSRACC));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0b00000111);
//     EXPECT_EQ(getStatus() & 0b00000001, 0b00000001);
// }

// TEST_F(CPUTest, InstructionLSRZeroPage){
//     getPC() = 0x0;
//     getStatus() = 0b00000000;
//     byte_t opcodeLSRZP = 0b01000110;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeLSRZP));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00000001));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0b00000001);
//     EXPECT_CALL(*memoryMapper, write(0x42, ADDRESS_SPACE::CPU, 0b00000001));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0b00000000);
//     EXPECT_CALL(*memoryMapper, write(0x42, ADDRESS_SPACE::CPU, 0b00000000));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b00000001, 0b00000001);
// }

// TEST_F(CPUTest, InstructionLSRZeroPageX){
//     getPC() = 0x0;
//     getStatus() = 0b00000000;
//     getX() = 0x2;
//     byte_t opcodeLSRZPX = 0b01010110;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeLSRZPX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x40));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x40, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x42);
//     EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00000001));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0b00000001);
//     EXPECT_CALL(*memoryMapper, write(0x42, ADDRESS_SPACE::CPU, 0b00000001));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0b00000000);
//     EXPECT_CALL(*memoryMapper, write(0x42, ADDRESS_SPACE::CPU, 0b00000000));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b00000001, 0b00000001);
// }

// TEST_F(CPUTest, InstructionLSRAbsolute){
//     getPC() = 0x0;
//     getStatus() = 0b00000000;
//     byte_t opcodeLSRABS = 0b01001110;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeLSRABS));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00000001));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0b00000001);
//     EXPECT_CALL(*memoryMapper, write(0xABCD, ADDRESS_SPACE::CPU, 0b00000001));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0b00000000);
//     EXPECT_CALL(*memoryMapper, write(0xABCD, ADDRESS_SPACE::CPU, 0b00000000));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b00000001, 0b00000001);
// }

// TEST_F(CPUTest, InstructionLSRAbsoluteX){
//     byte_t opcodeLSRABSX = 0b01011110;
    
//     // No page crossing
//     getPC() = 0x0;
//     getStatus() = 0b00000000;
//     getX() = 0x1;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeLSRABSX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCC));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00000001));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00000001));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0b00000001);
//     EXPECT_CALL(*memoryMapper, write(0xABCD, ADDRESS_SPACE::CPU, 0b00000001));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0b00000000);
//     EXPECT_CALL(*memoryMapper, write(0xABCD, ADDRESS_SPACE::CPU, 0b00000000));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b00000001, 0b00000001);

//     // Page crossing
//     getPC() = 0x0;
//     getStatus() = 0b00000000;
//     getX() = 0x1;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeLSRABSX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFF));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x0);
//     EXPECT_CALL(*memoryMapper, read(0xAB00, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0xAC00, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00000001));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0b00000001);
//     EXPECT_CALL(*memoryMapper, write(0xAC00, ADDRESS_SPACE::CPU, 0b00000001));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0b00000000);
//     EXPECT_CALL(*memoryMapper, write(0xAC00, ADDRESS_SPACE::CPU, 0b00000000));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b00000001, 0b00000001);
// }

// TEST_F(CPUTest, InstructionROLACC){
//     byte_t opcodeROLACC = 0b00101010;

//     // LSB from carry
//     getPC() = 0x0;
//     getA() = 0b00000000;
//     getStatus() = 0b00000001;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeROLACC));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeROLACC));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0b00000001);
//     EXPECT_EQ(getStatus() & 0b00000000, 0b00000000);

//     // MSB to carry
//     getPC() = 0x0;
//     getA() = 0b11110000;
//     getStatus() = 0b00000000;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeROLACC));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeROLACC));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0b11100000);
//     EXPECT_EQ(getStatus() & 0b00000001, 0b00000001);

//     // No carry
//     getPC() = 0x0;
//     getA() = 0b00011000;
//     getStatus() = 0b00000000;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeROLACC));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeROLACC));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0b00110000);
//     EXPECT_EQ(getStatus() & 0b00000001, 0b00000000);
// }

// TEST_F(CPUTest, InstructionROLZeroPage){
//     getPC() = 0x0;
//     getStatus() = 0b00000001;
//     byte_t opcodeROLZP = 0b00100110;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeROLZP));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0b10000000));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0b10000000);
//     EXPECT_CALL(*memoryMapper, write(0x42, ADDRESS_SPACE::CPU, 0b10000000));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0b00000001);
//     EXPECT_CALL(*memoryMapper, write(0x42, ADDRESS_SPACE::CPU, 0b00000001));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b00000001, 0b00000001);
// }

// TEST_F(CPUTest, InstructionROLZeroPageX){
//     getPC() = 0x0;
//     getStatus() = 0b00000001;
//     getX() = 0x2;
//     byte_t opcodeROLZPX = 0b00110110;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeROLZPX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x40));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x40, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x42);
//     EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0b10000000));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0b10000000);
//     EXPECT_CALL(*memoryMapper, write(0x42, ADDRESS_SPACE::CPU, 0b10000000));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0b00000001);
//     EXPECT_CALL(*memoryMapper, write(0x42, ADDRESS_SPACE::CPU, 0b00000001));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b00000001, 0b00000001);
// }

// TEST_F(CPUTest, InstructionROLAbsolute){
//     getPC() = 0x0;
//     getStatus() = 0b00000001;
//     byte_t opcodeROLABS = 0b00101110;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeROLABS));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0b10000000));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0b10000000);
//     EXPECT_CALL(*memoryMapper, write(0xABCD, ADDRESS_SPACE::CPU, 0b10000000));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0b00000001);
//     EXPECT_CALL(*memoryMapper, write(0xABCD, ADDRESS_SPACE::CPU, 0b00000001));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b00000001, 0b00000001);
// }

// TEST_F(CPUTest, InstructionROLAbsoluteX){
//     byte_t opcodeROLABSX = 0b00111110;
    
//     // No page crossing
//     getPC() = 0x0;
//     getStatus() = 0b00000001;
//     getX() = 0x1;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeROLABSX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCC));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0b10000000));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0b10000000));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0b10000000);
//     EXPECT_CALL(*memoryMapper, write(0xABCD, ADDRESS_SPACE::CPU, 0b10000000));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0b00000001);
//     EXPECT_CALL(*memoryMapper, write(0xABCD, ADDRESS_SPACE::CPU, 0b00000001));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b00000001, 0b00000001);

//     // Page crossing
//     getPC() = 0x0;
//     getStatus() = 0b00000001;
//     getX() = 0x1;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeROLABSX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFF));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x0);
//     EXPECT_CALL(*memoryMapper, read(0xAB00, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0xAC00, ADDRESS_SPACE::CPU)).WillOnce(Return(0b10000000));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0b10000000);
//     EXPECT_CALL(*memoryMapper, write(0xAC00, ADDRESS_SPACE::CPU, 0b10000000));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0b00000001);
//     EXPECT_CALL(*memoryMapper, write(0xAC00, ADDRESS_SPACE::CPU, 0b00000001));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b00000001, 0b00000001);
// }

// TEST_F(CPUTest, InstructionRORACC){
//     byte_t opcodeRORACC = 0b01101010;

//     // MSB from carry
//     getPC() = 0x0;
//     getA() = 0b00000000;
//     getStatus() = 0b00000001;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeRORACC));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeRORACC));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0b10000000);
//     EXPECT_EQ(getStatus() & 0b00000000, 0b00000000);

//     // LSB to carry
//     getPC() = 0x0;
//     getA() = 0b00001111;
//     getStatus() = 0b00000000;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeRORACC));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeRORACC));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0b00000111);
//     EXPECT_EQ(getStatus() & 0b00000001, 0b00000001);

//     // No carry
//     getPC() = 0x0;
//     getA() = 0b00011000;
//     getStatus() = 0b00000000;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeRORACC));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeRORACC));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0b00001100);
//     EXPECT_EQ(getStatus() & 0b00000001, 0b00000000);
// }

// TEST_F(CPUTest, InstructionRORZeroPage){
//     getPC() = 0x0;
//     getStatus() = 0b00000001;
//     byte_t opcodeRORZP = 0b01100110;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeRORZP));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00000001));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0b00000001);
//     EXPECT_CALL(*memoryMapper, write(0x42, ADDRESS_SPACE::CPU, 0b00000001));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0b10000000);
//     EXPECT_CALL(*memoryMapper, write(0x42, ADDRESS_SPACE::CPU, 0b10000000));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b00000001, 0b00000001);
// }

// TEST_F(CPUTest, InstructionRORZeroPageX){
//     getPC() = 0x0;
//     getStatus() = 0b00000001;
//     getX() = 0x2;
//     byte_t opcodeRORZPX = 0b01110110;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeRORZPX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x40));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x40, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x42);
//     EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00000001));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0b00000001);
//     EXPECT_CALL(*memoryMapper, write(0x42, ADDRESS_SPACE::CPU, 0b00000001));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0b10000000);
//     EXPECT_CALL(*memoryMapper, write(0x42, ADDRESS_SPACE::CPU, 0b10000000));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b00000001, 0b00000001);
// }

// TEST_F(CPUTest, InstructionRORAbsolute){
//     getPC() = 0x0;
//     getStatus() = 0b00000001;
//     byte_t opcodeRORABS = 0b01101110;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeRORABS));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00000001));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0b00000001);
//     EXPECT_CALL(*memoryMapper, write(0xABCD, ADDRESS_SPACE::CPU, 0b00000001));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0b10000000);
//     EXPECT_CALL(*memoryMapper, write(0xABCD, ADDRESS_SPACE::CPU, 0b10000000));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b00000001, 0b00000001);
// }

// TEST_F(CPUTest, InstructionRORAbsoluteX){
//     byte_t opcodeRORABSX = 0b01111110;
    
//     // No page crossing
//     getPC() = 0x0;
//     getStatus() = 0b00000001;
//     getX() = 0x1;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeRORABSX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCC));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00000001));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00000001));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0b00000001);
//     EXPECT_CALL(*memoryMapper, write(0xABCD, ADDRESS_SPACE::CPU, 0b00000001));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0b10000000);
//     EXPECT_CALL(*memoryMapper, write(0xABCD, ADDRESS_SPACE::CPU, 0b10000000));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b00000001, 0b00000001);

//     // Page crossing
//     getPC() = 0x0;
//     getStatus() = 0b00000001;
//     getX() = 0x1;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeRORABSX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFF));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x0);
//     EXPECT_CALL(*memoryMapper, read(0xAB00, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0xAC00, ADDRESS_SPACE::CPU)).WillOnce(Return(0b00000001));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0b00000001);
//     EXPECT_CALL(*memoryMapper, write(0xAC00, ADDRESS_SPACE::CPU, 0b00000001));
//     cpu.clkstep();
//     EXPECT_EQ(getIDL(), 0b10000000);
//     EXPECT_CALL(*memoryMapper, write(0xAC00, ADDRESS_SPACE::CPU, 0b10000000));
//     cpu.clkstep();
//     EXPECT_EQ(getStatus() & 0b00000001, 0b00000001);
// }

// TEST_F(CPUTest, InstructionADCImmediate){
//     byte_t opcodeADCIM = 0b01101001;

//     // No overflow, no carry
//     getPC() = 0x0;
//     getA() = 0xF0;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeADCIM));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x0F));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0xFF);

//     // Overflow after addition
//     getPC() = 0x0;
//     getA() = 0x7F;
//     getStatus() = 0b00000000;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeADCIM));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x01));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0x80);
//     EXPECT_EQ(getStatus(), 0b11000000);

//     // Carry before addition
//     getPC() = 0x0;
//     getA() = 0xF0;
//     getStatus() = 0b00000001;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeADCIM));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x0E));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0xFF);
//     EXPECT_EQ(getStatus(), 0b10000000);
// }

// TEST_F(CPUTest, InstructionADCZeroPage){
//     byte_t opcodeADCZP = 0b01100101;
//     getPC() = 0x0;
//     getA() = 0xF0;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeADCZP));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0x0F));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0xFF);
// }

// TEST_F(CPUTest, InstructionADCZeroPageX){
//     byte_t opcodeADCZPX = 0b01110101;
//     // No overflow on AddrL + X
//     getPC() = 0x0;
//     getA() = 0xF0;
//     getX() = 0x10;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeADCZPX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x52);
//     EXPECT_CALL(*memoryMapper, read(0x52, ADDRESS_SPACE::CPU)).WillOnce(Return(0x0F));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0xFF);

//     // Overflow on AddrL + X
//     getPC() = 0x0;
//     getA() = 0xF0;
//     getX() = 0xFF;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeADCZPX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x1));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x00);
//     EXPECT_CALL(*memoryMapper, read(0x00, ADDRESS_SPACE::CPU)).WillOnce(Return(0x0F));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0xFF);
// }

// TEST_F(CPUTest, InstructionADCAbsolute){
//     byte_t opcodeADCABS = 0b01101101;
//     getPC() = 0x0;
//     getA() = 0xF0;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeADCABS));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x0F));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0xFF);
// }

// TEST_F(CPUTest, InstructionADCAbsoluteX){
//     byte_t opcodeADCABSX = 0b01111101;
//     // No "oops" cycle (addrL + X does not cross a page)
//     getPC() = 0x0;
//     getA() = 0xF0;
//     getX() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeADCABSX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCC));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCC);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_EQ(getIAL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x0F));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0xFF);

//     // "oops" cycle (addrL + X does cross a page)
//     getPC() = 0x0;
//     getA() = 0xF0;
//     getX() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeADCABSX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFF));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xFF);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFE));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xFE);
//     EXPECT_EQ(getIAL(), 0x00);
//     EXPECT_CALL(*memoryMapper, read(0xFE00, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xFF);
//     EXPECT_EQ(getIAL(), 0x00);
//     EXPECT_CALL(*memoryMapper, read(0xFF00, ADDRESS_SPACE::CPU)).WillOnce(Return(0x0F));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0xFF);
// }

// TEST_F(CPUTest, InstructionADCAbsoluteY){
//     byte_t opcodeADCABSY = 0b01111001;
//     // No "oops" cycle (addrL + Y does not cross a page)
//     getPC() = 0x0;
//     getA() = 0xF0;
//     getY() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeADCABSY));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCC));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCC);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_EQ(getIAL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x0F));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0xFF);

//     // "oops" cycle (addrL + Y does cross a page)
//     getPC() = 0x0;
//     getA() = 0xF0;
//     getY() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeADCABSY));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFF));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xFF);
//     EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFE));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xFE);
//     EXPECT_EQ(getIAL(), 0x00);
//     EXPECT_CALL(*memoryMapper, read(0xFE00, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xFF);
//     EXPECT_EQ(getIAL(), 0x00);
//     EXPECT_CALL(*memoryMapper, read(0xFF00, ADDRESS_SPACE::CPU)).WillOnce(Return(0x0F));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0xFF);
// }

// TEST_F(CPUTest, InstructionADCIndirectX){
//     byte_t opcodeADCINDX = 0b01100001;
//     // No page crossing on (pointer + X)
//     getPC() = 0x0;
//     getA() = 0xF0;
//     getX() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeADCINDX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0x42);
//     EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x43);
//     EXPECT_CALL(*memoryMapper, read(0x0043, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0x0044, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x0F));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0xFF);

//     // Page crossing on (pointer + X)
//     getPC() = 0x0;
//     getA() = 0xF0;
//     getX() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeADCINDX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFF));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xFF);
//     EXPECT_CALL(*memoryMapper, read(0xFF, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x00);
//     EXPECT_CALL(*memoryMapper, read(0x0000, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0x0001, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x0F));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0xFF);

//     // Page crossing on (pointer + X + 1)
//     getPC() = 0x0;
//     getA() = 0xF0;
//     getX() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeADCINDX));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFE));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xFE);
//     EXPECT_CALL(*memoryMapper, read(0xFE, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0xFF);
//     EXPECT_CALL(*memoryMapper, read(0x00FF, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0x0000, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x0F));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0xFF);
// }

// TEST_F(CPUTest, InstructionADCIndirectY){
//     byte_t opcodeADCINDY = 0b01110001;
//     // No page crossing on (pointer + Y)
//     getPC() = 0x0;
//     getA() = 0xF0;
//     getY() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeADCINDY));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0x42);
//     EXPECT_CALL(*memoryMapper, read(0x0042, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCC));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x43);
//     EXPECT_EQ(getAddrL(), 0xCC);
//     EXPECT_CALL(*memoryMapper, read(0x0043, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_EQ(getAddrL(), 0xCD);
//     EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x0F));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0xFF);

//     // Page crossing on (pointer + Y)
//     getPC() = 0x0;
//     getA() = 0xF0;
//     getY() = 0x01;
//     EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeADCINDY));
//     cpu.clkstep();
//     EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrL(), 0x42);
//     EXPECT_CALL(*memoryMapper, read(0x0042, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFF));
//     cpu.clkstep();
//     EXPECT_EQ(getIAL(), 0x43);
//     EXPECT_EQ(getAddrL(), 0xFF);
//     EXPECT_CALL(*memoryMapper, read(0x0043, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAB);
//     EXPECT_EQ(getAddrL(), 0x00);
//     EXPECT_CALL(*memoryMapper, read(0xAB00, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
//     cpu.clkstep();
//     EXPECT_EQ(getAddrH(), 0xAC);
//     EXPECT_CALL(*memoryMapper, read(0xAC00, ADDRESS_SPACE::CPU)).WillOnce(Return(0x0F));
//     cpu.clkstep();
//     EXPECT_EQ(getA(), 0xFF);
// }

TEST_F(CPUTest, InstructionSBCImmediate){
    byte_t opcodeSBCIM = 0b11101001;

    // No overflow, no carry
    getPC() = 0x0;
    getA() = 0xFF;
    getStatus() = 0b00000001;
    EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeSBCIM));
    cpu.clkstep();
    EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x0F));
    cpu.clkstep();
    EXPECT_EQ(getA(), 0xF0);

    // Overflow after subtraction
    getPC() = 0x0;
    getA() = 0x00;
    getStatus() = 0b00000001;
    EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeSBCIM));
    cpu.clkstep();
    EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x80));
    cpu.clkstep();
    EXPECT_EQ(getA(), 0x80);
    EXPECT_EQ(getStatus(), 0b11000000);

    // Carry before and after subtraction
    getPC() = 0x0;
    getA() = 0xFF;
    getStatus() = 0b00000000;
    EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeSBCIM));
    cpu.clkstep();
    EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x0E));
    cpu.clkstep();
    EXPECT_EQ(getA(), 0xF0);
    EXPECT_EQ(getStatus(), 0b10000001);
}

TEST_F(CPUTest, InstructionSBCZeroPage){
    byte_t opcodeSBCZP = 0b11100101;
    getPC() = 0x0;
    getA() = 0xFF;
    getStatus() = 0b00000001;
    EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeSBCZP));
    cpu.clkstep();
    EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
    cpu.clkstep();
    EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0x0F));
    cpu.clkstep();
    EXPECT_EQ(getA(), 0xF0);
}

TEST_F(CPUTest, InstructionSBCZeroPageX){
    byte_t opcodeSBCZPX = 0b11110101;
    // No overflow on AddrL + X
    getPC() = 0x0;
    getA() = 0xFF;
    getStatus() = 0b00000001;
    getX() = 0x10;
    EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeSBCZPX));
    cpu.clkstep();
    EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
    cpu.clkstep();
    EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
    cpu.clkstep();
    EXPECT_EQ(getIAL(), 0x52);
    EXPECT_CALL(*memoryMapper, read(0x52, ADDRESS_SPACE::CPU)).WillOnce(Return(0x0F));
    cpu.clkstep();
    EXPECT_EQ(getA(), 0xF0);

    // Overflow on AddrL + X
    getPC() = 0x0;
    getA() = 0xFF;
    getStatus() = 0b00000001;
    getX() = 0xFF;
    EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeSBCZPX));
    cpu.clkstep();
    EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x1));
    cpu.clkstep();
    EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
    cpu.clkstep();
    EXPECT_EQ(getIAL(), 0x00);
    EXPECT_CALL(*memoryMapper, read(0x00, ADDRESS_SPACE::CPU)).WillOnce(Return(0x0F));
    cpu.clkstep();
    EXPECT_EQ(getA(), 0xF0);
}

TEST_F(CPUTest, InstructionSBCAbsolute){
    byte_t opcodeSBCABS = 0b11101101;
    getPC() = 0x0;
    getA() = 0xFF;
    getStatus() = 0b00000001;
    EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeSBCABS));
    cpu.clkstep();
    EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
    cpu.clkstep();
    EXPECT_EQ(getAddrL(), 0xCD);
    EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
    cpu.clkstep();
    EXPECT_EQ(getAddrH(), 0xAB);
    EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x0F));
    cpu.clkstep();
    EXPECT_EQ(getA(), 0xF0);
}

TEST_F(CPUTest, InstructionSBCAbsoluteX){
    byte_t opcodeSBCABSX = 0b11111101;
    // No "oops" cycle (addrL + X does not cross a page)
    getPC() = 0x0;
    getA() = 0xFF;
    getStatus() = 0b00000001;
    getX() = 0x01;
    EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeSBCABSX));
    cpu.clkstep();
    EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCC));
    cpu.clkstep();
    EXPECT_EQ(getAddrL(), 0xCC);
    EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
    cpu.clkstep();
    EXPECT_EQ(getAddrH(), 0xAB);
    EXPECT_EQ(getIAL(), 0xCD);
    EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x0F));
    cpu.clkstep();
    EXPECT_EQ(getA(), 0xF0);

    // "oops" cycle (addrL + X does cross a page)
    getPC() = 0x0;
    getA() = 0xFF;
    getStatus() = 0b00000001;
    getX() = 0x01;
    EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeSBCABSX));
    cpu.clkstep();
    EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFF));
    cpu.clkstep();
    EXPECT_EQ(getAddrL(), 0xFF);
    EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFE));
    cpu.clkstep();
    EXPECT_EQ(getAddrH(), 0xFE);
    EXPECT_EQ(getIAL(), 0x00);
    EXPECT_CALL(*memoryMapper, read(0xFE00, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
    cpu.clkstep();
    EXPECT_EQ(getAddrH(), 0xFF);
    EXPECT_EQ(getIAL(), 0x00);
    EXPECT_CALL(*memoryMapper, read(0xFF00, ADDRESS_SPACE::CPU)).WillOnce(Return(0x0F));
    cpu.clkstep();
    EXPECT_EQ(getA(), 0xF0);
}

TEST_F(CPUTest, InstructionSBCAbsoluteY){
    byte_t opcodeSBCABSY = 0b11111001;
    // No "oops" cycle (addrL + Y does not cross a page)
    getPC() = 0x0;
    getA() = 0xFF;
    getStatus() = 0b00000001;
    getY() = 0x01;
    EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeSBCABSY));
    cpu.clkstep();
    EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCC));
    cpu.clkstep();
    EXPECT_EQ(getAddrL(), 0xCC);
    EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
    cpu.clkstep();
    EXPECT_EQ(getAddrH(), 0xAB);
    EXPECT_EQ(getIAL(), 0xCD);
    EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x0F));
    cpu.clkstep();
    EXPECT_EQ(getA(), 0xF0);

    // "oops" cycle (addrL + Y does cross a page)
    getPC() = 0x0;
    getA() = 0xFF;
    getStatus() = 0b00000001;
    getY() = 0x01;
    EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeSBCABSY));
    cpu.clkstep();
    EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFF));
    cpu.clkstep();
    EXPECT_EQ(getAddrL(), 0xFF);
    EXPECT_CALL(*memoryMapper, read(0x2, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFE));
    cpu.clkstep();
    EXPECT_EQ(getAddrH(), 0xFE);
    EXPECT_EQ(getIAL(), 0x00);
    EXPECT_CALL(*memoryMapper, read(0xFE00, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
    cpu.clkstep();
    EXPECT_EQ(getAddrH(), 0xFF);
    EXPECT_EQ(getIAL(), 0x00);
    EXPECT_CALL(*memoryMapper, read(0xFF00, ADDRESS_SPACE::CPU)).WillOnce(Return(0x0F));
    cpu.clkstep();
    EXPECT_EQ(getA(), 0xF0);
}

TEST_F(CPUTest, InstructionSBCIndirectX){
    byte_t opcodeSBCINDX = 0b11100001;
    // No page crossing on (pointer + X)
    getPC() = 0x0;
    getA() = 0xFF;
    getStatus() = 0b00000001;
    getX() = 0x01;
    EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeSBCINDX));
    cpu.clkstep();
    EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
    cpu.clkstep();
    EXPECT_EQ(getAddrL(), 0x42);
    EXPECT_CALL(*memoryMapper, read(0x42, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
    cpu.clkstep();
    EXPECT_EQ(getIAL(), 0x43);
    EXPECT_CALL(*memoryMapper, read(0x0043, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
    cpu.clkstep();
    EXPECT_EQ(getAddrL(), 0xCD);
    EXPECT_CALL(*memoryMapper, read(0x0044, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
    cpu.clkstep();
    EXPECT_EQ(getAddrH(), 0xAB);
    EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x0F));
    cpu.clkstep();
    EXPECT_EQ(getA(), 0xF0);

    // Page crossing on (pointer + X)
    getPC() = 0x0;
    getA() = 0xFF;
    getStatus() = 0b00000001;
    getX() = 0x01;
    EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeSBCINDX));
    cpu.clkstep();
    EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFF));
    cpu.clkstep();
    EXPECT_EQ(getAddrL(), 0xFF);
    EXPECT_CALL(*memoryMapper, read(0xFF, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
    cpu.clkstep();
    EXPECT_EQ(getIAL(), 0x00);
    EXPECT_CALL(*memoryMapper, read(0x0000, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
    cpu.clkstep();
    EXPECT_EQ(getAddrL(), 0xCD);
    EXPECT_CALL(*memoryMapper, read(0x0001, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
    cpu.clkstep();
    EXPECT_EQ(getAddrH(), 0xAB);
    EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x0F));
    cpu.clkstep();
    EXPECT_EQ(getA(), 0xF0);

    // Page crossing on (pointer + X + 1)
    getPC() = 0x0;
    getA() = 0xFF;
    getStatus() = 0b00000001;
    getX() = 0x01;
    EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeSBCINDX));
    cpu.clkstep();
    EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFE));
    cpu.clkstep();
    EXPECT_EQ(getAddrL(), 0xFE);
    EXPECT_CALL(*memoryMapper, read(0xFE, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
    cpu.clkstep();
    EXPECT_EQ(getIAL(), 0xFF);
    EXPECT_CALL(*memoryMapper, read(0x00FF, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCD));
    cpu.clkstep();
    EXPECT_EQ(getAddrL(), 0xCD);
    EXPECT_CALL(*memoryMapper, read(0x0000, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
    cpu.clkstep();
    EXPECT_EQ(getAddrH(), 0xAB);
    EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x0F));
    cpu.clkstep();
    EXPECT_EQ(getA(), 0xF0);
}

TEST_F(CPUTest, InstructionSBCIndirectY){
    byte_t opcodeSBCINDY = 0b11110001;
    // No page crossing on (pointer + Y)
    getPC() = 0x0;
    getA() = 0xFF;
    getStatus() = 0b00000001;
    getY() = 0x01;
    EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeSBCINDY));
    cpu.clkstep();
    EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
    cpu.clkstep();
    EXPECT_EQ(getAddrL(), 0x42);
    EXPECT_CALL(*memoryMapper, read(0x0042, ADDRESS_SPACE::CPU)).WillOnce(Return(0xCC));
    cpu.clkstep();
    EXPECT_EQ(getIAL(), 0x43);
    EXPECT_EQ(getAddrL(), 0xCC);
    EXPECT_CALL(*memoryMapper, read(0x0043, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
    cpu.clkstep();
    EXPECT_EQ(getAddrH(), 0xAB);
    EXPECT_EQ(getAddrL(), 0xCD);
    EXPECT_CALL(*memoryMapper, read(0xABCD, ADDRESS_SPACE::CPU)).WillOnce(Return(0x0F));
    cpu.clkstep();
    EXPECT_EQ(getA(), 0xF0);

    // Page crossing on (pointer + Y)
    getPC() = 0x0;
    getA() = 0xFF;
    getStatus() = 0b00000001;
    getY() = 0x01;
    EXPECT_CALL(*memoryMapper, read(0x0, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeSBCINDY));
    cpu.clkstep();
    EXPECT_CALL(*memoryMapper, read(0x1, ADDRESS_SPACE::CPU)).WillOnce(Return(0x42));
    cpu.clkstep();
    EXPECT_EQ(getAddrL(), 0x42);
    EXPECT_CALL(*memoryMapper, read(0x0042, ADDRESS_SPACE::CPU)).WillOnce(Return(0xFF));
    cpu.clkstep();
    EXPECT_EQ(getIAL(), 0x43);
    EXPECT_EQ(getAddrL(), 0xFF);
    EXPECT_CALL(*memoryMapper, read(0x0043, ADDRESS_SPACE::CPU)).WillOnce(Return(0xAB));
    cpu.clkstep();
    EXPECT_EQ(getAddrH(), 0xAB);
    EXPECT_EQ(getAddrL(), 0x00);
    EXPECT_CALL(*memoryMapper, read(0xAB00, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
    cpu.clkstep();
    EXPECT_EQ(getAddrH(), 0xAC);
    EXPECT_CALL(*memoryMapper, read(0xAC00, ADDRESS_SPACE::CPU)).WillOnce(Return(0x0F));
    cpu.clkstep();
    EXPECT_EQ(getA(), 0xF0);
}

TEST_F(CPUTest, InstructionBRK){
    byte_t opcodeBRK = 0b00000000;
    
    getPC() = 0x4200;
    getSP() = 0xFF;
    getStatus() = 0b11111011;
    EXPECT_CALL(*memoryMapper, read(0x4200, ADDRESS_SPACE::CPU)).WillOnce(Return(opcodeBRK));
    cpu.clkstep();
    EXPECT_CALL(*memoryMapper, read(0x4201, ADDRESS_SPACE::CPU)).WillOnce(Return(0xF0));
    cpu.clkstep();
    EXPECT_EQ(getPC(), 0x4202);
    EXPECT_CALL(*memoryMapper, write(0x1FF, ADDRESS_SPACE::CPU, 0x42));
    cpu.clkstep();
    EXPECT_EQ(getSP(), 0xFE);
    EXPECT_CALL(*memoryMapper, write(0x1FE, ADDRESS_SPACE::CPU, 0x02));
    cpu.clkstep();
    EXPECT_EQ(getSP(), 0xFD);
    EXPECT_CALL(*memoryMapper, write(0x1FD, ADDRESS_SPACE::CPU, 0b11111011));
    cpu.clkstep();
    EXPECT_EQ(getSP(), 0xFC);
    EXPECT_EQ(getStatus(), 0b11111111);
    EXPECT_CALL(*memoryMapper, read(0xFFFE, ADDRESS_SPACE::CPU)).WillOnce(Return(0x34));
    cpu.clkstep();
    EXPECT_EQ(getPC() & 0x00FF, 0x34);
    EXPECT_CALL(*memoryMapper, read(0xFFFF, ADDRESS_SPACE::CPU)).WillOnce(Return(0x12));
    cpu.clkstep();
    EXPECT_EQ(getPC() & 0xFF00, 0x1200);
}


