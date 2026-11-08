#include <NES/6502.hpp>
#include <stdexcept>

CPU6502::CPU6502(sptr<IMemoryMapper> memoryMapper){
    X = 0;
    Y = 0;
    A = 0;
    IR = 0;
    SP = 0xFD;
    PC = 0xFFFC;

    /*
    status flags state on startup:
    C=0
    Z=0
    I=1
    D=0
    V=0
    N=0
    bit 5 is always 1, bit 4 (flag B) is irrelevant on startup

    status register flags:
    76543210
    NV1BDIZC
    */
    status = 0b00100100;

    this->memory = memoryMapper;
}

void CPU6502::clkstep()
{
    // If there's no instruction currently, fetch the next one.
    // Fetching the opcode is always done in the first cycle.
    if (currentInstruction == nullptr) {
        opcode = read(PC++);
        currentInstruction = decodeInstruction(opcode);
        return;
    }

    // Run the current micro step
    (this->*(currentInstruction->cycles[currentInstructionStep]))();

    currentInstructionStep++;

    // End of current instruction
    if (currentInstructionStep >= currentInstruction->cycles.size()) {
        currentInstructionStep = 0;
        currentInstruction = nullptr;
    }
}

byte_t CPU6502::read(addr_t addr) const{
    return memory->read(addr, ADDRESS_SPACE::CPU);
}

void CPU6502::write(addr_t addr, byte_t data) const{
    memory->write(addr, ADDRESS_SPACE::CPU, data);
}

CPU6502::Instruction *CPU6502::decodeInstruction(byte_t opcode)
{
    // Opcode bits can be divided as aaabbbcc
    // Gnerally, a combination of A+C dictate the kind of instruction,
    // while B dictates the addressing mode
    switch (opcode)
    {
        case 0b10101001: //5.2.1: LDA # - Load A immediate
            return &LDAIM;

        case 0b10100101: //5.1.1: LDA zp - Load A zero page
            return &LDAZP;

        case 0b10110101: // 5.5.1: LDA zp, x - Load A zero page X
            return &LDAZPX;

        case 0b10101101: // 5.3.1: LDA abs - Load A absolute
            return &LDAABS;

        case 0b10111101: // 5.7.1: LDA abs, x - Load A absolute X
            return &LDAABSX;

        case 0b10111001: // 5.6.1: LDA abs, y - Load A absolute Y
            return &LDAABSY;

        case 0b10100001: // 5.0.1: LDA X, ind - Load A indexed indirect X
            return &LDAINDX;

        case 0b10110001: // 5.4.1: LDA ind, y - Load A indirect indexed Y
            return &LDAINDY;

        case 0b10100010: // 5.0.2: LDX # - Load X immediate
            return &LDXIM;

        case 0b10100110: // 5.1.2: LDX zp - Load X zero page
            return &LDXZP;

        case 0b10110110: // 5.5.2: LDX zp, Y - Load X zero page, Y
            return &LDXZPY;

        case 0b10101110: // 5.3.2: LDX abs - Load X absolute
            return &LDXABS;

        case 0b10111110: // 5.7.2: LDX abs y - Load X absolute Y
            return &LDXABSY;

        case 0b10100000: // 5.0.0: LDX # - Load Y immediate
            return &LDYIM;

        case 0b10100100: // 5.1.0: LDX zp - Load Y zero page
            return &LDYZP;

        case 0b10110100: // 5.5.0: LDX zp, Y - Load Y zero page, X
            return &LDYZPX;

        case 0b10101100: // 5.3.0: LDX abs - Load Y absolute
            return &LDYABS;

        case 0b10111100: // 5.7.0: LDX abs y - Load Y absolute X
            return &LDYABSX;

        case 0b10000101: // 4.1.1: STA zp - Store A zero page
            return &STAZP;

        case 0b10010101: //4.5.1: STA zp, x - Store A zero page X
            return &STAZPX;

        case 0b10001101: //4.3.1: STA abs - Store A absolute
            return &STAABS;

        case 0b10011101: //4.7.1: STA abs, x - Store A absolute X
            return &STAABSX;
        
        case 0b10011001: //4.6.1: STA abs, y - Store A absolute Y
            return &STAABSY;

        case 0b10000001: //4.0.1: STA X, ind - Store A indexed indirect X
            return &STAINDX;

        case 0b10010001: //4.4.1: STA ind, y - Store A indirect indexed Y
            return &STAINDY;

        case 0b10000110: //4.1.2: STX zp - Store X zero page
            return &STXZP;

        case 0b10010110: //4.5.2: STX zp y - Store X zero page, Y
            return &STXZPY;

        case 0b10001110: //4.3.2: STX abs - Store X absolute
            return &STXABS;

        case 0b10000100: //4.1.0: STY zp - Store Y zero page
            return &STYZP;

        case 0b10010100: //4.5.0: STY zp X - Store Y zero page, X
            return &STYZPX;

        case 0b10001100: //4.3.0: STY abs - Store Y absolute
            return &STYABS;
        
        // case 0b01101001: //3.2.1: ADC # - Add with Carry immediate
        //     return &ADCIM;

        // case 0b01100101: //3.1.1: ADC zp - Add with Carry zero page
        //     return &ADCZP;

        // case 0b01110101: //3.5.1: ADC zp, x - Add with Carry zero page X
        //     return &ADCZPX;

        // case 0b01101101: //3.3.1: ADC abs - Add with Carry absolute
        //     return &ADCABS;

        // case 0b01111101: //3.7.1: ADC abs, x - Add with Carry absolute X
        //     return &ADCABSX;

        // case 0b01111001: //3.6.1: ADC abs, y - Add with Carry absolute Y
        //     return &ADCABSY;
        
        // case 0b01100001: //3.0.1: ADC X, ind - Add with Carry indexed indirect X
        //     return &ADCINDX;

        // case 0b01110001: //3.4.1: ADC ind, y - Add with Carry indirect indexed Y
        //     return &ADCINDY;

        // case 0b11101001: //7.2.1: SBC # - Subtract with Carry immediate
        //     return &SBCIM;
        
        // case 0b11100101: //7.1.1: SBC zp - Subtract with Carry zero page
        //     return &SBCZP;
        
        // case 0b11110101: //7.5.1: SBC zp, x - Subtract with Carry zero page X
        //     return &SBCZPX;
        
        // case 0b11101101: //7.3.1: SBC abs - Subtract with Carry absolute
        //     return &SBCABS;
        
        // case 0b11111101: //7.7.1: SBC abs, x - Subtract with Carry absolute X
        //     return &SBCABSX;

        // case 0b11111001: //7.6.1: SBC abs, y - Subtract with Carry absolute Y
        //     return &SBCABSY;

        // case 0b11100001: //7.0.1: SBC X, ind - Subtract with Carry indexed indirect X
        //     return &SBCINDX;
        
        // case 0b11110001: //7.4.1: SBC ind, y - Subtract with Carry indirect indexed Y
        //     return &SBCINDY;

        case 0b00101001: //1.2.1: AND # - Logical AND immediate
            return &ANDIM;

        case 0b00100101: //1.1.1: AND zp - Logical AND zero page
            return &ANDZP;

        case 0b00110101: //1.5.1: AND zp, x - Logical AND zero page X
            return &ANDZPX;
        
        case 0b00101101: //1.3.1: AND abs - Logical AND absolute
            return &ANDABS;
        
        case 0b00111101: //1.7.1: AND abs, x - Logical AND absolute X
            return &ANDABSX;
        
        case 0b00111001: //1.6.1: AND abs, y - Logical AND absolute Y
            return &ANDABSY;
        
        case 0b00100001: //1.0.1: AND X, ind - Logical AND indexed indirect X
            return &ANDINDX;

        case 0b00110001: //1.4.1: AND ind, y - Logical AND indirect indexed Y
            return &ANDINDY;

        case 0b00001001: //0.2.1: ORA # - Logical inclusive OR immediate
            return &ORAIM;
        
        case 0b00000101: //0.1.1: ORA zp - Logical inclusive OR zero page
            return &ORAZP;
        
        case 0b00010101: //0.5.1: ORA zp, x - Logical inclusive OR zero page X
            return &ORAZPX;

        case 0b00001101: //0.3.1: ORA abs - Logical inclusive OR absolute
            return &ORAABS;

        case 0b00011101: //0.7.1: ORA abs, x - Logical inclusive OR absolute X
            return &ORAABSX;

        case 0b00011001: //0.6.1: ORA abs, y - Logical inclusive OR absolute Y
            return &ORAABSY;

        case 0b00000001: //0.0.1: ORA X, ind - Logical inclusive OR indexed indirect X
            return &ORAINDX;

        case 0b00010001: //0.4.1: ORA ind, y - Logical inclusive OR indirect indexed Y
            return &ORAINDY;
        
        case 0b01001001: //2.2.1: EOR # - Exclusive OR immediate
            return &EORIM;
        
        case 0b01000101: //2.1.1: EOR zp - Exclusive OR zero page
            return &EORZP;
        
        case 0b01010101: //2.5.1: EOR zp, x - Exclusive OR zero page X
            return &EORZPX;
        
        case 0b01001101: //2.3.1: EOR abs - Exclusive OR absolute
            return &EORABS;
        
        case 0b01011101: //2.7.1: EOR abs, x - Exclusive OR absolute X
            return &EORABSX;
        
        case 0b01011001: //2.6.1: EOR abs, y - Exclusive OR absolute Y
            return &EORABSY;
        
        case 0b01000001: //2.0.1: EOR X, ind - Exclusive OR indexed indirect X
            return &EORINDX;
        
        case 0b01010001: //2.4.1: EOR ind, y - Exclusive OR indirect indexed Y
            return &EORINDY;
        
        case 0b11001001: // 6.2.1: CMP # - Compare A immediate
            return &CMPIM;
        
        case 0b11000101: // 6.1.1: CMP zp - Compare A zero page
            return &CMPZP;
        
        case 0b11010101: // 6.5.1: CMP zp, x - Compare A zero page X
            return &CMPZPX;
        
        case 0b11001101: // 6.3.1: CMP abs - Compare A absolute
            return &CMPABS;
        
        case 0b11011101: // 6.7.1: CMP abs, x - Compare A absolute X
            return &CMPABSX;
        
        case 0b11011001: // 6.6.1: CMP abs, y - Compare A absolute Y
            return &CMPABSY;
        
        case 0b11000001: // 6.0.1: CMP X, ind - Compare A indexed indirect X
            return &CMPINDX;
        
        case 0b11010001: // 6.4.1: CMP ind, y - Compare A indirect indexed Y
            return &CMPINDY;
        
        case 0b11100000: // 6.0.2: CPX # - Compare X immediate
            return &CPXIM;
        
        case 0b11100100: // 6.1.2: CPX zp - Compare X zero page
            return &CPXZP;
        
        case 0b11101100: // 6.3.2: CPX abs - Compare X absolute
            return &CPXABS;
        
        case 0b11000000: // 6.0.0: CPY # - Compare Y immediate
            return &CPYIM;
        
        case 0b11000100: // 6.1.0: CPY zp - Compare Y zero page
            return &CPYZP;
        
        case 0b11001100: // 6.3.0: CPY abs - Compare Y absolute
            return &CPYABS;
        
        case 0b00100100: // 1.1.0: BIT zp - Test bits in zero page
            return &BITZP;
        
        case 0b00101100: // 1.3.0: BIT abs - Test bits in absolute memory
            return &BITABS;
        
        // case 0b00000110: // 0.1.2: ASL zp - Arithmetic shift left zero page
        //     return &ASLZP;
        
        // case 0b00010110: // 0.5.2: ASL zp, x - Arithmetic shift left zero page X
        //     return &ASLZPX;
        
        // case 0b00001110: // 0.3.2: ASL abs - Arithmetic shift left absolute
        //     return &ASLABS;
        
        // case 0b00011110: // 0.7.2: ASL abs, x - Arithmetic shift left absolute X
        //     return &ASLABSX;
        
        // case 0b00001010: // 0.2.2: ASL A - Arithmetic shift left A
        //     return &ASLACC;
        
        // case 0b01000110: // 4.1.2: LSR zp - Logical shift right zero page
        //     return &LSRZP;
        
        // case 0b01010110: // 4.5.2: LSR zp, x - Logical shift right zero page X
        //     return &LSRZPX;
        
        // case 0b01001110: // 4.3.2: LSR abs - Logical shift right absolute
        //     return &LSRABS;
        
        // case 0b01011110: // 4.7.2: LSR abs, x - Logical shift right absolute X
        //     return &LSRABSX;
        
        // case 0b01001010: // 4.2.2: LSR A - Logical shift right A
        //     return &LSRACC;
        
        // case 0b00100110: // 2.1.2: ROL zp - Rotate left zero page
        //     return &ROLZP;
        
        // case 0b00110110: // 2.5.2: ROL zp, x - Rotate left zero page X
        //     return &ROLZPX;
        
        // case 0b00101110: // 2.3.2: ROL abs - Rotate left absolute
        //     return &ROLABS;
        
        // case 0b00111110: // 2.7.2: ROL abs, x - Rotate left absolute X
        //     return &ROLABSX;
        
        // case 0b00101010: // 2.2.2: ROL A - Rotate left A
        //     return &ROLACC;
        
        // case 0b01100110: // 6.1.2: ROR zp - Rotate right zero page
        //     return &RORZP;
        
        // case 0b01110110: // 6.5.2: ROR zp, x - Rotate right zero page X
        //     return &RORZPX;
        
        // case 0b01101110: // 6.3.2: ROR abs - Rotate right absolute
        //     return &RORABS;
        
        // case 0b01111110: // 6.7.2: ROR abs, x - Rotate right absolute X
        //     return &RORABSX;
        
        // case 0b01101010: // 6.2.2: ROR A - Rotate right A
        //     return &RORACC;
        
        case 0b11000110: // 6.1.3: DEC zp - Decrement memory zero page
            return &DECZP;
        
        case 0b11010110: // 6.5.3: DEC zp, x - Decrement memory zero page X
            return &DECZPX;
        
        case 0b11001110: // 6.3.3: DEC abs - Decrement memory absolute
            return &DECABS;
        
        case 0b11011110: // 6.7.3: DEC abs, x - Decrement memory absolute X
            return &DECABSX;
        
        case 0b11001010: // 6.2.0: DEX - Decrement X
            return &DEX;
        
        case 0b10001000: // 4.2.0: DEY - Decrement Y
            return &DEY;
        
        case 0b11100110: // 7.1.3: INC zp - Increment memory zero page
            return &INCZP;

        case 0b11110110: // 7.5.3: INC zp, x - Increment memory zero page X
            return &INCZPX;
        
        case 0b11101110: // 7.3.3: INC abs - Increment memory absolute
            return &INCABS;

        case 0b11111110: // 7.7.3: INC abs, x - Increment memory absolute X
            return &INCABSX;
        
        case 0b11101000: // 7.2.0: INX - Increment X
            return &INX;

        case 0b11001000: // 7.2.0: INY - Increment Y
            return &INY;
        
        case 0b10010000: // BCC - Branch if carry clear
            return &BCC;
        
        // case 0b10110000: // BCS - Branch if carry set
        //     return &BCS;
        
        // case 0b11110000: // BEQ - Branch if equal
        //     return &BEQ;
        
        // case 0b00110000: // BMI - Branch if minus
        //     return &BMI;
        
        // case 0b11010000: // BNE - Branch if not equal
        //     return &BNE;
        
        // case 0b00010000: // BPL - Branch if positive
        //     return &BPL;
        
        // case 0b01010000: // BVC - Branch if overflow clear
        //     return &BVC;
        
        // case 0b01110000: // BVS - Branch if overflow set
        //     return &BVS;
        
        case 0b01001100: // 2.3.0 JMP abs - Jump absolute
            return &JMPABS;
        
        case 0b01101100: // 3.3.0 JMP ind - Jump indirect
            return &JMPIND;

        case 0b00100000: // JSR - Jump to subroutine
            return &JSR;

        case 0b01100000: // RTS - Return from subroutine
            return &RTS;

        // case 0b01000000: // RTI - Return from interrupt
        //     return &RTI;
        
        // case 0b00000000: // BRK - Force interrupt
        //     return &BRK;
        
        case 0b01001000: // 2.2.0 PHA - Push A
            return &PHA;
        
        case 0b01101000: // 3.2.0 PLA - Pull A
            return &PLA;
        
        case 0b00001000: // 0.2.0 PHP - Push processor status
            return &PHP;
        
        case 0b00101000: // 1.2.0 PLP - Pull processor status
            return &PLP;
        
        case 0b00011000: // 0.6.0 CLC - Clear carry flag
            return &CLC;
        
        case 0b00111000: // 1.6.0 SEC - Set carry flag
            return &SEC;
        
        case 0b01011000: // 2.6.0 CLI - Clear interrupt-disable flag
            return &CLI;
        
        case 0b01111000: // 3.6.0 SEI - Set interrupt-disable flag
            return &SEI;
        
        case 0b11011000: // 6.6.0 CLD - Clear decimal mode
            return &CLD;
        
        case 0b11111000: // 7.6.0 SED - Set decimal mode
            return &SED;
        
        case 0b10111000: // 5.6.0 CLV - Clear overflow flag
            return &CLV;
        
        case 0b10101010: // 5.2.2 TAX - Transfer A to X
            return &TAX;

        case 0b10101000: // 5.2.0 TAY - Transfer A to Y
            return &TAY;
        
        case 0b10111010: // 5.6.2 TSX - Transfer stack pointer to X
            return &TSX;
        
        case 0b10001010: // 4.2.2 TXA - Transfer X to A
            return &TXA;
        
        case 0b10011010: // 4.6.2 TXS - Transfer X to stack pointer
            return &TXS;
        
        case 0b10011000: // 4.6.0 TYA - Transfer Y to A
            return &TYA;

        case 0b11101010: // 7.2.2 NOP - No operation
            return &NOP;
        
        
        default:
            break;
    }

    throw std::invalid_argument("Unknown opcode: " + std::to_string(opcode));
}

////////////////////////////////////////////////////////////////////////
// Micro Instructions. Anything that can happen in a single clk cycle //
////////////////////////////////////////////////////////////////////////
void CPU6502::fetchAddrL() { 
    addrL = read(PC); 
    PC++;

    IAL = addrL;
}

void CPU6502::fetchAddrLFromZeroPageIAL() {
    addrL = read(0x00FF & IAL);
    IAL++;
}

void CPU6502::fetchAddrHAddXToAddrLAndSkipNextStepIfPageCrossed(){
    addrH = read(PC);
    PC++;
    
    // No "oops" cycle -> skip 1 cycle
    if(addrL + X <= 255){
        currentInstructionStep++;
    } else {
        pageCrossed = PAGE_CROSS_STATUS::CROSS_UP;
    }

    IAL += X;
}

void CPU6502::fetchAddrHAddYToAddrLAndSkipNextStepIfPageCrossed(){
    addrH = read(PC);
    PC++;
    
    // No "oops" cycle -> skip 1 cycle
    if(addrL + Y <= 255){
        currentInstructionStep++;
    } else {
        pageCrossed = PAGE_CROSS_STATUS::CROSS_UP;
    }

    IAL += Y;
}

void CPU6502::fetchAddrH() { 
    addrH = read(PC); 
    PC++;
}

// Must be called after 'fetchAddrLFromZeroPageIAL'
void CPU6502::fetchAddrHFromZeroPageIAL() {
    addrH = read(0x00FF & IAL);
    IAL = addrL;
}

void CPU6502::fetchAddrHFromZeroPageIALAddYToAddrLAndSkipNextStepIfPageCrossed(){
    addrH = read(0x00FF & IAL);

    // No "oops" cycle -> skip 1 cycle
    if(addrL + Y <= 255){
        currentInstructionStep++;
    } else {
        pageCrossed = PAGE_CROSS_STATUS::CROSS_UP;
    }

    addrL += Y;
    IAL = addrL;
}

void CPU6502::fetchAddrHAddXToAddrLAndCheckPageCross(){
    addrH = read(PC);
    PC++;
    
    // No "oops" cycle -> skip 1 cycle
    if(addrL + X > 255){
        pageCrossed = PAGE_CROSS_STATUS::CROSS_UP;
    }

    IAL += X;
}

void CPU6502::fetchAddrHAddYToAddrLAndCheckPageCross(){
    addrH = read(PC);
    PC++;
    
    // No "oops" cycle -> skip 1 cycle
    if(addrL + Y > 255){
        pageCrossed = PAGE_CROSS_STATUS::CROSS_UP;
    }

    addrL += Y;
    IAL = addrL;
}

void CPU6502::fetchAddrHFromZeroPageIALAddYToAddrLAndCheckPageCross(){
    addrH = read(0x00FF & IAL);

    // No "oops" cycle -> skip 1 cycle
    if(addrL + Y > 255){
        pageCrossed = PAGE_CROSS_STATUS::CROSS_UP;
    }

    addrL += Y;
    IAL = addrL;
}

void CPU6502::loadAImmediate() {
    A = read(PC); 
    updateFlagN(A); 
    updateFlagZ(A);
    PC++;
}

void CPU6502::loadAZeroPage() { 
    A = read(0x00FF & IAL); 
    updateFlagN(A);
    updateFlagZ(A);
}

void CPU6502::loadAAbsolute(){
    addrL = IAL;
    A = read(getAddr());
    updateFlagN(A);
    updateFlagZ(A);
}

void CPU6502::loadXImmediate() {
    X = read(PC); 
    updateFlagN(X); 
    updateFlagZ(X);
    PC++;
}

void CPU6502::loadXZeroPage() { 
    X = read(0x00FF & addrL); 
    updateFlagN(X);
    updateFlagZ(X);
}

void CPU6502::loadXZeroPageY() { 
    X = read(0x00FF & IAL); 
    updateFlagN(X);
    updateFlagZ(X);
}

void CPU6502::loadXAbsolute(){
    addrL = IAL;
    X = read(getAddr());
    updateFlagN(X);
    updateFlagZ(X);
}



void CPU6502::loadYImmediate() {
    Y = read(PC); 
    updateFlagN(Y); 
    updateFlagZ(Y);
    PC++;
}

void CPU6502::loadYZeroPage() { 
    Y = read(0x00FF & addrL); 
    updateFlagN(Y);
    updateFlagZ(Y);
}

void CPU6502::loadYZeroPageX() { 
    Y = read(0x00FF & IAL); 
    updateFlagN(Y);
    updateFlagZ(Y);
}

void CPU6502::loadYAbsolute(){
    addrL = IAL;
    Y = read(getAddr());
    updateFlagN(Y);
    updateFlagZ(Y);
}



void CPU6502::loadIDLZeroPage(){
    IDL = read(0x00FF & IAL);
}

void CPU6502::loadIDLAbsolute(){
    addrL = IAL;
    IDL = read(getAddr());
}



void CPU6502::storeAZeroPage(){
    write(0x00FF & IAL, A);
}

void CPU6502::storeAAbsolute(){
    addrL = IAL;
    write(getAddr(), A);
}



void CPU6502::storeXZeroPage(){
    write(0x00FF & IAL, X);
}

void CPU6502::storeXAbsolute(){
    addrL = IAL;
    write(getAddr(), X);
}



void CPU6502::storeYZeroPage(){
    write(0x00FF & IAL, Y);
}

void CPU6502::storeYAbsolute(){
    addrL = IAL;
    write(getAddr(), Y);
}



void CPU6502::storeIDLZeroPage(){
    write(0x00FF & IAL, IDL);
}

void CPU6502::storeIDLAbsolute(){
    addrL = IAL;
    write(getAddr(), IDL);
}



void CPU6502::addXToAddrL() { 
    read(0x00FF & addrL); // Dummy read
    IAL = addrL + X;
}

void CPU6502::addYToAddrL() { 
    read(0x00FF & addrL); // Dummy read
    IAL = addrL + Y; 
}

void CPU6502::fixAddrHighByte(){
    addrL = IAL;
    read(getAddr()); // Dummy read

    switch(pageCrossed){
        case PAGE_CROSS_STATUS::NO_CROSS:
            return;

        case PAGE_CROSS_STATUS::CROSS_UP:
            addrH++;
            pageCrossed = PAGE_CROSS_STATUS::NO_CROSS;
            return;

        case PAGE_CROSS_STATUS::CROSS_DOWN:
            addrH--;
            pageCrossed = PAGE_CROSS_STATUS::NO_CROSS;
            return;
    }
}



void CPU6502::transferAX(){
    read(PC); // Dummy read
    X = A;
    updateFlagN(A);
    updateFlagZ(A);
}

void CPU6502::transferAY(){
    read(PC); // Dummy read
    Y = A;
    updateFlagN(A);
    updateFlagZ(A);
}

void CPU6502::transferXA(){
    read(PC); // Dummy read
    A = X;
    updateFlagN(X);
    updateFlagZ(X);
}

void CPU6502::transferYA(){
    read(PC); // Dummy read
    A = Y;
    updateFlagN(Y);
    updateFlagZ(Y);
}

void CPU6502::transferSX(){
    read(PC); // Dummy read
    X = SP;
    updateFlagN(SP);
    updateFlagZ(SP);
}

void CPU6502::transferXS(){
    read(PC); // Dummy read
    SP = X;
    updateFlagN(X);
    updateFlagZ(X);
}



void CPU6502::nop(){
    read(PC); // Dummy read
}

void CPU6502::unknown(){
    // Cycle 3 of instruction JSR
    // I couldn't find documentation for what this does
}

void CPU6502::incrementPC(){
    PC++;
}



void CPU6502::pushA(){
    write(0x100 + SP, A);
    SP--;
}

void CPU6502::pullA(){
    A = read(0x100 + SP);
}

void CPU6502::pushStatus(){
    write(0x100 + SP, status | 0b00010000); // B flag is always pushed as 1
    SP--;
}

void CPU6502::pullStatus(){
    // Bits 4 and 5 are always ignored on pull
    status = (0b11001111 & read(0x100 + SP)) | (0b00110000 & status) ;
}

void CPU6502::pushPCH(){
    write(0x100 + SP, (0xFF00 & PC));
    SP--;
}

void CPU6502::pullPCH(){
    setPCH(read(0x100 + SP));
}

void CPU6502::pushPCL(){
    write(0x100 + SP, (0x00FF & PC));
    SP--;
}

void CPU6502::pullPCL(){
    setPCL(read(0x100 + SP));
    SP++;
}

void CPU6502::incrementSP(){
    SP++;
}



void CPU6502::clearC(){
    read(PC); // Dummy read
    clearFlagC();
}

void CPU6502::clearD(){
    read(PC); // Dummy read
    clearFlagD();
}

void CPU6502::clearI(){
    read(PC); // Dummy read
    clearFlagI();
}

void CPU6502::clearV(){
    read(PC); // Dummy read
    clearFlagV();
}

void CPU6502::setC(){
    read(PC); // Dummy read
    setFlagC();
}

void CPU6502::setD(){
    read(PC); // Dummy read
    setFlagD();
}

void CPU6502::setI(){
    read(PC); // Dummy read
    setFlagI();
}



void CPU6502::incrementX(){
    read(PC); // Dummy read
    X++;
    updateFlagN(X);
    updateFlagZ(X);
}

void CPU6502::incrementY(){
    read(PC); // Dummy read
    Y++;
    updateFlagN(Y);
    updateFlagZ(Y);
}

void CPU6502::incrementIDL(){
    write(addrH<<8 | IAL, IDL); // Dummy write
    IDL++;
    updateFlagN(IDL);
    updateFlagZ(IDL);
}

void CPU6502::decrementX(){
    read(PC); // Dummy read
    X--;
    updateFlagN(X);
    updateFlagZ(X);
}

void CPU6502::decrementY(){
    read(PC); // Dummy read
    Y--;
    updateFlagN(Y);
    updateFlagZ(Y);
}

void CPU6502::decrementIDL(){
    write(addrH<<8 | IAL, IDL); // Dummy write
    IDL--;
    updateFlagN(IDL);
    updateFlagZ(IDL);
}

void CPU6502::andAImmediate(){
    A &= read(PC); 
    updateFlagN(A);
    updateFlagZ(A);
    PC++;
}

void CPU6502::andAZeroPage(){
    A &= read(0x00FF & IAL); 
    updateFlagN(A);
    updateFlagZ(A);
}

void CPU6502::andAAbsolute(){
    addrL = IAL;
    A &= read(getAddr()); 
    updateFlagN(A);
    updateFlagZ(A);
}



void CPU6502::orAImmediate(){
    A |= read(PC); 
    updateFlagN(A);
    updateFlagZ(A);
    PC++;
}

void CPU6502::orAZeroPage(){
    A |= read(0x00FF & IAL); 
    updateFlagN(A);
    updateFlagZ(A);
}

void CPU6502::orAAbsolute(){
    addrL = IAL;
    A |= read(getAddr()); 
    updateFlagN(A);
    updateFlagZ(A);
}



void CPU6502::xorAImmediate(){
    A ^= read(PC); 
    updateFlagN(A);
    updateFlagZ(A);
    PC++;
}

void CPU6502::xorAZeroPage(){
    A ^= read(0x00FF & IAL); 
    updateFlagN(A);
    updateFlagZ(A);
}

void CPU6502::xorAAbsolute(){
    addrL = IAL;
    A ^= read(getAddr()); 
    updateFlagN(A);
    updateFlagZ(A);
}



void CPU6502::bitTestZeroPage(){
    IDL = read(0x00FF & IAL);
    byte_t test = IDL & A;
    updateFlagZ(test);
    if(0b10000000 & IDL) setFlagN();
    if(0b01000000 & IDL) setFlagV();
}

void CPU6502::bitTestAbsolute(){
    addrL = IAL;
    IDL = read(getAddr());
    byte_t test = IDL & A;
    updateFlagZ(test);
    if(0b10000000 & IDL) setFlagN();
    if(0b01000000 & IDL) setFlagV();
}



void CPU6502::compareAImmediate(){
    byte_t operand = read(PC); 
    if(A>=operand) setFlagC();
    if(A==operand) setFlagZ();
    updateFlagN(A-operand);
    PC++;
}

void CPU6502::compareAZeroPage(){
    byte_t operand = read(0x00FF & IAL); 
    if(A>=operand) setFlagC();
    if(A==operand) setFlagZ();
    updateFlagN(A-operand);
}

void CPU6502::compareAAbsolute(){
    addrL = IAL;
    byte_t operand = read(getAddr()); 
    if(A>=operand) setFlagC();
    if(A==operand) setFlagZ();
    updateFlagN(A-operand);
}



void CPU6502::compareXImmediate(){
    byte_t operand = read(PC); 
    if(X>=operand) setFlagC();
    if(X==operand) setFlagZ();
    updateFlagN(X-operand);
    PC++;
}

void CPU6502::compareXZeroPage(){
    byte_t operand = read(0x00FF & IAL); 
    if(X>=operand) setFlagC();
    if(X==operand) setFlagZ();
    updateFlagN(X-operand);
}

void CPU6502::compareXAbsolute(){
    addrL = IAL;
    byte_t operand = read(getAddr()); 
    if(X>=operand) setFlagC();
    if(X==operand) setFlagZ();
    updateFlagN(X-operand);
}



void CPU6502::compareYImmediate(){
    byte_t operand = read(PC); 
    if(Y>=operand) setFlagC();
    if(Y==operand) setFlagZ();
    updateFlagN(Y-operand);
    PC++;
}

void CPU6502::compareYZeroPage(){
    byte_t operand = read(0x00FF & IAL); 
    if(Y>=operand) setFlagC();
    if(Y==operand) setFlagZ();
    updateFlagN(Y-operand);
}

void CPU6502::compareYAbsolute(){
    addrL = IAL;
    byte_t operand = read(getAddr()); 
    if(Y>=operand) setFlagC();
    if(Y==operand) setFlagZ();
    updateFlagN(Y-operand);
}



void CPU6502::jmpAbsolute(){
    setPCH(read(PC));
    setPCL(addrL);
}

void CPU6502::fetchPCLToIDL(){
    IDL = read(getAddr());
}

void CPU6502::jmpIndirect(){
    byte_t PCH = read(((addrH) << 8) + ((IAL+1)%256));
    byte_t PCL = IDL;

    setPCH(PCH);
    setPCL(PCL);
}



// Auxiliary function for branching, since branch operand can be negative
int8_t toSignedByte(uint8_t b){
    return static_cast<int8_t>(b);
}

void CPU6502::branch(){
    uint8_t PCL = PC & 0x00FF;
    int result = PCL + toSignedByte(IDL);
    
    if(result < 0){
        pageCrossed = PAGE_CROSS_STATUS::CROSS_DOWN;
    } else if(result > 255){
        pageCrossed = PAGE_CROSS_STATUS::CROSS_UP;
    } else {
        currentInstructionStep++;
    }

    setPCL(result);
}

void CPU6502::fixPCHighByte(){
    read(PC); // Dummy read

    switch(pageCrossed){
        case PAGE_CROSS_STATUS::NO_CROSS:
            return;

        case PAGE_CROSS_STATUS::CROSS_UP:
            PC += 0x0100;
            pageCrossed = PAGE_CROSS_STATUS::NO_CROSS;
            return;

        case PAGE_CROSS_STATUS::CROSS_DOWN:
            PC -= 0x0100;
            pageCrossed = PAGE_CROSS_STATUS::NO_CROSS;
            return;
    }
}

void CPU6502::checkBCCCondition(){
    IDL = read(PC);
    PC++;

    // Contition not met -> skip to end of instruction
    if(getFlagC()){
        currentInstructionStep += 2;
    }
}