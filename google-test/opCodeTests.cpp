#include "gtest/gtest.h"

#include "../Processor.h"
#include "../Memory.h"

#define RESET_LO_ADDR 0xFFFC
#define RESET_HI_ADDR 0xFFFD

//Given
class OpCodeTest : public testing::Test
{
public:
    Memory mem;
    Processor cpu;
    uint16_t addr_abs = 0x0200;
    uint8_t addr_rel = 0x71;
    virtual void SetUp(){
        mem[RESET_LO_ADDR] = 0x00;
        mem[RESET_HI_ADDR] = 0x02;
        mem[0x0200] = 0x05;
        mem[0x0201] = 0x06;
        cpu.connectMemory(&mem);
        cpu.reset();
        cpu.A = 0x61;
        cpu.X = 0x07;
        cpu.Y = 0x11;
    }
    virtual void TearDown(){
    }
};

TEST_F(OpCodeTest, addWithCarry_performAdditionalOrSubtractionOnAccumulatorWithAbsAddr_assignCZNVflags_additionalCycle) {
    //When
    int additionalCycles = cpu.ADC(addr_abs, addr_rel);
    //Then
    EXPECT_EQ(cpu.A, 0x66);
    EXPECT_EQ(cpu.getFlag(Processor::C), 0);
    EXPECT_EQ(cpu.getFlag(Processor::Z), 0);
    EXPECT_EQ(cpu.getFlag(Processor::N), 0);
    EXPECT_EQ(cpu.getFlag(Processor::V), 0);
    EXPECT_EQ(additionalCycles, 1);
    //Given
    cpu.A = 0xFD;
    //When
    additionalCycles = cpu.ADC(addr_abs, addr_rel);
    //Then
    EXPECT_EQ(cpu.A, 0x02);
    EXPECT_EQ(cpu.getFlag(Processor::C), 1);
    EXPECT_EQ(cpu.getFlag(Processor::Z), 0);
    EXPECT_EQ(cpu.getFlag(Processor::N), 0);
    EXPECT_EQ(cpu.getFlag(Processor::V), 0);
    //Given
    cpu.A = 0xFB;
    //When
    additionalCycles = cpu.ADC(addr_abs, addr_rel);
    //Then
    EXPECT_EQ(cpu.A, 0x00);
    EXPECT_EQ(cpu.getFlag(Processor::C), 1);
    EXPECT_EQ(cpu.getFlag(Processor::Z), 1);
    EXPECT_EQ(cpu.getFlag(Processor::N), 0);
    EXPECT_EQ(cpu.getFlag(Processor::V), 0);
}

//u8 Processor::ADC(u16 addr_abs, u8 addr_rel){
//    u8 fetched = fetch(addr_abs);
//    u16 temp = (u16)A + (u16)fetched + (u16)getFlag(C);
//    setOrClearFlag(C, temp > 255);
//    setOrClearFlag(Z, (temp & 0x00FF) == 0);
//    setOrClearFlag(N, temp & 0x80);
//    setOrClearFlag(V, (~((u16)A ^ (u16)fetched) & ((u16)A ^ (u16)temp)) & 0x0080);
//    A = temp & 0x00FF;
//    return 1;
//}