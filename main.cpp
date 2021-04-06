#include <iostream>
#include <string>

using Byte = unsigned char;
using Word = unsigned short;
using u32 = unsigned int;

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
};

struct CPU{



    Word PC;
    Word SP;
    Byte A, X, Y;
    Byte C : 1;
    Byte Z : 1;
    Byte I : 1;
    Byte D : 1;
    Byte B : 1;
    Byte V : 1;
    Byte N : 1;

    Byte ReadByte(u32 & Cycles, Byte Address, Mem & memory){
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

    Byte FetchByte(u32 & Cycles, Mem & memory){
        Byte Data = memory[PC];
        PC++;
        Cycles--;
        return Data;
    }

    static constexpr Byte
    INS_LDA_IM = 0xA9,
    INS_LDA_ZP = 0xA5,
    INS_LDA_ZPX = 0xB5;

    void LDASetStatus(){
        Z = (A == 0);
        N = (A & 0b10000000) > 0;
    }

    void Execute(u32 Cycles, Mem & memory){
        while(Cycles){
            Byte Ins = FetchByte(Cycles, memory);
            switch(Ins){
                case INS_LDA_IM:
                {
                    Byte Value = FetchByte(Cycles, memory);

                    LDASetStatus();

                } break;
                case INS_LDA_ZP:
                {
                    Byte ZeroPageAddr = FetchByte(Cycles, memory);
                    A = ReadByte(Cycles, ZeroPageAddr, memory);
                    LDASetStatus();
                } break;
                case INS_LDA_ZPX:
                {
                    Byte ZeroPageAddr = FetchByte(Cycles, memory);
                    ZeroPageAddr += X;
                    Cycles--;
                    A = ReadByte(Cycles, ZeroPageAddr, memory);
                    LDASetStatus();
                } break;
                default:
                {
                    std::cout << "Instruction not handled" << std::endl;
                } break;




            }
        }
    }
};

int main() {
    Mem mem;
    CPU cpu;
    cpu.Reset(mem);
    mem[0xFFFC] = CPU::INS_LDA_ZP;
    mem[0xFFFD] = 0x42;
    mem[0x0042] = 0x84;
    cpu.Execute(3, mem);
    std::string test;
    std::cin >> test;
    return 0;
}
