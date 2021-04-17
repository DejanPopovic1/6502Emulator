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

class Memory;
class Processor;

struct Instruction{
    uint8_t(Processor::*operate)() = nullptr;
    uint8_t(Processor::*addrmode)() = nullptr;
    uint8_t cycles = 0;
};

using u8 = uint8_t;
using u16 = uint16_t;


class Processor{

public:
    enum validFlagBits{C = BIT_ZERO, Z = BIT_ONE, I = BIT_TWO, D = BIT_THREE, B = BIT_FOUR, U = BIT_FIVE, V = BIT_SIX, N = BIT_SEVEN};
    Processor();
    ~Processor();
    void connectMemory(Memory *mem);
    //Addressing Modes
    u8 IMP();
    u8 IMM();
    u8 ABS();
    u8 ZPA();
    u8 ZPX();
    u8 ZPY();
    u8 ABX();
    u8 ABY();
    u8 REL();
    u8 IIX();
    u8 IIY();
    u8 IND();
    //Instructions
    //Load
    u8 LDA();
    u8 LDX();
    u8 LDY();
    u8 STA();
    u8 STX();
    u8 STY();
    //Register Transfers
    u8 TAX();
    u8 TAY();
    u8 TXA();
    u8 TYA();
    //Stack
    u8 TSX();
    u8 TXS();
    u8 PHA();
    u8 PHP();
    u8 PLA();
    u8 PLP();
    //Logical
    u8 AND();
    u8 EOR();
    u8 ORA();
    u8 BIT();
    //Arithmetic
    u8 ADC();
    u8 SBC();
    u8 CMP();
    u8 CPX();
    u8 CPY();
    //IncDecrements
    u8 INC();
    u8 INX();
    u8 INY();
    u8 DEC();
    u8 DEX();
    u8 DEY();
    //Shifts
    u8 ASL();
    u8 LSR();
    u8 ROL();
    u8 ROR();
    //JumpCalls
    u8 JMP();
    u8 JSR();
    u8 RTS();
    //Branches
    u8 BCC();
    u8 BCS();
    u8 BEQ();
    u8 BMI();
    u8 BNE();
    u8 BPL();
    u8 BVC();
    u8 BVS();
    //StatusChange
    u8 CLC();
    u8 CLD();
    u8 CLI();
    u8 CLV();
    u8 SEC();
    u8 SED();
    u8 SEI();
    //SysFunctions
    u8 BRK();
    u8 NOP();
    u8 RTI();
    //Invalid
    u8 XXX();
    //
    void clock();
    void reset();
    void irq();
    void nmi();
    u8 fetch();
    u16 addr_abs;
    u16 addr_rel;//Check if this can be defined as a Byte instead
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
