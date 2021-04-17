#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <cstdint>
#include <vector>
#include <string>

#define BIT_ZERO (1 << 0)
#define BIT_ONE (1 << 1)
#define BIT_TWO (1 << 2)
#define BIT_THREE (1 << 3)
#define BIT_FOUR (1 << 4)
#define BIT_FIVE (1 << 5)
#define BIT_SIX (1 << 6)
#define BIT_SEVEN (1 << 7)

using u8 = uint8_t;
using u16 = uint16_t;

class Memory;

class Processor{

public:
    //Macro each of the numbers because they are sometimes used by themselves
    enum flagsRegister{C = BIT_ZERO, Z = BIT_ONE, I = BIT_TWO, D = BIT_THREE, B = BIT_FOUR, U = BIT_FIVE, V = BIT_SIX, N = BIT_SEVEN};
    Processor();
    ~Processor();
    void connectMemory(Memory *mem);
    //Addressing Modes
    //Rename these in more logical fashion according to the Rockwell data sheet
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

    //OpCodes
    //Add descriptions of these in the file with a special escape character
    //Rather store these in a seperate file so that when function pointers point to them, we dont use class function pointers
    //Load/Store Operations | Register Transfers | Stack Operations | Logical        | Arithmetic     | Increments & Decrements | Shifts         | Jumps & Calls  | Branches       | Status Flag Changes | System Functions
    u8 LDA();            u8 TAX();       u8 TSX();     u8 AND();   u8 ADC();   u8 INC();            u8 ASL();   u8 JMP();   u8 BCC();   u8 CLC();        u8 BRK();
    u8 LDX();            u8 TAY();       u8 TXS();     u8 EOR();   u8 SBC();   u8 INX();            u8 LSR();   u8 JSR();   u8 BCS();   u8 CLD();        u8 NOP();
    u8 LDY();            u8 TXA();       u8 PHA();     u8 ORA();   u8 CMP();   u8 INY();            u8 ROL();   u8 RTS();   u8 BEQ();   u8 CLI();        u8 RTI();
    u8 STA();            u8 TYA();       u8 PHP();     u8 BIT();   u8 CPX();   u8 DEC();            u8 ROR();               u8 BMI();   u8 CLV();
    u8 STX();                            u8 PLA();                 u8 CPY();   u8 DEX();                                    u8 BNE();   u8 SEC();
    u8 STY();                            u8 PLP();                             u8 DEY();                                    u8 BPL();   u8 SED();
    u8 BVC();                            u8 SEI();
    u8 BVS();
    u8 XXX();
    void clock();
    void reset();
    void irq();
    void nmi();
    u8 fetch();
    u8 fetched = 0x00;
    u16 addr_abs = 0x0000;
    u16 addr_rel = 0x00;//Check if this can be defined as a Byte instead
    u8 opcode = 0;
    u8 cycles = 0;
    //If the internal inhibit(I am assuming that this is the IRQ Disable flag) is set, then the interrupt is ignored.
    u8 status = 0x00;

private:
    Memory *mem = nullptr;
    u8 read(u16 a);
    void write(u16 a, u8 d);
    //The upper significant 8 bits are hardcoded to 1. i.e. SP value is always 0000001xxxxxxxx
    //The range of the SP is from
    u8 A, X, Y;
    u8 SP;
    u16 PC;
    flagsRegister flagsRegister;
    u8 getFlag(enum flagsRegister);
    void setFlag(enum flagsRegister);
    void clearFlag(enum flagsRegister);
    //For this whole class, instead of class definition initialisation rather intialise as part of the constructor
    struct Instruction{
        std::string name;
        uint8_t(Processor::*operate)() = nullptr;
        uint8_t(Processor::*addrmode)() = nullptr;
        uint8_t cycles = 0;
    };
    std::vector<Instruction> lookup;
    void setOrClearFlag(enum Processor::flagsRegister f, bool isSet);
};


#endif //PROCESSOR_H
