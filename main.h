#include <iostream>
#include <string>
#include <cstdint>

using Byte = unsigned char;
using Word = unsigned short;
using u32 = unsigned int;
using s32 = signed int;

struct Mem{
    static constexpr u32 MAX_MEM = 1024 * 64;
    Byte Data[MAX_MEM];

    void Initialise(){
        for(u32 i = 0; i < MAX_MEM; i++) {
            Data[i] = 0;
        }
    }

    Byte operator[](u32 Address) const{
        return Data[Address];
    }

    Byte & operator[](u32 Address){
        return Data[Address];
    }

    void WriteWord(Word Value, u32 Address, s32 & Cycles){//Dean Note, add endianness options
        Data[Address] = Value & 0xFF;
        Data[Address + 1] = (Value >> 8);
        Cycles -= 2;
    }
};

struct CPU{
public:
    //Addressing Modes
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
    //Load/Store Operations     Register Transfers      Stack Operations    Logical         Arithmetic      Increments & Decrements     Shifts          Jumps & Calls   Branches        Status Flag Changes     System Functions
    uint8_t LDA();              uint8_t TAX();          uint8_t TSX();      uint8_t AND();  uint8_t ADC();  uint8_t INC();              uint8_t ASL();  uint8_t JMP();  uint8_t BCC();  uint8_t CLC();          uint8_t BRK();
    uint8_t LDX();              uint8_t TAY();          uint8_t TXS();      uint8_t EOR();  uint8_t SBC();  uint8_t INX();              uint8_t LSR();  uint8_t JSR();  uint8_t BCS();  uint8_t CLD();          uint8_t NOP();
    uint8_t LDY();              uint8_t TXA();          uint8_t PHA();      uint8_t ORA();  uint8_t CMP();  uint8_t INY();              uint8_t ROL();  uint8_t RTS();  uint8_t BEQ();  uint8_t CLI();          uint8_t RTI();
    uint8_t STA();              uint8_t TYA();          uint8_t PHP();      uint8_t BIT();  uint8_t CPX();  uint8_t DEC();              uint8_t ROR();                  uint8_t BMI();  uint8_t CLV();
    uint8_t STX();                                      uint8_t PLA();                      uint8_t CPY();  uint8_t DEX();                                              uint8_t BNE();  uint8_t SEC();
    uint8_t STY();                                      uint8_t PLP();                                      uint8_t DEY();                                              uint8_t BPL();  uint8_t SED();
                                                                                                                                                                        uint8_t BVC();  uint8_t SEI();
                                                                                                                                                                        uint8_t BVS();





    //Memory layout of the 6502
    //Zero Page 0x0000 - 0x00FF (256) For zero page x, The zero page memory wraps around so anything higher than 0xFF goes to 0x00. Need to find out what happens in the case of normal memory (error?).
    //Stack 0x0100 - 0x01FF (256) Note there is stack overflow after 256 which should be accounted/tested for
    //Stack grows from 0x01FF downward
    //Address Space 0x0000 - 0xFFF
    //At the top of the address space are three vectors
    //FFFA/FFFB NMI
    //FFFC/FFFD RESET
    //FFFE/FFFF IRQ

    Word PC;
    Word SP;
    //The upper significant 8 bits are hardcoded to 1. i.e. SP value is always 0000001xxxxxxxx
    //The range of the SP is from
    uint8_t A, X, Y;
    uint8_t C : 1;
    uint8_t Z : 1;
    uint8_t I : 1;
    uint8_t D : 1;
    uint8_t B : 1;
    uint8_t V : 1;
    uint8_t N : 1;

    Byte ReadByte(s32 & Cycles, Word Address, Mem & memory){//Used to be Byte
        Byte Data = memory[Address];
        Cycles--;
        return Data;
    }

    void Reset(Mem & memory){
        PC = 0xFFFC;
        SP = 0x0100;
        C = Z = I = D = B = V = N = 0;
        A = X = Y = 0;
        memory.Initialise();
    }

    Byte FetchByte(s32 & Cycles, Mem & memory){
        Byte Data = memory[PC];
        PC++;
        Cycles--;
        return Data;
    }

    Word FetchWord(s32 & Cycles, Mem & memory){
        Word Data = memory[PC];//Used to be Byte
        PC++;
        Data |= (memory[PC] << 8);        //Include a #IF to handle the endianness cases of the running platform
        PC++;
        Cycles -= 2;

        return Data;

    }

    static constexpr uint8_t
            INS_LDA_IM = 0xA9,
            INS_LDA_ZP = 0xA5,
            INS_LDA_ZPX = 0xB5,
            INS_JSR = 0x20,
            INS_LDA_ABS = 0xAD,
            INS_LDA_ABSX = 0xBD;

    void LDASetStatus(){
        Z = (A == 0);
        N = (A & 0b10000000) > 0;
    }

    s32 Execute(s32 Cycles, Mem & memory){
        const s32 CyclesRequested = Cycles;
        while(Cycles > 0){
            Byte Ins = FetchByte(Cycles, memory);
            switch(Ins){
                case INS_LDA_IM:
                {
                    Byte Value = FetchByte(Cycles, memory);
                    A = Value;
                    LDASetStatus();
                    //break;
                } break;
                case INS_LDA_ZP:
                {
                    Byte ZeroPageAddr = FetchByte(Cycles, memory);
                    A = ReadByte(Cycles, ZeroPageAddr, memory);
                    LDASetStatus();
                    //break;
                } break;
                case INS_LDA_ZPX:
                {
                    Byte ZeroPageAddr = FetchByte(Cycles, memory);
                    ZeroPageAddr += X;
                    Cycles--;
                    A = ReadByte(Cycles, ZeroPageAddr, memory);
                    LDASetStatus();
                    //break;
                } break;
                case INS_JSR:
                {
                    Word SubAddr = FetchWord(Cycles, memory);
                    memory.WriteWord(PC - 1, SP, Cycles);
                    SP += 2;
                    PC = SubAddr;
                    std::cout << "Jump to Address: " << (int)PC << std::endl;
                    Cycles--;

                    //break;
                } break;
                default:
                {
                    std::cout << "Instruction not handled" << std::endl;
                    //break;
                } break;




            }
        }
        const s32 NumCyclesUsed = CyclesRequested - Cycles;
        return NumCyclesUsed;
    }
};
