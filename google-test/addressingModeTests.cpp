#include "gtest/gtest.h"

#include "../Processor.h"
#include "../Memory.h"

#define RESET_LO_ADDR 0xFFFC
#define RESET_HI_ADDR 0xFFFD

//Given
class ProcessorTest : public testing::Test
{
public:
    Memory mem;
    Processor cpu;
    uint16_t addr_abs = 0x0042;
    uint8_t addr_rel = 0x42;
    virtual void SetUp(){
        cpu.connectMemory(&mem);
        mem[0x0005] = 0x99;
        mem[0x0006] = 0x95;
        mem[0x000C] = 0x78;
        mem[0x000D] = 0x79;
        mem[RESET_LO_ADDR] = 0x00;
        mem[RESET_HI_ADDR] = 0x02;
        mem[0x0200] = 0x05;
        mem[0x0201] = 0x06;
        mem[0x0605] = 0x44;
        mem[0x0606] = 0x43;
        cpu.reset();
        cpu.X = 0x07;
        cpu.Y = 0x11;
    }
    virtual void TearDown(){
    }
};

TEST_F(ProcessorTest, impliedAddressing_DoesntChangePC_noAddCycles) {
    //When
    int additionalCycles = cpu.IMP(cpu.PC, addr_abs, addr_rel);
    //Then
    EXPECT_EQ(cpu.PC, 0x0200);
    EXPECT_EQ(additionalCycles, 0);
    EXPECT_EQ(addr_abs, 0x0042);
}

TEST_F(ProcessorTest, immediateAddressing_IncrementPC_AbsAddrEqualsPC_noAddCycles) {
    //When
    int additionalCycles = cpu.IMM(cpu.PC, addr_abs, addr_rel);
    //Then
    EXPECT_EQ(cpu.PC, 0x0201);
    EXPECT_EQ(addr_abs, 0x0200);
    EXPECT_EQ(additionalCycles, 0);
}

TEST_F(ProcessorTest, absoluteAddressing_IncrementPCTwice_AbsAddrEqualsDereferencedPC_noAddCycles) {
    //When
    int additionalCycles = cpu.ABS(cpu.PC, addr_abs, addr_rel);
    //Then
    EXPECT_EQ(cpu.PC, 0x0202);
    EXPECT_EQ(addr_abs, 0x0605);
    EXPECT_EQ(additionalCycles, 0);
}

TEST_F(ProcessorTest, zeroPageAddressing_IncrementsPC_AbsAddrEqualsFirstByteOfDereferencedPC_noAddCycles) {
    //When
    int additionalCycles = cpu.ZPA(cpu.PC, addr_abs, addr_rel);
    //Then
    EXPECT_EQ(cpu.PC, 0x0201);
    EXPECT_EQ(addr_abs, 0x0005);
    EXPECT_EQ(additionalCycles, 0);
}

TEST_F(ProcessorTest, indexedZeroPageAddressingX_IncrementsPC_AbsAddrEqualsPCPlusX_noAddCycles_resultWrapsAroundByte) {
    //When
    int additionalCycles = cpu.ZPX(cpu.PC, addr_abs, addr_rel);
    //Then
    EXPECT_EQ(cpu.PC, 0x0201);
    EXPECT_EQ(addr_abs, 0x0005 + 0x0007);
    EXPECT_EQ(additionalCycles, 0);
    //Given
    mem[0x0200] = 0x80;
    cpu.reset();
    cpu.X = 0xFF;
    //When
    cpu.ZPX(cpu.PC, addr_abs, addr_rel);
    //Then
    EXPECT_EQ(addr_abs, 0x007F);
}

TEST_F(ProcessorTest, indexedZeroPageAddressingY_IncrementsPC_AbsAddrEqualsPCPlusY_noAddCycles_resultWrapsAroundByte) {
    //When
    int additionalCycles = cpu.ZPY(cpu.PC, addr_abs, addr_rel);
    //Then
    EXPECT_EQ(cpu.PC, 0x0201);
    EXPECT_EQ(addr_abs, 0x0005 + 0x0011);
    EXPECT_EQ(additionalCycles, 0);
    //Given
    mem[0x0200] = 0x80;
    cpu.reset();
    cpu.Y = 0xFF;
    //WHen
    cpu.ZPY(cpu.PC, addr_abs, addr_rel);
    //Then
    EXPECT_EQ(addr_abs, 0x007F);
}

TEST_F(ProcessorTest, indexedAbsoluteAddressingX_IncrementsPC_AbsAddrEqualsDereferencedPCPlusX_AddCycleOnPageBoundary) {
    //When
    int additionalCycles = cpu.ABX(cpu.PC, addr_abs, addr_rel);
    //Then
    EXPECT_EQ(cpu.PC, 0x0202);
    EXPECT_EQ(addr_abs, 0x060C);
    EXPECT_EQ(additionalCycles, 0);
    //Given
    cpu.reset();
    cpu.X = 0xFE;
    //When
    additionalCycles = cpu.ABX(cpu.PC, addr_abs, addr_rel);
    //Then
    EXPECT_EQ(cpu.PC, 0x0202);
    EXPECT_EQ(addr_abs, 0x0703);
    EXPECT_EQ(additionalCycles, 1);
}

TEST_F(ProcessorTest, indexedAbsoluteAddressingY_IncrementsPC_AbsAddrEqualsDereferencedPCPlusY_AddCycleOnPageBoundary) {
    //When
    int additionalCycles = cpu.ABY(cpu.PC, addr_abs, addr_rel);
    //Then
    EXPECT_EQ(cpu.PC, 0x0202);
    EXPECT_EQ(addr_abs, 0x0616);
    EXPECT_EQ(additionalCycles, 0);
    //Given
    cpu.reset();
    cpu.Y = 0xFE;
    //When
    additionalCycles = cpu.ABY(cpu.PC, addr_abs, addr_rel);
    //Then
    EXPECT_EQ(cpu.PC, 0x0202);
    EXPECT_EQ(addr_abs, 0x0703);
    EXPECT_EQ(additionalCycles, 1);
}

TEST_F(ProcessorTest, relAddressing_IncrementsPC_RelAddrEqualsDereferencedPC_NoAddCycles) {
    //When
    int additionalCycles = cpu.REL(cpu.PC, addr_abs, addr_rel);
    //Then
    EXPECT_EQ(cpu.PC, 0x0201);
    EXPECT_EQ(addr_rel, 0x05);
    EXPECT_EQ(additionalCycles, 0);
}

TEST_F(ProcessorTest, indexedIndirectAddressingX_IncrementsPC_AbsAddrEqualsDereferencedPCPlusXDereferenced_NoAddCycles) {
    //When
    int additionalCycles = cpu.IIX(cpu.PC, addr_abs, addr_rel);
    //Then
    EXPECT_EQ(cpu.PC, 0x0201);
    EXPECT_EQ(addr_abs, 0x7978);
    EXPECT_EQ(additionalCycles, 0);
}

TEST_F(ProcessorTest, indirectIndexedAddressingY_IncrementsPC_AbsAddrEqualsDereferencedDereferencedPCPlusY_AddCycleOnPageBoundary) {
    //When
    int additionalCycles = cpu.IIY(cpu.PC, addr_abs, addr_rel);
    //Then
    EXPECT_EQ(cpu.PC, 0x0201);
    EXPECT_EQ(addr_abs, 0x95AA);
    EXPECT_EQ(additionalCycles, 0);
    //Given
    cpu.reset();
    cpu.Y = 0xFE;
    //When
    additionalCycles = cpu.IIY(cpu.PC, addr_abs, addr_rel);
    //Then
    EXPECT_EQ(cpu.PC, 0x0201);
    EXPECT_EQ(addr_abs, 0x9697);
    EXPECT_EQ(additionalCycles, 1);
}

TEST_F(ProcessorTest, absoluteIndirectAddressing_IncrementsPCTwice_RelAddrEqualsDereferencedPC_NoAddCycles) {
    //When
    int additionalCycles = cpu.IND(cpu.PC, addr_abs, addr_rel);
    //Then
    EXPECT_EQ(cpu.PC, 0x0202);
    EXPECT_EQ(addr_abs, 0x4344);
    EXPECT_EQ(additionalCycles, 0);
}