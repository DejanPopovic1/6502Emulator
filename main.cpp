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

    void WriteWord(Word Value, u32 Address, u32 & Cycles){
        Data[Address] = Value & 0xFF;
        Data[Address + 1] = (Value >> 8);
        Cycles -= 2;
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

    Byte FetchWord(u32 & Cycles, Mem & memory){
        Byte Data = memory[PC];
        PC++;
        Data |= (memory[PC] << 8);        //Include a #IF to handle the endianness cases of the running platform
        PC++;
        Cycles -= 2;

        return Data;

    }

    static constexpr Byte
    INS_LDA_IM = 0xA9,
    INS_LDA_ZP = 0xA5,
    INS_LDA_ZPX = 0xB5,
    INS_JSR = 0x20;

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
                case INS_JSR:
                {
                    Word SubAddr = FetchWord(Cycles, memory);
                    memory.WriteWord(PC - 1, SP, Cycles);
                    Cycles--;
                    PC = SubAddr;
                    Cycles--;
                    SP++;
                } break;
                default:
                {
                    std::cout << "Instruction not handled" << std::endl;
                } break;




            }
        }
    }
};

//// Demonstrate some basic assertions.
//TEST(HelloTest, BasicAssertions) {
//// Expect two strings not to be equal.
//EXPECT_STRNE("hello", "world");
//// Expect equality.
//EXPECT_EQ(7 * 6, 42);
//}
//
void test(){

}

int main() {
    //test();
    Mem mem;
    CPU cpu;
    cpu.Reset(mem);
    mem[0xFFFC] = CPU::INS_JSR;
    mem[0xFFFD] = 0x42;
    mem[0xFFFE] = 0x42;
    mem[0x4242] = CPU::INS_LDA_IM;
    mem[0x4243] = 0x84;
    cpu.Execute(7, mem);
    std::string test;
    //::testing::InitGoogleTest();
    //return RUN_ALL_TESTS();
    std::cin >> test;
    return 0;
}
