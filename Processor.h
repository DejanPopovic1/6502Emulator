#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <cstdint>
#include <vector>
#include <string>

#define BIT_ZERO    (1 << 0)
#define BIT_ONE     (1 << 1)
#define BIT_TWO     (1 << 2)
#define BIT_THREE   (1 << 3)
#define BIT_FOUR    (1 << 4)
#define BIT_FIVE    (1 << 5)
#define BIT_SIX     (1 << 6)
#define BIT_SEVEN   (1 << 7)

//This works, but is technically undefined behaviour and a better option is to use friend classes
#if IS_TESTING
#define private public
#endif

class Processor;
class Memory;
struct Instruction;

using u8 = uint8_t;
using u16 = uint16_t;

//Rename the first function pointer as "opcode"
struct Instruction{
    uint8_t(Processor::*operate)(u16 addr_abs, u8 addr_rel) = nullptr;
    uint8_t(Processor::*addrmode)(u16 &PC, u16 &addr_abs, u8 &addr_rel) = nullptr;
    uint8_t cycles = 0;
};

class Processor{

public:
    enum validFlagBits{C = BIT_ZERO, Z = BIT_ONE, I = BIT_TWO, D = BIT_THREE, B = BIT_FOUR, U = BIT_FIVE, V = BIT_SIX, N = BIT_SEVEN};
    Processor();
    ~Processor();
    void connectMemory(Memory *mem);
    //Addressing Modes
    u8 IMP(u16 &PC, u16 &addr_abs, u8 &addr_rel);
    u8 IMM(u16 &PC, u16 &addr_abs, u8 &addr_rel);
    u8 ABS(u16 &PC, u16 &addr_abs, u8 &addr_rel);
    u8 ZPA(u16 &PC, u16 &addr_abs, u8 &addr_rel);
    u8 ZPX(u16 &PC, u16 &addr_abs, u8 &addr_rel);
    u8 ZPY(u16 &PC, u16 &addr_abs, u8 &addr_rel);
    u8 ABX(u16 &PC, u16 &addr_abs, u8 &addr_rel);
    u8 ABY(u16 &PC, u16 &addr_abs, u8 &addr_rel);
    u8 REL(u16 &PC, u16 &addr_abs, u8 &addr_rel);
    u8 IIX(u16 &PC, u16 &addr_abs, u8 &addr_rel);
    u8 IIY(u16 &PC, u16 &addr_abs, u8 &addr_rel);
    u8 IND(u16 &PC, u16 &addr_abs, u8 &addr_rel);
    //Load Operations
    u8 LDA(u16 addr_abs, u8 addr_rel); u8 LDX(u16 addr_abs, u8 addr_rel); u8 LDY(u16 addr_abs, u8 addr_rel); u8 STA(u16 addr_abs, u8 addr_rel); u8 STX(u16 addr_abs, u8 addr_rel); u8 STY(u16 addr_abs, u8 addr_rel);
    //Register Transfer Operations
    u8 TAX(u16 addr_abs, u8 addr_rel); u8 TAY(u16 addr_abs, u8 addr_rel); u8 TXA(u16 addr_abs, u8 addr_rel); u8 TYA(u16 addr_abs, u8 addr_rel);
    //Stack Operations
    u8 TSX(u16 addr_abs, u8 addr_rel); u8 TXS(u16 addr_abs, u8 addr_rel); u8 PHA(u16 addr_abs, u8 addr_rel); u8 PHP(u16 addr_abs, u8 addr_rel); u8 PLA(u16 addr_abs, u8 addr_rel); u8 PLP(u16 addr_abs, u8 addr_rel);
    //Logical Operations
    u8 AND(u16 addr_abs, u8 addr_rel); u8 EOR(u16 addr_abs, u8 addr_rel); u8 ORA(u16 addr_abs, u8 addr_rel); u8 BIT(u16 addr_abs, u8 addr_rel);
    //Arithmetic Operations
    u8 ADC(u16 addr_abs, u8 addr_rel); u8 SBC(u16 addr_abs, u8 addr_rel); u8 CMP(u16 addr_abs, u8 addr_rel); u8 CPX(u16 addr_abs, u8 addr_rel); u8 CPY(u16 addr_abs, u8 addr_rel);
    //Increment Decrement Operations
    u8 INC(u16 addr_abs, u8 addr_rel); u8 INX(u16 addr_abs, u8 addr_rel); u8 INY(u16 addr_abs, u8 addr_rel); u8 DEC(u16 addr_abs, u8 addr_rel); u8 DEX(u16 addr_abs, u8 addr_rel); u8 DEY(u16 addr_abs, u8 addr_rel);
    //Shift Operations
    u8 ASL(u16 addr_abs, u8 addr_rel); u8 LSR(u16 addr_abs, u8 addr_rel); u8 ROL(u16 addr_abs, u8 addr_rel); u8 ROR(u16 addr_abs, u8 addr_rel);
    //Jump & Call Operations
    u8 JMP(u16 addr_abs, u8 addr_rel); u8 JSR(u16 addr_abs, u8 addr_rel); u8 RTS(u16 addr_abs, u8 addr_rel);
    //Branch Operations
    u8 BCC(u16 addr_abs, u8 addr_rel); u8 BCS(u16 addr_abs, u8 addr_rel); u8 BEQ(u16 addr_abs, u8 addr_rel); u8 BMI(u16 addr_abs, u8 addr_rel); u8 BNE(u16 addr_abs, u8 addr_rel); u8 BPL(u16 addr_abs, u8 addr_rel); u8 BVC(u16 addr_abs, u8 addr_rel); u8 BVS(u16 addr_abs, u8 addr_rel);
    //Status Change Operations
    u8 CLC(u16 addr_abs, u8 addr_rel); u8 CLD(u16 addr_abs, u8 addr_rel); u8 CLI(u16 addr_abs, u8 addr_rel); u8 CLV(u16 addr_abs, u8 addr_rel); u8 SEC(u16 addr_abs, u8 addr_rel); u8 SED(u16 addr_abs, u8 addr_rel); u8 SEI(u16 addr_abs, u8 addr_rel);
    //SysFunctions Operations
    u8 BRK(u16 addr_abs, u8 addr_rel); u8 NOP(u16 addr_abs, u8 addr_rel); u8 RTI(u16 addr_abs, u8 addr_rel);
    //Invalid Operations
    u8 XXX(u16 addr_abs, u8 addr_rel);
    //
    void clock();
    void reset();
    void irq();
    void nmi();
    u8 fetch(uint16_t addr_abs);
    u8 opcode;
    u8 cycles;
    u8 status;
private:
    Memory *mem = nullptr;
    u8 read(u16 a);
    void write(u16 a, u8 d);
    u8 A, X, Y;
    u8 SP;
    u16 PC;
    u8 getFlag(enum validFlagBits);
    void setFlag(enum validFlagBits);
    void clearFlag(enum validFlagBits);
    //For this whole class, instead of class definition initialisation rather intialise as part of the constructor
    struct Instruction i;
    std::vector<struct Instruction> lookup;
    void setOrClearFlag(enum Processor::validFlagBits f, bool isSet);
};

#endif //PROCESSOR_H
