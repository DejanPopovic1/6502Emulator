//
// Created by DejanP on 2021/04/08.
//

#include "gtest/gtest.h"
#include "../main.h"

class M6502Test1 : public testing::Test
{
public:
    Mem mem;
    CPU cpu;
    virtual void SetUp(){
        cpu.Reset(mem);
    }

    virtual void TearDown(){

    }
};

TEST_F(M6502Test1, LDAImmediateCanLoadAValueIntoTheARegister) {
    mem[0xFFFC] = CPU::INS_LDA_IM;
    mem[0xFFFD] = 0x84;
    CPU CPUCopy = cpu;
    s32 CyclesUsed = cpu.Execute(2, mem);
    EXPECT_EQ(cpu.A, 0x84);
    EXPECT_EQ(CyclesUsed, 2);
    EXPECT_FALSE(cpu.Z);
    EXPECT_TRUE(cpu.N);
    EXPECT_EQ(cpu.C, CPUCopy.C);
    EXPECT_EQ(cpu.I, CPUCopy.I);
    EXPECT_EQ(cpu.D, CPUCopy.D);
    EXPECT_EQ(cpu.B, CPUCopy.B);
    EXPECT_EQ(cpu.V, CPUCopy.V);
}

TEST_F(M6502Test1, LDAZeroPageCanLoadAValueIntoTheARegister) {
    mem[0xFFFC] = CPU::INS_LDA_ZP;
    mem[0xFFFD] = 0x42;
    mem[0x0042] = 0x37;
    CPU CPUCopy = cpu;
    s32 CyclesUsed = cpu.Execute(3, mem);
    EXPECT_EQ(cpu.A, 0x37);
    EXPECT_EQ(CyclesUsed, 3);
    EXPECT_FALSE(cpu.Z);
    EXPECT_FALSE(cpu.N);
    EXPECT_EQ(cpu.C, CPUCopy.C);
    EXPECT_EQ(cpu.I, CPUCopy.I);
    EXPECT_EQ(cpu.D, CPUCopy.D);
    EXPECT_EQ(cpu.B, CPUCopy.B);
    EXPECT_EQ(cpu.V, CPUCopy.V);
}

TEST_F(M6502Test1, LDAZeroPageXCanLoadAValueIntoTheARegister) {
    cpu.X = 5;
    mem[0xFFFC] = CPU::INS_LDA_ZPX;
    mem[0xFFFD] = 0x42;
    mem[0x0047] = 0x37;
    CPU CPUCopy = cpu;
    s32 CyclesUsed = cpu.Execute(4, mem);
    EXPECT_EQ(cpu.A, 0x37);
    EXPECT_EQ(CyclesUsed, 4);
    EXPECT_FALSE(cpu.Z);
    EXPECT_FALSE(cpu.N);
    EXPECT_EQ(cpu.C, CPUCopy.C);
    EXPECT_EQ(cpu.I, CPUCopy.I);
    EXPECT_EQ(cpu.D, CPUCopy.D);
    EXPECT_EQ(cpu.B, CPUCopy.B);
    EXPECT_EQ(cpu.V, CPUCopy.V);
}

TEST_F(M6502Test1, LDAZeroPageXCanLoadAValueIntoTheARegisterWhenItWraps) {
    cpu.X = 0xFF;
    mem[0xFFFC] = CPU::INS_LDA_ZPX;
    mem[0xFFFD] = 0x80;
    mem[0x007F] = 0x37;
    CPU CPUCopy = cpu;
    s32 CyclesUsed = cpu.Execute(4, mem);
    EXPECT_EQ(cpu.A, 0x37);
    EXPECT_EQ(CyclesUsed, 4);
    EXPECT_FALSE(cpu.Z);
    EXPECT_FALSE(cpu.N);
    EXPECT_EQ(cpu.C, CPUCopy.C);
    EXPECT_EQ(cpu.I, CPUCopy.I);
    EXPECT_EQ(cpu.D, CPUCopy.D);
    EXPECT_EQ(cpu.B, CPUCopy.B);
    EXPECT_EQ(cpu.V, CPUCopy.V);
}

//// Tests factorial of 0.
//TEST(FactorialTest, HandlesZeroInput) {
//EXPECT_EQ(1, 1);
//}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
