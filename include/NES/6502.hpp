#pragma once
#include <vector>
#include "commons.hpp"
#include "NES/MemoryMapper.hpp"

class CPU6502{  
    // For unit tests
    friend class CPUTest;

    public:
        using MicroOp = void (CPU6502::*)();

        struct Instruction{
            const char* name;
            const char* mnemonic;
            std::vector<MicroOp> cycles;
        };

        // Initializes all registers in standard powerup state
        CPU6502(sptr<IMemoryMapper> memoryMapper);

        // Executes a single microinstruction
        void clkstep();

    private:
        // Registers
        byte_t X; // Register X.
        byte_t Y; // Register Y.
        byte_t A; // Register A/ACC/Accumulator.
        byte_t IR; // Instruction Register.
        byte_t status; // Status register. flags: NV1BDIZC
        byte_t SP; // Stack Pointer.
        addr_t PC; // Program Counter
        
        byte_t addrL = 0; // Address register, low byte
        byte_t addrH = 0; // Address register, high byte

        byte_t IAL = 0; // Input address latch
        byte_t IDL = 0; // Input data latch

        // For some isntructions, we need to know 1 cycle ahead of time if a page was crossed
        enum class PAGE_CROSS_STATUS{
            NO_CROSS,
            CROSS_UP,
            CROSS_DOWN
        };

        // Auxiliary flag for some instructions
        PAGE_CROSS_STATUS pageCrossed = PAGE_CROSS_STATUS::NO_CROSS; 
        
        sptr<IMemoryMapper> memory; // Memory interface. CPU will real/write to here.
        
        byte_t opcode = 0; // The opcode for the current instruction
        Instruction* currentInstruction = nullptr; // Current executing instruction
        uint8_t currentInstructionStep = 0; // Current step of current instruction
        
        // Auxiliary functions to help implement instruction behavior
        addr_t getAddr() const { return (addrH << 8) | addrL; } // Returns whole address register
        byte_t getFlagC() const { return status & 0b00000001; }
        byte_t getFlagZ() const { return status & 0b00000010; }
        byte_t getFlagI() const { return status & 0b00000100; }
        byte_t getFlagD() const { return status & 0b00001000; }
        byte_t getFlagV() const { return status & 0b01000000; }
        byte_t getFlagN() const { return status & 0b10000000; }

        void setFlagC() { status |= 0b00000001; }
        void setFlagZ() { status |= 0b00000010; }
        void setFlagI() { status |= 0b00000100; }
        void setFlagD() { status |= 0b00001000; }
        void setFlagV() { status |= 0b01000000; }
        void setFlagN() { status |= 0b10000000; }

        void clearFlagC() { status &= 0b11111110; }
        void clearFlagZ() { status &= 0b11111101; }
        void clearFlagI() { status &= 0b11111011; }
        void clearFlagD() { status &= 0b11110111; }
        void clearFlagV() { status &= 0b10111111; }
        void clearFlagN() { status &= 0b01111111; }

        // Reads a single byte from address `addr`.
        byte_t read(addr_t addr) const;

        // Writes a single byte to address `addr`.
        void write(addr_t addr, byte_t data) const;

        void updateFlagZ(byte_t val) { if(val == 0) setFlagZ(); }
        void updateFlagN(byte_t val) { if(val & 0b10000000) setFlagN(); }

        // Sets the low byte of PC to the given value
        void setPCL(byte_t v){
            PC &= 0xFF00;
            PC |= v;
        }

        // Sets the high byte of PC to the given value
        void setPCH(byte_t v){
            PC &= 0x00FF;
            PC |= (v<<8);
        }

        // Returns the appropriate instruction for the given opcode
        Instruction* decodeInstruction(byte_t opcode);

        ////////////////////////////////////////////////////////////////////////
        // Micro Instructions. Anything that can happen in a single clk cycle //
        ////////////////////////////////////////////////////////////////////////
        void fetchAddrL();
        void fetchAddrLFromZeroPageIAL();
        void fetchAddrHAddXToAddrLAndSkipNextStepIfPageCrossed();
        void fetchAddrHAddYToAddrLAndSkipNextStepIfPageCrossed();
        void fetchAddrHFromZeroPageIALAddYToAddrLAndSkipNextStepIfPageCrossed();
        void fetchAddrHAddXToAddrLAndCheckPageCross();
        void fetchAddrHAddYToAddrLAndCheckPageCross();
        void fetchAddrHFromZeroPageIALAddYToAddrLAndCheckPageCross();
        void fetchAddrH();
        void fetchAddrHFromZeroPageIAL();

        void loadAImmediate();
        void loadAZeroPage();
        void loadAAbsolute();

        void loadXImmediate();
        void loadXZeroPage();
        void loadXZeroPageY();
        void loadXAbsolute();

        void loadYImmediate();
        void loadYZeroPage();
        void loadYZeroPageX();
        void loadYAbsolute();

        void loadIDLZeroPage();
        void loadIDLAbsolute();

        void storeAZeroPage();
        void storeAAbsolute();

        void storeXZeroPage();
        void storeXAbsolute();

        void storeYZeroPage();
        void storeYAbsolute();

        void storeIDLZeroPage();
        void storeIDLAbsolute();
        
        void addXToAddrL();
        void addYToAddrL();
        void fixAddrHighByte();

        void transferAX();
        void transferAY();
        void transferXA();
        void transferYA();
        void transferSX();
        void transferXS();

        void nop();
        void unknown();
        void incrementPC();
        void readNextInstructionAndIncrementPC();

        void pushA();
        void pullA();
        void pushStatus();
        void pushStatusAndSetI();
        void pullStatus();
        void pullStatusAndIncrementSP();
        void pushPCH();
        void pullPCH();
        void pushPCL();
        void pullPCL();
        void incrementSP();

        void clearC();
        void clearD();
        void clearI();
        void clearV();
        void setC();
        void setD();
        void setI();
        
        void incrementX();
        void incrementY();
        void incrementIDL();
        void decrementX();
        void decrementY();
        void decrementIDL();

        void andAImmediate();
        void andAZeroPage();
        void andAAbsolute();

        void orAImmediate();
        void orAZeroPage();
        void orAAbsolute();

        void xorAImmediate();
        void xorAZeroPage();
        void xorAAbsolute();

        void bitTestZeroPage();
        void bitTestAbsolute();

        void compareAImmediate();
        void compareAZeroPage();
        void compareAAbsolute();

        void compareXImmediate();
        void compareXZeroPage();
        void compareXAbsolute();

        void compareYImmediate();
        void compareYZeroPage();
        void compareYAbsolute();

        void jmpAbsolute();
        void fetchPCLToIDL();
        void jmpIndirect();

        void branch();
        void fixPCHighByte();
        void checkBCCCondition();
        void checkBCSCondition();
        void checkBEQCondition();
        void checkBMICondition();
        void checkBNECondition();
        void checkBPLCondition();
        void checkBVCCondition();
        void checkBVSCondition();

        void aslAccumulator();
        void aslIDL();
        void lsrAccumulator();
        void lsrIDL();
        void rolAccumulator();
        void rolIDL();
        void rorAccumulator();
        void rorIDL();

        void adcAImmediate();
        void adcAZeroPage();
        void adcAAbsolute();

        void sbcAImmediate();
        void sbcAZeroPage();
        void sbcAAbsolute();

        void fetchIRQL();
        void fetchIRQH();
        void fetchNMIL();
        void fetchNMIH();

        //////////////////
        // Instructions //
        //////////////////

        Instruction LDAIM = {
            .name = "Load A (immediate)",
            .mnemonic = "LDA",
            .cycles = std::vector<MicroOp>{
                &CPU6502::loadAImmediate
            }
        };

        Instruction LDAZP = {
            .name = "Load A (zero page)",
            .mnemonic = "LDA",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::loadAZeroPage
            }
        };

        Instruction LDAZPX = {
            .name = "Load A (zero page, X)",
            .mnemonic = "LDA",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::addXToAddrL,
                &CPU6502::loadAZeroPage
            }
        };

        Instruction LDAABS = {
            .name = "Load A (absolute)",
            .mnemonic = "LDA",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrH,
                &CPU6502::loadAAbsolute
            }
        };

        Instruction LDAABSX = {
            .name = "Load A (absolute, X)",
            .mnemonic = "LDA",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrHAddXToAddrLAndSkipNextStepIfPageCrossed,
                &CPU6502::fixAddrHighByte,
                &CPU6502::loadAAbsolute
            }
        };

        Instruction LDAABSY = {
            .name = "Load A (absolute, Y)",
            .mnemonic = "LDA",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrHAddYToAddrLAndSkipNextStepIfPageCrossed,
                &CPU6502::fixAddrHighByte,
                &CPU6502::loadAAbsolute
            }
        };

        Instruction LDAINDX = {
            .name = "Load A (indexed indirect, X)",
            .mnemonic = "LDA",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::addXToAddrL,
                &CPU6502::fetchAddrLFromZeroPageIAL,
                &CPU6502::fetchAddrHFromZeroPageIAL,
                &CPU6502::loadAAbsolute
            }
        };

        Instruction LDAINDY = {
            .name = "Load A (indirect indexed, Y)",
            .mnemonic = "LDA",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrLFromZeroPageIAL,
                &CPU6502::fetchAddrHFromZeroPageIALAddYToAddrLAndSkipNextStepIfPageCrossed,
                &CPU6502::fixAddrHighByte,
                &CPU6502::loadAAbsolute
            }
        };

        Instruction LDXIM = {
            .name = "Load X (immediate)",
            .mnemonic = "LDX",
            .cycles = std::vector<MicroOp>{
                &CPU6502::loadXImmediate
            }
        };

        Instruction LDXZP = {
            .name = "Load X (zero page)",
            .mnemonic = "LDX",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::loadXZeroPage
            }
        };

        Instruction LDXZPY = {
            .name = "Load X (zero page, Y)",
            .mnemonic = "LDX",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::addYToAddrL,
                &CPU6502::loadXZeroPageY
            }
        };

        Instruction LDXABS = {
            .name = "Load X (absolute)",
            .mnemonic = "LDX",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrH,
                &CPU6502::loadXAbsolute
            }
        };

        Instruction LDXABSY = {
            .name = "Load X (absolute Y)",
            .mnemonic = "LDX",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrHAddYToAddrLAndSkipNextStepIfPageCrossed,
                &CPU6502::fixAddrHighByte,
                &CPU6502::loadXAbsolute
            }
        };

        Instruction LDYIM = {
            .name = "Load Y (immediate)",
            .mnemonic = "LDY",
            .cycles = std::vector<MicroOp>{
                &CPU6502::loadYImmediate
            }
        };

        Instruction LDYZP = {
            .name = "Load Y (zero page)",
            .mnemonic = "LDY",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::loadYZeroPage
            }
        };

        Instruction LDYZPX = {
            .name = "Load Y (zero page, X)",
            .mnemonic = "LDY",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::addXToAddrL,
                &CPU6502::loadYZeroPageX
            }
        };

        Instruction LDYABS = {
            .name = "Load Y (absolute)",
            .mnemonic = "LDY",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrH,
                &CPU6502::loadYAbsolute
            }
        };

        Instruction LDYABSX = {
            .name = "Load Y (absolute X)",
            .mnemonic = "LDY",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrHAddXToAddrLAndSkipNextStepIfPageCrossed,
                &CPU6502::fixAddrHighByte,
                &CPU6502::loadYAbsolute
            }
        };

        Instruction STAZP = {
            .name = "Store A (zero page)",
            .mnemonic = "STA",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::storeAZeroPage
            }
        };

        Instruction STAZPX = {
            .name = "Store A (zero page X)",
            .mnemonic = "STA",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::addXToAddrL,
                &CPU6502::storeAZeroPage
            }
        };

        Instruction STAABS = {
            .name = "Store A (absolute)",
            .mnemonic = "STA",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrH,
                &CPU6502::storeAAbsolute
            }
        };

        Instruction STAABSX = {
            .name = "Store A (absolute X)",
            .mnemonic = "STA",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrHAddXToAddrLAndCheckPageCross,
                &CPU6502::fixAddrHighByte,
                &CPU6502::storeAAbsolute
            }
        };

        Instruction STAABSY = {
            .name = "Store A (absolute Y)",
            .mnemonic = "STA",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrHAddYToAddrLAndCheckPageCross,
                &CPU6502::fixAddrHighByte,
                &CPU6502::storeAAbsolute
            }
        };

        Instruction STAINDX = {
            .name = "Store A (indexed indirect, X)",
            .mnemonic = "STA",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::addXToAddrL,
                &CPU6502::fetchAddrLFromZeroPageIAL,
                &CPU6502::fetchAddrHFromZeroPageIAL,
                &CPU6502::storeAAbsolute
            }
        };

        Instruction STAINDY = {
            .name = "Store A (indirect indexed, Y)",
            .mnemonic = "STA",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrLFromZeroPageIAL,
                &CPU6502::fetchAddrHFromZeroPageIALAddYToAddrLAndCheckPageCross,
                &CPU6502::fixAddrHighByte,
                &CPU6502::storeAAbsolute
            }
        };

        Instruction STXZP = {
            .name = "Store X (zero page)",
            .mnemonic = "STX",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::storeXZeroPage
            }
        };

        Instruction STXZPY = {
            .name = "Store X (zero page Y)",
            .mnemonic = "STX",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::addYToAddrL,
                &CPU6502::storeXZeroPage
            }
        };

        Instruction STXABS = {
            .name = "Store X (absolute)",
            .mnemonic = "STX",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrH,
                &CPU6502::storeXAbsolute
            }
        };

        Instruction STYZP = {
            .name = "Store Y (zero page)",
            .mnemonic = "STY",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::storeYZeroPage
            }
        };

        Instruction STYZPX = {
            .name = "Store Y (zero page X)",
            .mnemonic = "STY",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::addXToAddrL,
                &CPU6502::storeYZeroPage
            }
        };

        Instruction STYABS = {
            .name = "Store Y (absolute)",
            .mnemonic = "STY",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrH,
                &CPU6502::storeYAbsolute
            }
        };

        Instruction TAX = {
            .name = "Transfer A to X",
            .mnemonic = "TAX",
            .cycles = std::vector<MicroOp>{
                &CPU6502::transferAX
            }
        };

        Instruction TAY = {
            .name = "Transfer A to Y",
            .mnemonic = "TAY",
            .cycles = std::vector<MicroOp>{
                &CPU6502::transferAY
            }
        };

        Instruction TXA = {
            .name = "Transfer X to A",
            .mnemonic = "TXA",
            .cycles = std::vector<MicroOp>{
                &CPU6502::transferXA
            }
        };

        Instruction TYA = {
            .name = "Transfer Y to A",
            .mnemonic = "TYA",
            .cycles = std::vector<MicroOp>{
                &CPU6502::transferYA
            }
        };

        Instruction NOP = {
            .name = "No operation",
            .mnemonic = "NOP",
            .cycles = std::vector<MicroOp>{
                &CPU6502::nop
            }
        };

        Instruction PHA = {
            .name = "Push A",
            .mnemonic = "PHA",
            .cycles = std::vector<MicroOp>{
                &CPU6502::nop,
                &CPU6502::pushA
            }
        };

        Instruction PLA = {
            .name = "Pull A",
            .mnemonic = "PLA",
            .cycles = std::vector<MicroOp>{
                &CPU6502::nop,
                &CPU6502::incrementSP,
                &CPU6502::pullA
            }
        };

        Instruction PHP = {
            .name = "Push Processor Status",
            .mnemonic = "PHP",
            .cycles = std::vector<MicroOp>{
                &CPU6502::nop,
                &CPU6502::pushStatus
            }
        };

        Instruction PLP = {
            .name = "Pull Processor Status",
            .mnemonic = "PLP",
            .cycles = std::vector<MicroOp>{
                &CPU6502::nop,
                &CPU6502::incrementSP,
                &CPU6502::pullStatus
            }
        };

        Instruction TSX = {
            .name = "Transfer SP to X",
            .mnemonic = "TSX",
            .cycles = std::vector<MicroOp>{
                &CPU6502::transferSX
            }
        };

        Instruction TXS = {
            .name = "Transfer X to SP",
            .mnemonic = "TXS",
            .cycles = std::vector<MicroOp>{
                &CPU6502::transferXS
            }
        };

        Instruction CLC = {
            .name = "Clear Carry",
            .mnemonic = "CLC",
            .cycles = std::vector<MicroOp>{
                &CPU6502::clearC
            }
        };

        Instruction CLD = {
            .name = "Clear Decimal",
            .mnemonic = "CLD",
            .cycles = std::vector<MicroOp>{
                &CPU6502::clearD
            }
        };

        Instruction CLI = {
            .name = "Clear Interrupt Disable",
            .mnemonic = "CLI",
            .cycles = std::vector<MicroOp>{
                &CPU6502::clearI
            }
        };

        Instruction CLV = {
            .name = "Clear Overflow",
            .mnemonic = "CLV",
            .cycles = std::vector<MicroOp>{
                &CPU6502::clearV
            }
        };

        Instruction SEC = {
            .name = "Set Carry",
            .mnemonic = "SEC",
            .cycles = std::vector<MicroOp>{
                &CPU6502::setC
            }
        };

        Instruction SED = {
            .name = "Set Decimal",
            .mnemonic = "SED",
            .cycles = std::vector<MicroOp>{
                &CPU6502::setD
            }
        };

        Instruction SEI = {
            .name = "Set Interrupt Disable",
            .mnemonic = "SEI",
            .cycles = std::vector<MicroOp>{
                &CPU6502::setI
            }
        };

        Instruction INX = {
            .name = "Increment X",
            .mnemonic = "INX",
            .cycles = std::vector<MicroOp>{
                &CPU6502::incrementX
            }
        };

        Instruction INY = {
            .name = "Increment Y",
            .mnemonic = "INY",
            .cycles = std::vector<MicroOp>{
                &CPU6502::incrementY
            }
        };

        Instruction DEX = {
            .name = "Decrement X",
            .mnemonic = "DEX",
            .cycles = std::vector<MicroOp>{
                &CPU6502::decrementX
            }
        };

        Instruction DEY = {
            .name = "Decrement Y",
            .mnemonic = "DEY",
            .cycles = std::vector<MicroOp>{
                &CPU6502::decrementY
            }
        };

        Instruction INCZP = {
            .name = "Increment Memory zero page",
            .mnemonic = "INC",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::loadIDLZeroPage,
                &CPU6502::incrementIDL,
                &CPU6502::storeIDLZeroPage
            }
        };

        Instruction INCZPX = {
            .name = "Increment Memory zero page X",
            .mnemonic = "INC",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::addXToAddrL,
                &CPU6502::loadIDLZeroPage,
                &CPU6502::incrementIDL,
                &CPU6502::storeIDLZeroPage
            }
        };

        Instruction INCABS = {
            .name = "Increment Memory absolute",
            .mnemonic = "INC",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrH,
                &CPU6502::loadIDLAbsolute,
                &CPU6502::incrementIDL,
                &CPU6502::storeIDLAbsolute
            }
        };

        Instruction INCABSX = {
            .name = "Increment Memory absolute X",
            .mnemonic = "INC",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrHAddXToAddrLAndCheckPageCross,
                &CPU6502::fixAddrHighByte,
                &CPU6502::loadIDLAbsolute,
                &CPU6502::incrementIDL,
                &CPU6502::storeIDLAbsolute
            }
        };

        Instruction DECZP = {
            .name = "Decrement Memory zero page",
            .mnemonic = "DEC",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::loadIDLZeroPage,
                &CPU6502::decrementIDL,
                &CPU6502::storeIDLZeroPage
            }
        };

        Instruction DECZPX = {
            .name = "Decrement Memory zero page X",
            .mnemonic = "DEC",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::addXToAddrL,
                &CPU6502::loadIDLZeroPage,
                &CPU6502::decrementIDL,
                &CPU6502::storeIDLZeroPage
            }
        };

        Instruction DECABS = {
            .name = "Decrement Memory absolute",
            .mnemonic = "DEC",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrH,
                &CPU6502::loadIDLAbsolute,
                &CPU6502::decrementIDL,
                &CPU6502::storeIDLAbsolute
            }
        };

        Instruction DECABSX = {
            .name = "Decrement Memory absolute X",
            .mnemonic = "DEC",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrHAddXToAddrLAndCheckPageCross,
                &CPU6502::fixAddrHighByte,
                &CPU6502::loadIDLAbsolute,
                &CPU6502::decrementIDL,
                &CPU6502::storeIDLAbsolute
            }
        };

        Instruction ANDIM = {
            .name = "Binary AND A (immediate)",
            .mnemonic = "AND",
            .cycles = std::vector<MicroOp>{
                &CPU6502::andAImmediate
            }
        };

        Instruction ANDZP = {
            .name = "Binary AND A (zero page)",
            .mnemonic = "AND",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::andAZeroPage
            }
        };

        Instruction ANDZPX = {
            .name = "Binary AND A (zero page, X)",
            .mnemonic = "AND",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::addXToAddrL,
                &CPU6502::andAZeroPage
            }
        };

        Instruction ANDABS = {
            .name = "Binary AND A (absolute)",
            .mnemonic = "AND",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrH,
                &CPU6502::andAAbsolute
            }
        };

        Instruction ANDABSX = {
            .name = "Binary AND A (absolute, X)",
            .mnemonic = "AND",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrHAddXToAddrLAndSkipNextStepIfPageCrossed,
                &CPU6502::fixAddrHighByte,
                &CPU6502::andAAbsolute
            }
        };

        Instruction ANDABSY = {
            .name = "Binary AND A (absolute, Y)",
            .mnemonic = "AND",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrHAddYToAddrLAndSkipNextStepIfPageCrossed,
                &CPU6502::fixAddrHighByte,
                &CPU6502::andAAbsolute
            }
        };

        Instruction ANDINDX = {
            .name = "Binary AND A (indexed indirect, X)",
            .mnemonic = "AND",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::addXToAddrL,
                &CPU6502::fetchAddrLFromZeroPageIAL,
                &CPU6502::fetchAddrHFromZeroPageIAL,
                &CPU6502::andAAbsolute
            }
        };

        Instruction ANDINDY = {
            .name = "Binary AND A (indirect indexed, Y)",
            .mnemonic = "AND",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrLFromZeroPageIAL,
                &CPU6502::fetchAddrHFromZeroPageIALAddYToAddrLAndSkipNextStepIfPageCrossed,
                &CPU6502::fixAddrHighByte,
                &CPU6502::andAAbsolute
            }
        };

        Instruction ORAIM = {
            .name = "Binary OR A (immediate)",
            .mnemonic = "ORA",
            .cycles = std::vector<MicroOp>{
                &CPU6502::orAImmediate
            }
        };

        Instruction ORAZP = {
            .name = "Binary OR A (zero page)",
            .mnemonic = "ORA",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::orAZeroPage
            }
        };

        Instruction ORAZPX = {
            .name = "Binary OR A (zero page, X)",
            .mnemonic = "ORA",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::addXToAddrL,
                &CPU6502::orAZeroPage
            }
        };

        Instruction ORAABS = {
            .name = "Binary OR A (absolute)",
            .mnemonic = "ORA",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrH,
                &CPU6502::orAAbsolute
            }
        };

        Instruction ORAABSX = {
            .name = "Binary OR A (absolute, X)",
            .mnemonic = "ORA",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrHAddXToAddrLAndSkipNextStepIfPageCrossed,
                &CPU6502::fixAddrHighByte,
                &CPU6502::orAAbsolute
            }
        };

        Instruction ORAABSY = {
            .name = "Binary OR A (absolute, Y)",
            .mnemonic = "ORA",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrHAddYToAddrLAndSkipNextStepIfPageCrossed,
                &CPU6502::fixAddrHighByte,
                &CPU6502::orAAbsolute
            }
        };

        Instruction ORAINDX = {
            .name = "Binary OR A (indexed indirect, X)",
            .mnemonic = "ORA",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::addXToAddrL,
                &CPU6502::fetchAddrLFromZeroPageIAL,
                &CPU6502::fetchAddrHFromZeroPageIAL,
                &CPU6502::orAAbsolute
            }
        };

        Instruction ORAINDY = {
            .name = "Binary OR A (indirect indexed, Y)",
            .mnemonic = "ORA",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrLFromZeroPageIAL,
                &CPU6502::fetchAddrHFromZeroPageIALAddYToAddrLAndSkipNextStepIfPageCrossed,
                &CPU6502::fixAddrHighByte,
                &CPU6502::orAAbsolute
            }
        };

        Instruction EORIM = {
            .name = "Compare A (immediate)",
            .mnemonic = "EOR",
            .cycles = std::vector<MicroOp>{
                &CPU6502::xorAImmediate
            }
        };

        Instruction EORZP = {
            .name = "Bitwise Exclusive OR A (zero page)",
            .mnemonic = "EOR",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::xorAZeroPage
            }
        };

        Instruction EORZPX = {
            .name = "Compare A (zero page, X)",
            .mnemonic = "EOR",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::addXToAddrL,
                &CPU6502::xorAZeroPage
            }
        };

        Instruction EORABS = {
            .name = "Compare A (absolute)",
            .mnemonic = "EOR",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrH,
                &CPU6502::xorAAbsolute
            }
        };

        Instruction EORABSX = {
            .name = "Compare A (absolute, X)",
            .mnemonic = "EOR",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrHAddXToAddrLAndSkipNextStepIfPageCrossed,
                &CPU6502::fixAddrHighByte,
                &CPU6502::xorAAbsolute
            }
        };

        Instruction EORABSY = {
            .name = "Compare A (absolute, Y)",
            .mnemonic = "EOR",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrHAddYToAddrLAndSkipNextStepIfPageCrossed,
                &CPU6502::fixAddrHighByte,
                &CPU6502::xorAAbsolute
            }
        };

        Instruction EORINDX = {
            .name = "Compare A (indexed indirect, X)",
            .mnemonic = "EOR",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::addXToAddrL,
                &CPU6502::fetchAddrLFromZeroPageIAL,
                &CPU6502::fetchAddrHFromZeroPageIAL,
                &CPU6502::xorAAbsolute
            }
        };

        Instruction EORINDY = {
            .name = "Compare A (indirect indexed, Y)",
            .mnemonic = "EOR",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrLFromZeroPageIAL,
                &CPU6502::fetchAddrHFromZeroPageIALAddYToAddrLAndSkipNextStepIfPageCrossed,
                &CPU6502::fixAddrHighByte,
                &CPU6502::xorAAbsolute
            }
        };

        Instruction BITZP = {
            .name = "Bit Test (zero page)",
            .mnemonic = "BIT",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::bitTestZeroPage
            }
        };

        Instruction BITABS = {
            .name = "Bit Test (absolute)",
            .mnemonic = "BIT",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrH,
                &CPU6502::bitTestAbsolute
            }
        };

        Instruction CMPIM = {
            .name = "Compare A (immediate)",
            .mnemonic = "CMP",
            .cycles = std::vector<MicroOp>{
                &CPU6502::compareAImmediate
            }
        };

        Instruction CMPZP = {
            .name = "Compare A (zero page)",
            .mnemonic = "CMP",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::compareAZeroPage
            }
        };

        Instruction CMPZPX = {
            .name = "Compare A (zero page, X)",
            .mnemonic = "CMP",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::addXToAddrL,
                &CPU6502::compareAZeroPage
            }
        };

        Instruction CMPABS = {
            .name = "Compare A (absolute)",
            .mnemonic = "CMP",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrH,
                &CPU6502::compareAAbsolute
            }
        };

        Instruction CMPABSX = {
            .name = "Compare A (absolute, X)",
            .mnemonic = "CMP",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrHAddXToAddrLAndSkipNextStepIfPageCrossed,
                &CPU6502::fixAddrHighByte,
                &CPU6502::compareAAbsolute
            }
        };

        Instruction CMPABSY = {
            .name = "Compare A (absolute, Y)",
            .mnemonic = "CMP",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrHAddYToAddrLAndSkipNextStepIfPageCrossed,
                &CPU6502::fixAddrHighByte,
                &CPU6502::compareAAbsolute
            }
        };

        Instruction CMPINDX = {
            .name = "Compare A (indexed indirect, X)",
            .mnemonic = "CMP",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::addXToAddrL,
                &CPU6502::fetchAddrLFromZeroPageIAL,
                &CPU6502::fetchAddrHFromZeroPageIAL,
                &CPU6502::compareAAbsolute
            }
        };

        Instruction CMPINDY = {
            .name = "Compare A (indirect indexed, Y)",
            .mnemonic = "CMP",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrLFromZeroPageIAL,
                &CPU6502::fetchAddrHFromZeroPageIALAddYToAddrLAndSkipNextStepIfPageCrossed,
                &CPU6502::fixAddrHighByte,
                &CPU6502::compareAAbsolute
            }
        };

        Instruction CPXIM = {
            .name = "Compare X (immediate)",
            .mnemonic = "CPX",
            .cycles = std::vector<MicroOp>{
                &CPU6502::compareXImmediate,
            }
        };

        Instruction CPXZP = {
            .name = "Compare X (zero page)",
            .mnemonic = "CPX",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::compareXZeroPage
            }
        };

        Instruction CPXABS = {
            .name = "Compare X (absolute)",
            .mnemonic = "CPX",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrH,
                &CPU6502::compareXAbsolute
            }
        };

        Instruction CPYIM = {
            .name = "Compare Y (immediate)",
            .mnemonic = "CPY",
            .cycles = std::vector<MicroOp>{
                &CPU6502::compareYImmediate,
            }
        };

        Instruction CPYZP = {
            .name = "Compare Y (zero page)",
            .mnemonic = "CPY",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::compareYZeroPage
            }
        };

        Instruction CPYABS = {
            .name = "Compare Y (absolute)",
            .mnemonic = "CPY",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrH,
                &CPU6502::compareYAbsolute
            }
        };

        Instruction JMPABS = {
            .name = "Jump (absolute)",
            .mnemonic = "JMP",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::jmpAbsolute,
            }
        };

        Instruction JMPIND = {
            .name = "Jump (indirect)",
            .mnemonic = "JMP",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrH,
                &CPU6502::fetchPCLToIDL,
                &CPU6502::jmpIndirect,
            }
        };

        Instruction JSR = {
            .name = "Jump to subroutine",
            .mnemonic = "JSR",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::unknown,
                &CPU6502::pushPCH,
                &CPU6502::pushPCL,
                &CPU6502::jmpAbsolute,
            }
        };

        Instruction RTS = {
            .name = "Return from subroutine",
            .mnemonic = "RTS",
            .cycles = std::vector<MicroOp>{
                &CPU6502::nop,
                &CPU6502::incrementSP,
                &CPU6502::pullPCL,
                &CPU6502::pullPCH,
                &CPU6502::incrementPC,
            }
        };

        Instruction BCC = {
            .name = "Branch if carry clear",
            .mnemonic = "BCC",
            .cycles = std::vector<MicroOp>{
                &CPU6502::checkBCCCondition,
                &CPU6502::branch,
                &CPU6502::fixPCHighByte
            }
        };

        Instruction BCS = {
            .name = "Branch if carry set",
            .mnemonic = "BCS",
            .cycles = std::vector<MicroOp>{
                &CPU6502::checkBCSCondition,
                &CPU6502::branch,
                &CPU6502::fixPCHighByte
            }
        };

        Instruction BEQ = {
            .name = "Branch if equal",
            .mnemonic = "BEQ",
            .cycles = std::vector<MicroOp>{
                &CPU6502::checkBEQCondition,
                &CPU6502::branch,
                &CPU6502::fixPCHighByte
            }
        };

        Instruction BMI = {
            .name = "Branch if minus",
            .mnemonic = "BMI",
            .cycles = std::vector<MicroOp>{
                &CPU6502::checkBMICondition,
                &CPU6502::branch,
                &CPU6502::fixPCHighByte
            }
        };

        Instruction BNE = {
            .name = "Branch if not equal",
            .mnemonic = "BNE",
            .cycles = std::vector<MicroOp>{
                &CPU6502::checkBNECondition,
                &CPU6502::branch,
                &CPU6502::fixPCHighByte
            }
        };

        Instruction BPL = {
            .name = "Branch if plus",
            .mnemonic = "BPL",
            .cycles = std::vector<MicroOp>{
                &CPU6502::checkBPLCondition,
                &CPU6502::branch,
                &CPU6502::fixPCHighByte
            }
        };

        Instruction BVC = {
            .name = "Branch if overflow clear",
            .mnemonic = "BCV",
            .cycles = std::vector<MicroOp>{
                &CPU6502::checkBVCCondition,
                &CPU6502::branch,
                &CPU6502::fixPCHighByte
            }
        };

        Instruction BVS = {
            .name = "Branch if overflow set",
            .mnemonic = "BCS",
            .cycles = std::vector<MicroOp>{
                &CPU6502::checkBVSCondition,
                &CPU6502::branch,
                &CPU6502::fixPCHighByte
            }
        };

        Instruction ASLACC = {
            .name = "Arithmetic shift left (accumulator)",
            .mnemonic = "ASL",
            .cycles = std::vector<MicroOp>{
                &CPU6502::aslAccumulator
            }
        };

        Instruction ASLZP = {
            .name = "Arithmetic shift left (zero page)",
            .mnemonic = "ASL",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::loadIDLZeroPage,
                &CPU6502::aslIDL,
                &CPU6502::storeIDLZeroPage
            }
        };

        Instruction ASLZPX = {
            .name = "Arithmetic shift left (zero page X)",
            .mnemonic = "ASL",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::addXToAddrL,
                &CPU6502::loadIDLZeroPage,
                &CPU6502::aslIDL,
                &CPU6502::storeIDLZeroPage
            }
        };

        Instruction ASLABS = {
            .name = "Arithmetic shift left (absolute)",
            .mnemonic = "ASL",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrH,
                &CPU6502::loadIDLAbsolute,
                &CPU6502::aslIDL,
                &CPU6502::storeIDLAbsolute
            }
        };

        Instruction ASLABSX = {
            .name = "Arithmetic shift left (absolute X)",
            .mnemonic = "ASL",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrHAddXToAddrLAndCheckPageCross,
                &CPU6502::fixAddrHighByte,
                &CPU6502::loadIDLAbsolute,
                &CPU6502::aslIDL,
                &CPU6502::storeIDLAbsolute
            }
        };

        Instruction LSRACC = {
            .name = "Logic shift right (accumulator)",
            .mnemonic = "LSR",
            .cycles = std::vector<MicroOp>{
                &CPU6502::lsrAccumulator
            }
        };

        Instruction LSRZP = {
            .name = "Logic shift right (zero page)",
            .mnemonic = "LSR",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::loadIDLZeroPage,
                &CPU6502::lsrIDL,
                &CPU6502::storeIDLZeroPage
            }
        };

        Instruction LSRZPX = {
            .name = "Logic shift right (zero page X)",
            .mnemonic = "LSR",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::addXToAddrL,
                &CPU6502::loadIDLZeroPage,
                &CPU6502::lsrIDL,
                &CPU6502::storeIDLZeroPage
            }
        };

        Instruction LSRABS = {
            .name = "Logic shift right (absolute)",
            .mnemonic = "LSR",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrH,
                &CPU6502::loadIDLAbsolute,
                &CPU6502::lsrIDL,
                &CPU6502::storeIDLAbsolute
            }
        };

        Instruction LSRABSX = {
            .name = "Logic shift right (absolute X)",
            .mnemonic = "LSR",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrHAddXToAddrLAndCheckPageCross,
                &CPU6502::fixAddrHighByte,
                &CPU6502::loadIDLAbsolute,
                &CPU6502::lsrIDL,
                &CPU6502::storeIDLAbsolute
            }
        };

        Instruction ROLACC = {
            .name = "Rotate left (accumulator)",
            .mnemonic = "ROL",
            .cycles = std::vector<MicroOp>{
                &CPU6502::rolAccumulator
            }
        };

        Instruction ROLZP = {
            .name = "Rotate left (zero page)",
            .mnemonic = "ROL",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::loadIDLZeroPage,
                &CPU6502::rolIDL,
                &CPU6502::storeIDLZeroPage
            }
        };

        Instruction ROLZPX = {
            .name = "Rotate left (zero page X)",
            .mnemonic = "ROL",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::addXToAddrL,
                &CPU6502::loadIDLZeroPage,
                &CPU6502::rolIDL,
                &CPU6502::storeIDLZeroPage
            }
        };

        Instruction ROLABS = {
            .name = "Rotate left (absolute)",
            .mnemonic = "ROL",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrH,
                &CPU6502::loadIDLAbsolute,
                &CPU6502::rolIDL,
                &CPU6502::storeIDLAbsolute
            }
        };

        Instruction ROLABSX = {
            .name = "Rotate left (absolute X)",
            .mnemonic = "ROL",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrHAddXToAddrLAndCheckPageCross,
                &CPU6502::fixAddrHighByte,
                &CPU6502::loadIDLAbsolute,
                &CPU6502::rolIDL,
                &CPU6502::storeIDLAbsolute
            }
        };

        Instruction RORACC = {
            .name = "Rotate right (accumulator)",
            .mnemonic = "ROR",
            .cycles = std::vector<MicroOp>{
                &CPU6502::rorAccumulator
            }
        };

        Instruction RORZP = {
            .name = "Rotate right (zero page)",
            .mnemonic = "ROR",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::loadIDLZeroPage,
                &CPU6502::rorIDL,
                &CPU6502::storeIDLZeroPage
            }
        };

        Instruction RORZPX = {
            .name = "Rotate right (zero page X)",
            .mnemonic = "ROR",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::addXToAddrL,
                &CPU6502::loadIDLZeroPage,
                &CPU6502::rorIDL,
                &CPU6502::storeIDLZeroPage
            }
        };

        Instruction RORABS = {
            .name = "Rotate right (absolute)",
            .mnemonic = "ROR",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrH,
                &CPU6502::loadIDLAbsolute,
                &CPU6502::rorIDL,
                &CPU6502::storeIDLAbsolute
            }
        };

        Instruction RORABSX = {
            .name = "Rotate right (absolute X)",
            .mnemonic = "ROR",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrHAddXToAddrLAndCheckPageCross,
                &CPU6502::fixAddrHighByte,
                &CPU6502::loadIDLAbsolute,
                &CPU6502::rorIDL,
                &CPU6502::storeIDLAbsolute
            }
        };

        Instruction ADCIM = {
            .name = "Add with carry (immediate)",
            .mnemonic = "ADC",
            .cycles = std::vector<MicroOp>{
                &CPU6502::adcAImmediate
            }
        };

        Instruction ADCZP = {
            .name = "Add with carry (zero page)",
            .mnemonic = "ADC",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::adcAZeroPage
            }
        };

        Instruction ADCZPX = {
            .name = "Add with carry (zero page, X)",
            .mnemonic = "ADC",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::addXToAddrL,
                &CPU6502::adcAZeroPage
            }
        };

        Instruction ADCABS = {
            .name = "Add with carry (absolute)",
            .mnemonic = "ADC",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrH,
                &CPU6502::adcAAbsolute
            }
        };

        Instruction ADCABSX = {
            .name = "Add with carry (absolute, X)",
            .mnemonic = "ADC",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrHAddXToAddrLAndSkipNextStepIfPageCrossed,
                &CPU6502::fixAddrHighByte,
                &CPU6502::adcAAbsolute
            }
        };

        Instruction ADCABSY = {
            .name = "Add with carry (absolute, Y)",
            .mnemonic = "ADC",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrHAddYToAddrLAndSkipNextStepIfPageCrossed,
                &CPU6502::fixAddrHighByte,
                &CPU6502::adcAAbsolute
            }
        };

        Instruction ADCINDX = {
            .name = "Add with carry (indexed indirect, X)",
            .mnemonic = "ADC",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::addXToAddrL,
                &CPU6502::fetchAddrLFromZeroPageIAL,
                &CPU6502::fetchAddrHFromZeroPageIAL,
                &CPU6502::adcAAbsolute
            }
        };

        Instruction ADCINDY = {
            .name = "Add with carry (indirect indexed, Y)",
            .mnemonic = "ADC",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrLFromZeroPageIAL,
                &CPU6502::fetchAddrHFromZeroPageIALAddYToAddrLAndSkipNextStepIfPageCrossed,
                &CPU6502::fixAddrHighByte,
                &CPU6502::adcAAbsolute
            }
        };

        Instruction SBCIM = {
            .name = "Subtract with carry (immediate)",
            .mnemonic = "SBC",
            .cycles = std::vector<MicroOp>{
                &CPU6502::sbcAImmediate
            }
        };

        Instruction SBCZP = {
            .name = "Subtract with carry (zero page)",
            .mnemonic = "SBC",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::sbcAZeroPage
            }
        };

        Instruction SBCZPX = {
            .name = "Subtract with carry (zero page, X)",
            .mnemonic = "SBC",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::addXToAddrL,
                &CPU6502::sbcAZeroPage
            }
        };

        Instruction SBCABS = {
            .name = "Subtract with carry (absolute)",
            .mnemonic = "SBC",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrH,
                &CPU6502::sbcAAbsolute
            }
        };

        Instruction SBCABSX = {
            .name = "Subtract with carry (absolute, X)",
            .mnemonic = "SBC",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrHAddXToAddrLAndSkipNextStepIfPageCrossed,
                &CPU6502::fixAddrHighByte,
                &CPU6502::sbcAAbsolute
            }
        };

        Instruction SBCABSY = {
            .name = "Subtract with carry (absolute, Y)",
            .mnemonic = "SBC",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrHAddYToAddrLAndSkipNextStepIfPageCrossed,
                &CPU6502::fixAddrHighByte,
                &CPU6502::sbcAAbsolute
            }
        };

        Instruction SBCINDX = {
            .name = "Subtract with carry (indexed indirect, X)",
            .mnemonic = "SBC",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::addXToAddrL,
                &CPU6502::fetchAddrLFromZeroPageIAL,
                &CPU6502::fetchAddrHFromZeroPageIAL,
                &CPU6502::sbcAAbsolute
            }
        };

        Instruction SBCINDY = {
            .name = "Subtract with carry (indirect indexed, Y)",
            .mnemonic = "SBC",
            .cycles = std::vector<MicroOp>{
                &CPU6502::fetchAddrL,
                &CPU6502::fetchAddrLFromZeroPageIAL,
                &CPU6502::fetchAddrHFromZeroPageIALAddYToAddrLAndSkipNextStepIfPageCrossed,
                &CPU6502::fixAddrHighByte,
                &CPU6502::sbcAAbsolute
            }
        };

        Instruction BRK = {
            .name = "Break",
            .mnemonic = "SBC",
            .cycles = std::vector<MicroOp>{
                &CPU6502::readNextInstructionAndIncrementPC,
                &CPU6502::pushPCH,
                &CPU6502::pushPCL,
                &CPU6502::pushStatusAndSetI,
                &CPU6502::fetchIRQL,
                &CPU6502::fetchIRQH,
            }
        };

        Instruction RTI = {
            .name = "Return from interrupt",
            .mnemonic = "RTI",
            .cycles = std::vector<MicroOp>{
                &CPU6502::nop,
                &CPU6502::incrementSP,
                &CPU6502::pullStatusAndIncrementSP,
                &CPU6502::pullPCL,
                &CPU6502::pullPCH
            }
        };
};