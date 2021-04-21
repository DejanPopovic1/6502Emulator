#include "gtest/gtest.h"

#define IS_TESTING 1

#include "../Processor.h"
#include "../Memory.h"
#include "../testingFile.h"

#define RESET_LO_ADDR 0xFFFC
#define RESET_HI_ADDR 0xFFFD

class ProcessorTest : public testing::Test
{
public:
    Memory mem;
    Processor cpu;
    uint16_t addr_abs = 0x0042;
    uint8_t addr_rel = 0x42;
    virtual void SetUp(){
        cpu.connectMemory(&mem);
        mem[RESET_LO_ADDR] = 0x00;
        mem[RESET_HI_ADDR] = 0x02;
        mem[0x0200] = 0x05;
        mem[0x0201] = 0x06;
        cpu.reset();
        cpu.X = 0x07;
        cpu.Y = 0x11;
        //At this point, PC is set to 0x0200
    }
    virtual void TearDown(){
    }
};


//The PC is not always itself the Address. Addressing modes tell us how to calculate the operand address from a
//given set of parameters, one of the most important being the PC
TEST_F(ProcessorTest, impliedAddressing_DoesntChangePC_noAddCycles) {
    int additionalCycles = cpu.IMP(cpu.PC, addr_abs, addr_rel);
    EXPECT_EQ(cpu.PC, 0x0200);
    EXPECT_EQ(additionalCycles, 0);
    EXPECT_EQ(addr_abs, 0x0042);
}

TEST_F(ProcessorTest, immediateAddressing_IncrementPC_AbsAddrEqualsPC_noAddCycles) {
    int additionalCycles = cpu.IMM(cpu.PC, addr_abs, addr_rel);
    EXPECT_EQ(cpu.PC, 0x0201);
    EXPECT_EQ(addr_abs, 0x0200);
    EXPECT_EQ(additionalCycles, 0);
}

TEST_F(ProcessorTest, absoluteAddressing_IncrementPCTwice_AbsAddrEqualsDereferencedPC_noAddCycles) {
    int additionalCycles = cpu.ABS(cpu.PC, addr_abs, addr_rel);
    EXPECT_EQ(cpu.PC, 0x0202);
    EXPECT_EQ(addr_abs, 0x0605);
    EXPECT_EQ(additionalCycles, 0);
}

TEST_F(ProcessorTest, zeroPageAddressing_IncrementsPC_AbsAddrEqualsFirstByteOfDereferencedPC_noAddCycles) {
    int additionalCycles = cpu.ZPA(cpu.PC, addr_abs, addr_rel);
    EXPECT_EQ(cpu.PC, 0x0201);
    EXPECT_EQ(addr_abs, 0x0005);
    EXPECT_EQ(additionalCycles, 0);
}

TEST_F(ProcessorTest, indexedZeroPageAddressingX_IncrementsPC_AbsAddrEqualsPCPlusX_noAddCycles_resultWrapsAroundByte) {
    int additionalCycles = cpu.ZPX(cpu.PC, addr_abs, addr_rel);
    EXPECT_EQ(cpu.PC, 0x0201);
    EXPECT_EQ(addr_abs, 0x0005 + 0x0007);
    EXPECT_EQ(additionalCycles, 0);
    mem[0x0200] = 0x80;
    cpu.reset();
    cpu.X = 0xFF;
    cpu.ZPX(cpu.PC, addr_abs, addr_rel);
    EXPECT_EQ(addr_abs, 0x007F);
}

TEST_F(ProcessorTest, indexedZeroPageAddressingY_IncrementsPC_AbsAddrEqualsPCPlusY_noAddCycles_resultWrapsAroundByte) {
    int additionalCycles = cpu.ZPY(cpu.PC, addr_abs, addr_rel);
    EXPECT_EQ(cpu.PC, 0x0201);
    EXPECT_EQ(addr_abs, 0x0005 + 0x0011);
    EXPECT_EQ(additionalCycles, 0);
    mem[0x0200] = 0x80;
    cpu.reset();
    cpu.Y = 0xFF;
    cpu.ZPY(cpu.PC, addr_abs, addr_rel);
    EXPECT_EQ(addr_abs, 0x007F);
}

TEST_F(ProcessorTest, indexedAbsoluteAddressingX_IncrementsPC_AbsAddrEqualsDereferencedPCPlusX_AddCycleOnPageBoundary) {
    int additionalCycles = cpu.ABX(cpu.PC, addr_abs, addr_rel);
    EXPECT_EQ(cpu.PC, 0x0202);
    EXPECT_EQ(addr_abs, 0x060C);
    EXPECT_EQ(additionalCycles, 0);
    cpu.reset();
    cpu.X = 0xFE;
    additionalCycles = cpu.ABX(cpu.PC, addr_abs, addr_rel);
    EXPECT_EQ(cpu.PC, 0x0202);
    EXPECT_EQ(addr_abs, 0x0703);
    EXPECT_EQ(additionalCycles, 1);
}

TEST_F(ProcessorTest, indexedAbsoluteAddressingY_IncrementsPC_AbsAddrEqualsDereferencedPCPlusY_AddCycleOnPageBoundary) {
    int additionalCycles = cpu.ABY(cpu.PC, addr_abs, addr_rel);
    EXPECT_EQ(cpu.PC, 0x0202);
    EXPECT_EQ(addr_abs, 0x0616);
    EXPECT_EQ(additionalCycles, 0);
    cpu.reset();
    cpu.Y = 0xFE;
    additionalCycles = cpu.ABY(cpu.PC, addr_abs, addr_rel);
    EXPECT_EQ(cpu.PC, 0x0202);
    EXPECT_EQ(addr_abs, 0x0703);
    EXPECT_EQ(additionalCycles, 1);
}

TEST_F(ProcessorTest, relAddressing_IncrementsPC_RelAddrEqualsDereferencedPC_NoAddCycles) {
    int additionalCycles = cpu.REL(cpu.PC, addr_abs, addr_rel);
    EXPECT_EQ(cpu.PC, 0x0201);
    EXPECT_EQ(addr_rel, 0x05);
    EXPECT_EQ(additionalCycles, 0);
}

TEST_F(ProcessorTest, indexedIndirectAddressingX_IncrementsPC_RelAddrEqualsDereferencedPC_NoAddCycles) {
    int additionalCycles = cpu.IIX(cpu.PC, addr_abs, addr_rel);
    EXPECT_EQ(cpu.PC, 0x0201);
    EXPECT_EQ(addr_rel, 0x05);
    EXPECT_EQ(additionalCycles, 0);
}

TEST_F(ProcessorTest, indexedIndirectAddressingY_IncrementsPC_RelAddrEqualsDereferencedPC_NoAddCycles) {
    int additionalCycles = cpu.IIY(cpu.PC, addr_abs, addr_rel);
    EXPECT_EQ(cpu.PC, 0x0201);
    EXPECT_EQ(addr_rel, 0x05);
    EXPECT_EQ(additionalCycles, 0);
    cpu.reset();
    cpu.Y = 0xFE;
    additionalCycles = cpu.ABY(cpu.PC, addr_abs, addr_rel);
    EXPECT_EQ(cpu.PC, 0x0202);
    EXPECT_EQ(addr_abs, 0x0703);
    EXPECT_EQ(additionalCycles, 1);
}

TEST_F(ProcessorTest, absoluteIndirectAddressing_IncrementsPC_RelAddrEqualsDereferencedPC_NoAddCycles) {
    int additionalCycles = cpu.IND(cpu.PC, addr_abs, addr_rel);
    EXPECT_EQ(cpu.PC, 0x0201);
    EXPECT_EQ(addr_rel, 0x05);
    EXPECT_EQ(additionalCycles, 0);
}



















//TEST_F(ProcessorTest, impliedAddressingDoesntSetAddress) {
//    cpu.IMP(cpu.PC, addr_abs, addr_rel);
//    EXPECT_EQ(1, 1);
////    constexpr s32 NUM_CYCLES = 0;
////    s32 CyclesUsed = cpu.Execute(NUM_CYCLES, mem);
////    EXPECT_EQ(CyclesUsed, 0);
//}

//TEST_F(ProcessorTest, impliedAddressingDoesntSetAddress) {
//    cpu.IMP(cpu.PC, addr_abs, addr_rel);
//    EXPECT_EQ(1, 1);
////    constexpr s32 NUM_CYCLES = 0;
////    s32 CyclesUsed = cpu.Execute(NUM_CYCLES, mem);
////    EXPECT_EQ(CyclesUsed, 0);
//}

//static void VerifyUnmodifiedFlagsFromDA(CPU cpu, CPU CPUCopy){
//    EXPECT_EQ(cpu.C, CPUCopy.C);
//    EXPECT_EQ(cpu.I, CPUCopy.I);
//    EXPECT_EQ(cpu.D, CPUCopy.D);
//    EXPECT_EQ(cpu.B, CPUCopy.B);
//    EXPECT_EQ(cpu.V, CPUCopy.V);
//}


//
//static void VerifyUnmodifiedFlagsFromDA(CPU cpu, CPU CPUCopy){
//    EXPECT_EQ(cpu.C, CPUCopy.C);
//    EXPECT_EQ(cpu.I, CPUCopy.I);
//    EXPECT_EQ(cpu.D, CPUCopy.D);
//    EXPECT_EQ(cpu.B, CPUCopy.B);
//    EXPECT_EQ(cpu.V, CPUCopy.V);
//}
//
//TEST_F(M6502Test1, TheCPUDOesNothingWhenWeExecuteZeroCycles) {
//    constexpr s32 NUM_CYCLES = 0;
//    s32 CyclesUsed = cpu.Execute(NUM_CYCLES, mem);
//    EXPECT_EQ(CyclesUsed, 0);
//}
//
//TEST_F(M6502Test1, CPUCanExecuteMoreCyclesThanRequestedIfRequiredByTheInstruction) {
//    mem[0xFFFC] = CPU::INS_LDA_IM;
//    mem[0xFFFD] = 0x84;
//    CPU CPUCopy = cpu;
//    constexpr s32 NUM_CYCLES = 0;
//    s32 CyclesUsed = cpu.Execute(1, mem);
//    EXPECT_EQ(CyclesUsed, 2);
//}
//
//TEST_F(M6502Test1, ExecutingABadInstructionDoesNotPutUsInAnInfiniteLoop) {
//    mem[0xFFFC] = 0x0;//Invalid instruction/opcode
//    mem[0xFFFD] = 0x0;
//    CPU CPUCopy = cpu;
//    constexpr s32 NUM_CYCLES = 2;
//    s32 CyclesUsed = cpu.Execute(NUM_CYCLES, mem);
//    EXPECT_EQ(CyclesUsed, NUM_CYCLES);
//}
//
//
//TEST_F(M6502Test1, LDAImmediateCanLoadAValueIntoTheARegister) {
//    mem[0xFFFC] = CPU::INS_LDA_IM;
//    mem[0xFFFD] = 0x84;
//    CPU CPUCopy = cpu;
//    s32 CyclesUsed = cpu.Execute(2, mem);
//    EXPECT_EQ(cpu.A, 0x84);
//    EXPECT_EQ(CyclesUsed, 2);
//    EXPECT_FALSE(cpu.Z);
//    EXPECT_TRUE(cpu.N);
//    VerifyUnmodifiedFlagsFromDA(cpu, CPUCopy);
//}
//
//TEST_F(M6502Test1, LDAImmediateCanAffectTheZeroFlag) {
//    cpu.A = 0x44;
//    mem[0xFFFC] = CPU::INS_LDA_IM;
//    mem[0xFFFD] = 0x0;
//    CPU CPUCopy = cpu;
//    s32 CyclesUsed = cpu.Execute(2, mem);
//    EXPECT_EQ(cpu.A, 0x0);
//    EXPECT_EQ(CyclesUsed, 2);
//    EXPECT_TRUE(cpu.Z);
//    EXPECT_FALSE(cpu.N);
//    VerifyUnmodifiedFlagsFromDA(cpu, CPUCopy);
//}
//
//TEST_F(M6502Test1, LDAAbsoluteCanLoadAValueIntoTheARegister) {
//    mem[0xFFFC] = CPU::INS_LDA_ABS;
//    mem[0xFFFD] = 0x80;
//    mem[0xFFFE] = 0x44; //0x4480
//    mem[0x4480] = 0x37;
//    constexpr s32 EXPECTED_CYCLES = 4;
//    CPU CPUCopy = cpu;
//    s32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, mem);
//    EXPECT_EQ(cpu.A, 0x37);
//    EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
//    EXPECT_FALSE(cpu.Z);
//    EXPECT_TRUE(cpu.N);
//    VerifyUnmodifiedFlagsFromDA(cpu, CPUCopy);
//}
//
//TEST_F(M6502Test1, LDAAbsoluteXCanLoadAValueIntoTheARegister) {
//    cpu.X = 1;
//    mem[0xFFFC] = CPU::INS_LDA_ABSX;
//    mem[0xFFFD] = 0x80;
//    mem[0xFFFE] = 0x44; //0x4480
//    mem[0x4481] = 0x37;
//    constexpr s32 EXPECTED_CYCLES = 4;
//    CPU CPUCopy = cpu;
//    s32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, mem);
//    EXPECT_EQ(cpu.A, 0x37);
//    EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
//    EXPECT_FALSE(cpu.Z);
//    EXPECT_TRUE(cpu.N);
//    VerifyUnmodifiedFlagsFromDA(cpu, CPUCopy);
//}
//
//
//TEST_F(M6502Test1, LDAZeroPageCanLoadAValueIntoTheARegister) {
//    mem[0xFFFC] = CPU::INS_LDA_ZP;
//    mem[0xFFFD] = 0x42;
//    mem[0x0042] = 0x37;
//    CPU CPUCopy = cpu;
//    s32 CyclesUsed = cpu.Execute(3, mem);
//    EXPECT_EQ(cpu.A, 0x37);
//    EXPECT_EQ(CyclesUsed, 3);
//    EXPECT_FALSE(cpu.Z);
//    EXPECT_FALSE(cpu.N);
//    VerifyUnmodifiedFlagsFromDA(cpu, CPUCopy);
//}
//
//TEST_F(M6502Test1, LDAZeroPageXCanLoadAValueIntoTheARegister) {
//    cpu.X = 5;
//    mem[0xFFFC] = CPU::INS_LDA_ZPX;
//    mem[0xFFFD] = 0x42;
//    mem[0x0047] = 0x37;
//    CPU CPUCopy = cpu;
//    s32 CyclesUsed = cpu.Execute(4, mem);
//    EXPECT_EQ(cpu.A, 0x37);
//    EXPECT_EQ(CyclesUsed, 4);
//    EXPECT_FALSE(cpu.Z);
//    EXPECT_FALSE(cpu.N);
//    VerifyUnmodifiedFlagsFromDA(cpu, CPUCopy);
//}
//
//TEST_F(M6502Test1, LDAZeroPageXCanLoadAValueIntoTheARegisterWhenItWraps) {
//    cpu.X = 0xFF;
//    mem[0xFFFC] = CPU::INS_LDA_ZPX;
//    mem[0xFFFD] = 0x80;
//    mem[0x007F] = 0x37;
//    CPU CPUCopy = cpu;
//    s32 CyclesUsed = cpu.Execute(4, mem);
//    EXPECT_EQ(cpu.A, 0x37);
//    EXPECT_EQ(CyclesUsed, 4);
//    EXPECT_FALSE(cpu.Z);
//    EXPECT_FALSE(cpu.N);
//    VerifyUnmodifiedFlagsFromDA(cpu, CPUCopy);
//}
//
////// Tests factorial of 0.
////TEST(FactorialTest, HandlesZeroInput) {
////EXPECT_EQ(1, 1);
////}
//
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
