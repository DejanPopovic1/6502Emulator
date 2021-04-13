//
// Created by DejanP on 2021/04/09.
//

#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <cstdint>
#include <vector>
#include <string>

using Byte = uint8_t;
using Word = uint16_t;
using u32 = uint32_t;
using s32 = int32_t;

class Memory;

class Processor{

public:
    //Refactor into bit fields
    enum flagsRegister{C = (1 << 0), Z = (1 << 1), I = (1 << 2), D = (1 << 3), B = (1 << 4), U = (1 << 5), V = (1 << 6), N = (1 << 7)};
    Processor();
    ~Processor();
    void connectMemory(Memory *mem);
    //Addressing Modes
    //Rename these in more logical fashion according to the Rockwell data sheet
    uint8_t IMP();
    uint8_t ZP0();
    uint8_t ZPY();
    uint8_t ABS();
    uint8_t ABY();
    uint8_t IZX();
    uint8_t IMM();
    uint8_t ZPX();
    uint8_t REL();
    uint8_t ABX();
    uint8_t IND();
    uint8_t IZY();
    //OpCodes
    //Add descriptions of these in the file with a special escape character
    //Rather store these in a seperate file so that when function pointers point to them, we dont use class function pointers
    //Load/Store Operations | Register Transfers | Stack Operations | Logical        | Arithmetic     | Increments & Decrements | Shifts         | Jumps & Calls  | Branches       | Status Flag Changes | System Functions
    uint8_t LDA();            uint8_t TAX();       uint8_t TSX();     uint8_t AND();   uint8_t ADC();   uint8_t INC();            uint8_t ASL();   uint8_t JMP();   uint8_t BCC();   uint8_t CLC();        uint8_t BRK();
    uint8_t LDX();            uint8_t TAY();       uint8_t TXS();     uint8_t EOR();   uint8_t SBC();   uint8_t INX();            uint8_t LSR();   uint8_t JSR();   uint8_t BCS();   uint8_t CLD();        uint8_t NOP();
    uint8_t LDY();            uint8_t TXA();       uint8_t PHA();     uint8_t ORA();   uint8_t CMP();   uint8_t INY();            uint8_t ROL();   uint8_t RTS();   uint8_t BEQ();   uint8_t CLI();        uint8_t RTI();
    uint8_t STA();            uint8_t TYA();       uint8_t PHP();     uint8_t BIT();   uint8_t CPX();   uint8_t DEC();            uint8_t ROR();                    uint8_t BMI();   uint8_t CLV();
    uint8_t STX();                                 uint8_t PLA();                      uint8_t CPY();   uint8_t DEX();                                              uint8_t BNE();   uint8_t SEC();
    uint8_t STY();                                 uint8_t PLP();                                       uint8_t DEY();                                              uint8_t BPL();   uint8_t SED();
                                                                                                                                                                    uint8_t BVC();   uint8_t SEI();
                                                                                                                                                                    uint8_t BVS();
    uint8_t XXX();
    void clock();
    void reset();
    void irq();
    void nmi();
    uint8_t fetch();
    uint8_t fetched = 0x00;
    Word addr_abs = 0x0000;
    Word addr_rel = 0x00;//Check if this can be defined as a Byte instead
    Byte opcode = 0;
    Byte cycles = 0;

    uint8_t status = 0x00;

private:
    Memory *mem = nullptr;
    uint8_t read(uint16_t a);
    void write(uint16_t a, uint8_t d);
    //The upper significant 8 bits are hardcoded to 1. i.e. SP value is always 0000001xxxxxxxx
    //The range of the SP is from
    Byte A, X, Y;
    Byte SP;
    Word PC;
    flagsRegister flagsRegister;

    Byte getFlag(enum flagsRegister);
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
