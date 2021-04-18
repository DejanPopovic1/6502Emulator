#include "Processor.h"
#include "Memory.h"

#define STACK_BASE_ADDR 0x0100

//Interrupt Vectors - BRK is a software interrupt and the rest are hardware interrupts
//If the internal inhibit/IRQ Disable Flag (2nd bit in status register) is set, then an interrupt request is ignored
#define IRQBRK_LO_ADDR 0xFFFE
#define IRQBRK_HI_ADDR 0xFFFF
#define NMI_LO_ADDR 0xFFFA
#define NMI_HI_ADDR 0xFFFB
#define RESET_LO_ADDR 0xFFFC
#define RESET_HI_ADDR 0xFFFD

//Use setIf flag and setOrClearFlag
//Be careful when handling flags. Must change some of them. Sometimes they must be explicitly set and otherwise nothing is done. Other times, the value of the flag must be equal to
//the value of the bit
//When adding the dis assembler, it should be in a separate class called disassembler. In any case this should be in backlog
Processor::Processor(){
    using a = Processor;
    //this->addr_abs = 0;
    //this->addr_rel = 0;
    this->opcode = 0;
    this->cycles = 0;
    this->status = 0;
    this->lookup =
            {
                    {&a::BRK, &a::IMM, 7 }, {&a::ORA, &a::IIX, 6 }, {&a::XXX, &a::IMP, 2 }, {&a::XXX, &a::IMP, 8 }, {&a::NOP, &a::IMP, 3 }, {&a::ORA, &a::ZPA, 3 }, {&a::ASL, &a::ZPA, 5 }, {&a::XXX, &a::IMP, 5 }, {&a::PHP, &a::IMP, 3 }, {&a::ORA, &a::IMM, 2 }, {&a::ASL, &a::IMP, 2 }, {&a::XXX, &a::IMP, 2 }, {&a::NOP, &a::IMP, 4 }, {&a::ORA, &a::ABS, 4 }, {&a::ASL, &a::ABS, 6 }, {&a::XXX, &a::IMP, 6 },
                    {&a::BPL, &a::REL, 2 }, {&a::ORA, &a::IIY, 5 }, {&a::XXX, &a::IMP, 2 }, {&a::XXX, &a::IMP, 8 }, {&a::NOP, &a::IMP, 4 }, {&a::ORA, &a::ZPX, 4 }, {&a::ASL, &a::ZPX, 6 }, {&a::XXX, &a::IMP, 6 }, {&a::CLC, &a::IMP, 2 }, {&a::ORA, &a::ABY, 4 }, {&a::NOP, &a::IMP, 2 }, {&a::XXX, &a::IMP, 7 }, {&a::NOP, &a::IMP, 4 }, {&a::ORA, &a::ABX, 4 }, {&a::ASL, &a::ABX, 7 }, {&a::XXX, &a::IMP, 7 },
                    {&a::JSR, &a::ABS, 6 }, {&a::AND, &a::IIX, 6 }, {&a::XXX, &a::IMP, 2 }, {&a::XXX, &a::IMP, 8 }, {&a::BIT, &a::ZPA, 3 }, {&a::AND, &a::ZPA, 3 }, {&a::ROL, &a::ZPA, 5 }, {&a::XXX, &a::IMP, 5 }, {&a::PLP, &a::IMP, 4 }, {&a::AND, &a::IMM, 2 }, {&a::ROL, &a::IMP, 2 }, {&a::XXX, &a::IMP, 2 }, {&a::BIT, &a::ABS, 4 }, {&a::AND, &a::ABS, 4 }, {&a::ROL, &a::ABS, 6 }, {&a::XXX, &a::IMP, 6 },
                    {&a::BMI, &a::REL, 2 }, {&a::AND, &a::IIY, 5 }, {&a::XXX, &a::IMP, 2 }, {&a::XXX, &a::IMP, 8 }, {&a::NOP, &a::IMP, 4 }, {&a::AND, &a::ZPX, 4 }, {&a::ROL, &a::ZPX, 6 }, {&a::XXX, &a::IMP, 6 }, {&a::SEC, &a::IMP, 2 }, {&a::AND, &a::ABY, 4 }, {&a::NOP, &a::IMP, 2 }, {&a::XXX, &a::IMP, 7 }, {&a::NOP, &a::IMP, 4 }, {&a::AND, &a::ABX, 4 }, {&a::ROL, &a::ABX, 7 }, {&a::XXX, &a::IMP, 7 },
                    {&a::RTI, &a::IMP, 6 }, {&a::EOR, &a::IIX, 6 }, {&a::XXX, &a::IMP, 2 }, {&a::XXX, &a::IMP, 8 }, {&a::NOP, &a::IMP, 3 }, {&a::EOR, &a::ZPA, 3 }, {&a::LSR, &a::ZPA, 5 }, {&a::XXX, &a::IMP, 5 }, {&a::PHA, &a::IMP, 3 }, {&a::EOR, &a::IMM, 2 }, {&a::LSR, &a::IMP, 2 }, {&a::XXX, &a::IMP, 2 }, {&a::JMP, &a::ABS, 3 }, {&a::EOR, &a::ABS, 4 }, {&a::LSR, &a::ABS, 6 }, {&a::XXX, &a::IMP, 6 },
                    {&a::BVC, &a::REL, 2 }, {&a::EOR, &a::IIY, 5 }, {&a::XXX, &a::IMP, 2 }, {&a::XXX, &a::IMP, 8 }, {&a::NOP, &a::IMP, 4 }, {&a::EOR, &a::ZPX, 4 }, {&a::LSR, &a::ZPX, 6 }, {&a::XXX, &a::IMP, 6 }, {&a::CLI, &a::IMP, 2 }, {&a::EOR, &a::ABY, 4 }, {&a::NOP, &a::IMP, 2 }, {&a::XXX, &a::IMP, 7 }, {&a::NOP, &a::IMP, 4 }, {&a::EOR, &a::ABX, 4 }, {&a::LSR, &a::ABX, 7 }, {&a::XXX, &a::IMP, 7 },
                    {&a::RTS, &a::IMP, 6 }, {&a::ADC, &a::IIX, 6 }, {&a::XXX, &a::IMP, 2 }, {&a::XXX, &a::IMP, 8 }, {&a::NOP, &a::IMP, 3 }, {&a::ADC, &a::ZPA, 3 }, {&a::ROR, &a::ZPA, 5 }, {&a::XXX, &a::IMP, 5 }, {&a::PLA, &a::IMP, 4 }, {&a::ADC, &a::IMM, 2 }, {&a::ROR, &a::IMP, 2 }, {&a::XXX, &a::IMP, 2 }, {&a::JMP, &a::IND, 5 }, {&a::ADC, &a::ABS, 4 }, {&a::ROR, &a::ABS, 6 }, {&a::XXX, &a::IMP, 6 },
                    {&a::BVS, &a::REL, 2 }, {&a::ADC, &a::IIY, 5 }, {&a::XXX, &a::IMP, 2 }, {&a::XXX, &a::IMP, 8 }, {&a::NOP, &a::IMP, 4 }, {&a::ADC, &a::ZPX, 4 }, {&a::ROR, &a::ZPX, 6 }, {&a::XXX, &a::IMP, 6 }, {&a::SEI, &a::IMP, 2 }, {&a::ADC, &a::ABY, 4 }, {&a::NOP, &a::IMP, 2 }, {&a::XXX, &a::IMP, 7 }, {&a::NOP, &a::IMP, 4 }, {&a::ADC, &a::ABX, 4 }, {&a::ROR, &a::ABX, 7 }, {&a::XXX, &a::IMP, 7 },
                    {&a::NOP, &a::IMP, 2 }, {&a::STA, &a::IIX, 6 }, {&a::NOP, &a::IMP, 2 }, {&a::XXX, &a::IMP, 6 }, {&a::STY, &a::ZPA, 3 }, {&a::STA, &a::ZPA, 3 }, {&a::STX, &a::ZPA, 3 }, {&a::XXX, &a::IMP, 3 }, {&a::DEY, &a::IMP, 2 }, {&a::NOP, &a::IMP, 2 }, {&a::TXA, &a::IMP, 2 }, {&a::XXX, &a::IMP, 2 }, {&a::STY, &a::ABS, 4 }, {&a::STA, &a::ABS, 4 }, {&a::STX, &a::ABS, 4 }, {&a::XXX, &a::IMP, 4 },
                    {&a::BCC, &a::REL, 2 }, {&a::STA, &a::IIY, 6 }, {&a::XXX, &a::IMP, 2 }, {&a::XXX, &a::IMP, 6 }, {&a::STY, &a::ZPX, 4 }, {&a::STA, &a::ZPX, 4 }, {&a::STX, &a::ZPY, 4 }, {&a::XXX, &a::IMP, 4 }, {&a::TYA, &a::IMP, 2 }, {&a::STA, &a::ABY, 5 }, {&a::TXS, &a::IMP, 2 }, {&a::XXX, &a::IMP, 5 }, {&a::NOP, &a::IMP, 5 }, {&a::STA, &a::ABX, 5 }, {&a::XXX, &a::IMP, 5 }, {&a::XXX, &a::IMP, 5 },
                    {&a::LDY, &a::IMM, 2 }, {&a::LDA, &a::IIX, 6 }, {&a::LDX, &a::IMM, 2 }, {&a::XXX, &a::IMP, 6 }, {&a::LDY, &a::ZPA, 3 }, {&a::LDA, &a::ZPA, 3 }, {&a::LDX, &a::ZPA, 3 }, {&a::XXX, &a::IMP, 3 }, {&a::TAY, &a::IMP, 2 }, {&a::LDA, &a::IMM, 2 }, {&a::TAX, &a::IMP, 2 }, {&a::XXX, &a::IMP, 2 }, {&a::LDY, &a::ABS, 4 }, {&a::LDA, &a::ABS, 4 }, {&a::LDX, &a::ABS, 4 }, {&a::XXX, &a::IMP, 4 },
                    {&a::BCS, &a::REL, 2 }, {&a::LDA, &a::IIY, 5 }, {&a::XXX, &a::IMP, 2 }, {&a::XXX, &a::IMP, 5 }, {&a::LDY, &a::ZPX, 4 }, {&a::LDA, &a::ZPX, 4 }, {&a::LDX, &a::ZPY, 4 }, {&a::XXX, &a::IMP, 4 }, {&a::CLV, &a::IMP, 2 }, {&a::LDA, &a::ABY, 4 }, {&a::TSX, &a::IMP, 2 }, {&a::XXX, &a::IMP, 4 }, {&a::LDY, &a::ABX, 4 }, {&a::LDA, &a::ABX, 4 }, {&a::LDX, &a::ABY, 4 }, {&a::XXX, &a::IMP, 4 },
                    {&a::CPY, &a::IMM, 2 }, {&a::CMP, &a::IIX, 6 }, {&a::NOP, &a::IMP, 2 }, {&a::XXX, &a::IMP, 8 }, {&a::CPY, &a::ZPA, 3 }, {&a::CMP, &a::ZPA, 3 }, {&a::DEC, &a::ZPA, 5 }, {&a::XXX, &a::IMP, 5 }, {&a::INY, &a::IMP, 2 }, {&a::CMP, &a::IMM, 2 }, {&a::DEX, &a::IMP, 2 }, {&a::XXX, &a::IMP, 2 }, {&a::CPY, &a::ABS, 4 }, {&a::CMP, &a::ABS, 4 }, {&a::DEC, &a::ABS, 6 }, {&a::XXX, &a::IMP, 6 },
                    {&a::BNE, &a::REL, 2 }, {&a::CMP, &a::IIY, 5 }, {&a::XXX, &a::IMP, 2 }, {&a::XXX, &a::IMP, 8 }, {&a::NOP, &a::IMP, 4 }, {&a::CMP, &a::ZPX, 4 }, {&a::DEC, &a::ZPX, 6 }, {&a::XXX, &a::IMP, 6 }, {&a::CLD, &a::IMP, 2 }, {&a::CMP, &a::ABY, 4 }, {&a::NOP, &a::IMP, 2 }, {&a::XXX, &a::IMP, 7 }, {&a::NOP, &a::IMP, 4 }, {&a::CMP, &a::ABX, 4 }, {&a::DEC, &a::ABX, 7 }, {&a::XXX, &a::IMP, 7 },
                    {&a::CPX, &a::IMM, 2 }, {&a::SBC, &a::IIX, 6 }, {&a::NOP, &a::IMP, 2 }, {&a::XXX, &a::IMP, 8 }, {&a::CPX, &a::ZPA, 3 }, {&a::SBC, &a::ZPA, 3 }, {&a::INC, &a::ZPA, 5 }, {&a::XXX, &a::IMP, 5 }, {&a::INX, &a::IMP, 2 }, {&a::SBC, &a::IMM, 2 }, {&a::NOP, &a::IMP, 2 }, {&a::SBC, &a::IMP, 2 }, {&a::CPX, &a::ABS, 4 }, {&a::SBC, &a::ABS, 4 }, {&a::INC, &a::ABS, 6 }, {&a::XXX, &a::IMP, 6 },
                    {&a::BEQ, &a::REL, 2 }, {&a::SBC, &a::IIY, 5 }, {&a::XXX, &a::IMP, 2 }, {&a::XXX, &a::IMP, 8 }, {&a::NOP, &a::IMP, 4 }, {&a::SBC, &a::ZPX, 4 }, {&a::INC, &a::ZPX, 6 }, {&a::XXX, &a::IMP, 6 }, {&a::SED, &a::IMP, 2 }, {&a::SBC, &a::ABY, 4 }, {&a::NOP, &a::IMP, 2 }, {&a::XXX, &a::IMP, 7 }, {&a::NOP, &a::IMP, 4 }, {&a::SBC, &a::ABX, 4 }, {&a::INC, &a::ABX, 7 }, {&a::XXX, &a::IMP, 7 },
            };
}

Processor::~Processor(){

}

//Add clear and set enums
//I surmise that after every clock cycle, we conduct an IRQ() routine to check if the interrupt flag is enabled!
//The endianess of the stack pointer is the same as in general memory
//Special dedicated purpose address used. Encode into MACRO
void Processor::irq(){
    if(!getFlag(I)){
        write(0x0100 + SP, (PC >> 8) & 0x00FF);
        SP--;
        write(0x0100 + SP, PC & 0x00FF);
        SP--;
        setOrClearFlag(B, 0);
        setOrClearFlag(U, 1);
        setOrClearFlag(I, 1);
        write(0x0100 + SP, status);
        SP--;
        u16 low = read(IRQBRK_LO_ADDR);
        u16 high = read(IRQBRK_HI_ADDR);
        PC = (high << 8) | low;
        cycles = 7;
    }
}

//Special dedicated purpose address used. Encode into MACRO
void Processor::nmi(){
    if(!getFlag(I)) {
        write(STACK_BASE_ADDR + SP, (PC >> 8) & 0x00FF);
        SP--;
        write(STACK_BASE_ADDR + SP, PC & 0x00FF);
        SP--;
        setOrClearFlag(B, 0);
        setOrClearFlag(U, 1);
        setOrClearFlag(I, 1);
        write(STACK_BASE_ADDR + SP, status);
        SP--;
        u16 low = read(NMI_LO_ADDR);
        u16 high = read(NMI_HI_ADDR);
        PC = (high << 8) | low;
        cycles = 8;
    }
}

void Processor::connectMemory(Memory *m){
    this->mem = m;
}

u8 Processor::read(u16 a){
    return (*mem)[a];
}

void Processor::write(u16 a, u8 b){
    (*mem)[a] = b;
}

//Next two functions. Clock calls the instruction and the instruction calls fetch

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//Understand this in more detail
//Use AND() as an example to run through this
//This is the main "runner" of the processor and the function pointer runs the instruction
//To run the processor call while(true){ProcessorObject.clock()}
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void Processor::clock(){
    //If cycles == 0, then it means that the next instruction is due, together with its operands.
    //If its non-zero, it means an instruction, and the fetching of its operands, is currently being executed and we just decrement our way through it
    if(cycles == 0){
        opcode = read(this->PC);
        this->PC++;
        cycles = lookup[opcode].cycles;
        u16 addr_abs, addr_rel;
        u8 additional_cycle1 = (this->*lookup[opcode].addrmode)(this->PC, addr_abs, addr_rel);//We must fix this. This function addrmode changes the class variable state addr_abs and other variables including PC
        //At this point in time, we have fetched the absolute address depending on the address mode above. Only now can we operate on the addr_abs
        u8 additional_cycle2 = (this->*lookup[opcode].operate)(addr_abs, addr_rel);
        cycles += (additional_cycle1 & additional_cycle2);//We must fix this. Using bit operations on this seems like overkill. Add_cycle_1 and add_cycle_2 either returns a 0 or 1. Make this explicit here.
    }
    cycles--;
}

//Return the fetched value and use this rather than relying on internal values
//Fetch returns a value we hardly ever use. It rather changes internal class variables. Should it therefore not be redesigned? (it also adds to confusion and code tracing)
u8 Processor::fetch(uint16_t addr_abs){
    if(!(lookup[opcode].addrmode == &Processor::IMP)){
        return read(addr_abs);
    }
    return this->A;
}

//Refactor into read u8
void Processor::reset(){
    A = 0;
    X = 0;
    Y = 0;
    SP = 0xFD;
    status = 0x00;
    u16 low = read(RESET_LO_ADDR);
    u16 high = read(RESET_HI_ADDR);
    PC = (high << 8) | low;
    cycles = 8;

}

void Processor::setOrClearFlag(enum validFlagBits b, bool isSet){
    if(isSet){
        setFlag(b);
    }
    else if(!isSet) {
        clearFlag(b);
    }
}

void Processor::setFlag(enum validFlagBits b){
    status |= b;
}

void Processor::clearFlag(enum validFlagBits b){
    status &= ~b;
}

u8 Processor::getFlag(enum validFlagBits f){
    u8 test = (status & f);
    if(test > 0){
        return 1;
    }
    else{
        return 0;
    }
}

//ADDRESSING MODES
//The addressing modes all return the absolute address and secondary they tell us if there are any additional clock cycles required. They do nothing else

//Implied still "fetches" the operand, albeit not from memory. Its in the accumulator.
u8 Processor::IMP(u16 &PC, u16 &addr_abs, u16 &addr_rel){
    return 0;
}

u8 Processor::ZPA(u16 &PC, u16 &addr_abs, u16 &addr_rel){
    addr_abs = read(PC);
    PC++;
    addr_abs &= 0x00FF;
    return 0;
}

//Implement wrap around
u8 Processor::ZPY(u16 &PC, u16 &addr_abs, u16 &addr_rel){
    addr_abs = read(PC) + this->Y;
    PC++;
    addr_abs &= 0x00FF;
    return 0;
}

u8 Processor::ABS(u16 &PC, u16 &addr_abs, u16 &addr_rel){
    u16 lowu8 = read(PC);
    PC++;
    u16 highu8 = read(PC);
    PC++;
    addr_abs = (highu8 << 8) | lowu8;
    return 0;
}

u8 Processor::ABY(u16 &PC, u16 &addr_abs, u16 &addr_rel){
    u16 lowu8 = read(PC);
    PC++;
    u16 highu8 = read(PC);
    PC++;
    addr_abs = (highu8 << 8) | lowu8;
    addr_abs += this->Y;
    //Cross page boundary
    if((addr_abs & 0xFF00) != (highu8 << 8)){
        return 1;//"May" need an additional clock cycle
    }
    else {
        return 0;
    }
}

//"Both memory locations specifying the effective address must be in zero page"
u8 Processor::IIX(u16 &PC, u16 &addr_abs, u16 &addr_rel){
    u16 t = read(PC);
    PC++;
    u16 low = read((u16)(t + (u16)X) & 0x00FF);
    u16 high = read((u16)(t + (u16)X + 1) & 0x00FF);
    addr_abs = (high << 8) | low;
    return 0;
}

u8 Processor::IMM(u16 &PC, u16 &addr_abs, u16 &addr_rel){
    addr_abs = PC++;
    return 0;
}

//Must implement a wrap around here!
u8 Processor::ZPX(u16 &PC, u16 &addr_abs, u16 &addr_rel){
    addr_abs = read(PC) + this->X;
    PC++;
    addr_abs &= 0x00FF;
    return 0;
}

//Check *WHY* this negative addition is needed
//The If statement essentially does the following. If the read number is negative then append 1's infront of it:
//If xxxxxxxx is negative, i.e. if 1xxxxxxx, then:
//11111111*1*xxxxxxx
//Else, 00000000*0*xxxxxxx
u8 Processor::REL(u16 &PC, u16 &addr_abs, u16 &addr_rel){
    addr_rel = read(PC);
    PC++;
    if(addr_rel & 0x80){
        addr_rel |= 0xFF00;
    }
    return 0;
}

u8 Processor::ABX(u16 &PC, u16 &addr_abs, u16 &addr_rel){
    u16 lowu8 = read(PC);
    PC++;
    u16 highu8 = read(PC);
    PC++;
    addr_abs = (highu8 << 8) | lowu8;
    addr_abs += this->X;
    //Cross page boundary
    if((addr_abs & 0xFF00) != (highu8 << 8)){//Cross page boundary
        return 1;//"May" need an additional clock cycle
    }
    else {
        return 0;
    }
}

u8 Processor::IND(u16 &PC, u16 &addr_abs, u16 &addr_rel){
    u16 ptrLow = read(PC);
    PC++;
    u16 ptrHigh = read(PC);
    PC++;
    u16 ptr = (ptrHigh << 8) | ptrLow;
    if(ptrLow == 0x00FF){
        addr_abs = (read(ptr & 0xFF00) << 8) | read(ptr + 0);
    }
    else {
        addr_abs = (read(ptr + 1) << 8) | read(ptr + 0);
    }

    return 0;
}

u8 Processor::IIY(u16 &PC, u16 &addr_abs, u16 &addr_rel){
    u16 t = read(PC);
    PC++;
    u16 low = read(t & 0x00FF);
    u16 high = read((t + 1) & 0x00FF);
    addr_abs = (high << 8) | low;
    addr_abs += Y;
    if(addr_abs & 0xFF00 != (high << 8)){
        return 1;//"May" need an additional clock cycle
    }
    else{
        return 0;
    }
}


//====================
//INSTRUCTIONS
//====================

u8 Processor::ADC(u16 addr_abs, u16 addr_rel){
    u8 fetched = fetch(addr_abs);
    u16 temp = (u16)A + (u16)fetched + (u16)getFlag(C);
    setOrClearFlag(C, temp > 255);
    setOrClearFlag(Z, (temp & 0x00FF) == 0);
    setOrClearFlag(N, temp & 0x80);
    setOrClearFlag(V, (~((u16)A ^ (u16)fetched) & ((u16)A ^ (u16)temp)) & 0x0080);
    A = temp & 0x00FF;
    return 1;
}



u8 Processor::SBC(u16 addr_abs, u16 addr_rel){
    u8 fetched = fetch(addr_abs);
    u16 value = ((u16)fetched) ^ 0x00FF;
    u16 temp = (u16)A + value + (u16)getFlag(C);
    setOrClearFlag(C, temp & 0xFF00);
    setOrClearFlag(Z, ((temp & 0x00FF) == 0));
    setOrClearFlag(V, (temp ^ (u16)A) & (temp ^ value) & 0x0080);
    setOrClearFlag(N, temp & 0x0080);
    A = temp & 0x00FF;
    return 1;
}

u8 Processor::AND(u16 addr_abs, u16 addr_rel){
    u8 fetched = fetch(addr_abs);
    this->A = this->A & fetched;
    setOrClearFlag(Z, A == 0x00);
    setOrClearFlag(N, A & 0x80);
    return 1;
}


u8 Processor::ASL(u16 addr_abs, u16 addr_rel){
    u8 fetched = fetch(addr_abs);//If its implied addressing mode, it wont update any internal variables. One of the addressing modes is implied.
    setOrClearFlag(C, fetched & 0x80);
    setOrClearFlag(Z, !(fetched << 1));
    setOrClearFlag(N, (fetched << 1) & 0x80);
    if(lookup[opcode].addrmode == &Processor::IMP){
        A = (fetched << 1);
    }
    else {
        write(addr_abs, fetched << 1);
    }
    return 0;
}


u8 Processor::BCC(u16 addr_abs, u16 addr_rel){
    if (getFlag(C) == 0)
    {
        cycles++;
        addr_abs = PC + addr_rel;
        if((addr_abs & 0xFF00) != (PC & 0xFF00)) {
            cycles++;
        }
        PC = addr_abs;
    }
    return 0;
}

u8 Processor::BCS(u16 addr_abs, u16 addr_rel){
    if(getFlag(C) == 1){
        cycles++;
        addr_abs = PC + addr_rel;
        if((addr_abs & 0xFF00) != (PC & 0xFF00)){
            cycles++;
        }
        PC = addr_abs;
    }
    return 0;
}

u8 Processor::BEQ(u16 addr_abs, u16 addr_rel){
    if (getFlag(Z) == 1)
    {
        cycles++;
        addr_abs = PC + addr_rel;

        if ((addr_abs & 0xFF00) != (PC & 0xFF00))
            cycles++;

        PC = addr_abs;
    }
    return 0;
}

u8 Processor::BIT(u16 addr_abs, u16 addr_rel){
    u8 fetched = fetch(addr_abs);
    setOrClearFlag(Z, !fetched & A);
    setOrClearFlag(V, fetched & BIT_SIX);//Sixth bit. Add macro's. Rather use 1 << 6 as the macro
    setOrClearFlag(N, fetched & BIT_SEVEN);//Seventh bit. Add macro's. Rather use 1 << 7 as the macro
    return 0;
}


u8 Processor::BMI(u16 addr_abs, u16 addr_rel){
    if (getFlag(N) == 1)
    {
        cycles++;
        addr_abs = PC + addr_rel;

        if ((addr_abs & 0xFF00) != (PC & 0xFF00))
            cycles++;

        PC = addr_abs;
    }
    return 0;
}

u8 Processor::BNE(u16 addr_abs, u16 addr_rel){
    if (getFlag(Z) == 0)
    {
        cycles++;
        addr_abs = PC + addr_rel;

        if ((addr_abs & 0xFF00) != (PC & 0xFF00))
            cycles++;

        PC = addr_abs;
    }
    return 0;
}

u8 Processor::BPL(u16 addr_abs, u16 addr_rel){
    if (getFlag(N) == 0)
    {
        cycles++;
        addr_abs = PC + addr_rel;

        if ((addr_abs & 0xFF00) != (PC & 0xFF00))
            cycles++;

        PC = addr_abs;
    }
    return 0;
}

//""
//when BRK is encountered, internally the CPU pushes the contents of P onto the stack *with bit 4 set*.
//""

//Only three hardware interrupts exist:
//RESET
//NMI
//IRQ
//Note that BRK is a software interrupt
//Interrupt and its vector in hexadecimal
//INTERRUPT     LSB         MSB
//IRQ/BRK       FFFE        FFFF
//NMI           FFFA        FFFB
//RESET         FFFC        FFFD
//See https://en.wikipedia.org/wiki/Interrupts_in_65xx_processors

u8 Processor::BRK(u16 addr_abs, u16 addr_rel){
    u8 fetched = fetch(addr_abs);
    PC++;

    write(STACK_BASE_ADDR + SP, PC >> 8);//The second argument gets converted to a u8 & therefore no reason to narrow the passed argument
    SP--;
    write(STACK_BASE_ADDR + SP, PC);//The second argument gets converted to a u8 & therefore no reason to narrow the passed argument
    SP--;
    //We must disable the interrupt request
    setOrClearFlag(I, true);
    //And break command to indicate that it is in break mode
    setOrClearFlag(B, 1);
    write(0x0100 + SP, status);
    //Undo the above command after weve written it to memory
    setOrClearFlag(B, 0);
    SP--;

    //The interrupt vector is located at FFFE and this should be a macro called INTERRUPT_VECTOR_LOCATION
    //The interrupt vector location will itself house the location of where the PC needs to point to
    //In essence, before calling a break, the location of the break pointer needs to be placed in the interrupt vector at the very end of the memory address space
    PC = (u16)read(IRQBRK_LO_ADDR) | ((u16)read(IRQBRK_HI_ADDR) << 8);
    return 0;
}


u8 Processor::BVC(u16 addr_abs, u16 addr_rel){
    if (getFlag(V) == 0)
    {
        cycles++;
        addr_abs = PC + addr_rel;

        if ((addr_abs & 0xFF00) != (PC & 0xFF00))
            cycles++;

        PC = addr_abs;
    }
    return 0;
}


u8 Processor::BVS(u16 addr_abs, u16 addr_rel){
    if (getFlag(V) == 1)
    {
        cycles++;
        addr_abs = PC + addr_rel;
        if ((addr_abs & 0xFF00) != (PC & 0xFF00)) {
            cycles++;
        }
        PC = addr_abs;
    }
    return 0;
}


u8 Processor::CLC(u16 addr_abs, u16 addr_rel){
    setOrClearFlag(C, false);
    return 0;
}


u8 Processor::CLD(u16 addr_abs, u16 addr_rel){
    setOrClearFlag(D, false);
    return 0;
}


u8 Processor::CLI(u16 addr_abs, u16 addr_rel){
    setOrClearFlag(I, false);
    return 0;
}


u8 Processor::CLV(u16 addr_abs, u16 addr_rel){
    setOrClearFlag(V, false);
    return 0;
}

u8 Processor::CMP(u16 addr_abs, u16 addr_rel){
    u8 fetched = fetch(addr_abs);
    setOrClearFlag(C, A >= fetched);
    setOrClearFlag(Z, A == fetched);
    setOrClearFlag(N, (A - fetched) & BIT_SEVEN);//MACRO this. Also be consistent with 1 << 7 and using 0x0080
    return 1;
}

u8 Processor::CPX(u16 addr_abs, u16 addr_rel){
    u8 fetched = fetch(addr_abs);
    setOrClearFlag(C, X >= fetched);
    setOrClearFlag(Z, X == fetched);
    setOrClearFlag(N, (X - fetched) & BIT_SEVEN);//MACRO this. Also be consistent with 1 << 7 and using 0x0080
    return 0;
}

u8 Processor::CPY(u16 addr_abs, u16 addr_rel){
    u8 fetched = fetch(addr_abs);
    setOrClearFlag(C, Y >= fetched);
    setOrClearFlag(Z, Y == fetched);
    setOrClearFlag(N, (Y - fetched) & BIT_SEVEN);//MACRO this. Also be consistent with 1 << 7 and using 0x0080
    return 0;
}

u8 Processor::DEC(u16 addr_abs, u16 addr_rel){
    u8 fetched = fetch(addr_abs);
    fetched--;
    write(addr_abs, fetched);
    setOrClearFlag(Z, !fetched);
    setOrClearFlag(N, fetched & BIT_SEVEN);
    return 0;
}

u8 Processor::DEX(u16 addr_abs, u16 addr_rel){
    X--;
    setOrClearFlag(Z, !X);
    setOrClearFlag(N, X & BIT_SEVEN);
    return 0;
}

u8 Processor::DEY(u16 addr_abs, u16 addr_rel){
    Y--;
    setOrClearFlag(Z, !Y);
    setOrClearFlag(N, Y & BIT_SEVEN);
    return 0;
}

u8 Processor::EOR(u16 addr_abs, u16 addr_rel){
    u8 fetched = fetch(addr_abs);
    A ^= fetched;
    setOrClearFlag(Z, !A);
    setOrClearFlag(N, A & BIT_SEVEN);
    return 1;
}

u8 Processor::INC(u16 addr_abs, u16 addr_rel){
    u8 fetched = fetch(addr_abs);
    fetched++;
    write(addr_abs, fetched);
    setOrClearFlag(Z, !fetched);
    setOrClearFlag(N, fetched & BIT_SEVEN);
    return 0;
}

u8 Processor::INX(u16 addr_abs, u16 addr_rel){
    X++;
    setOrClearFlag(Z, !X);
    setOrClearFlag(N, X & BIT_SEVEN);
    return 0;
}

u8 Processor::INY(u16 addr_abs, u16 addr_rel){
    Y++;
    setOrClearFlag(Z, !Y);
    setOrClearFlag(N, Y & BIT_SEVEN);
    return 0;
}

u8 Processor::JMP(u16 addr_abs, u16 addr_rel){
    PC = addr_abs;
    return 0;
}

u8 Processor::JSR(u16 addr_abs, u16 addr_rel){
    u16 addressToBePushed = PC - 1;
    write(STACK_BASE_ADDR + SP, addressToBePushed >> 8);//If this is an error, then try (addressToBePushed >> 8) & 0x00FF
    SP--;
    write(STACK_BASE_ADDR + SP, addressToBePushed);//If this is an error, then try (addressToBePushed) & 0x00FF
    SP--;
    PC = addr_abs;
    return 0;
}

u8 Processor::LDA(u16 addr_abs, u16 addr_rel){
    u8 fetched = fetch(addr_abs);
    A = fetched;
    setOrClearFlag(Z, A == 0);
    setOrClearFlag(N, A & BIT_SEVEN);
    return 1;
}

u8 Processor::LDX(u16 addr_abs, u16 addr_rel){
    u8 fetched = fetch(addr_abs);
    X = fetched;
    setOrClearFlag(Z, X == 0);
    setOrClearFlag(N, X & BIT_SEVEN);
    return 1;
}

u8 Processor::LDY(u16 addr_abs, u16 addr_rel){
    u8 fetched = fetch(addr_abs);
    Y = fetched;
    setOrClearFlag(Z, Y == 0);
    setOrClearFlag(N, Y & BIT_SEVEN);
    return 1;
}

u8 Processor::LSR(u16 addr_abs, u16 addr_rel){
    u8 fetched = fetch(addr_abs);
    u16 originalFetched = fetched;
    fetched = fetched >> 1;
    fetched &= 0x7F;
    setOrClearFlag(C, originalFetched & BIT_ONE);
    setOrClearFlag(Z, fetched == 0);
    setOrClearFlag(N, fetched & BIT_SEVEN);
    if(lookup[opcode].addrmode == & Processor::IMM){
        A = fetched;
    }
    else{
        write(addr_abs, fetched);
    }
    return 0;
}

u8 Processor::NOP(u16 addr_abs, u16 addr_rel){
    return 0;
}

u8 Processor::ORA(u16 addr_abs, u16 addr_rel){
    u8 fetched = fetch(addr_abs);
    A |= fetched;
    setOrClearFlag(Z, A == 0);
    setOrClearFlag(N, A & BIT_SEVEN);
    return 1;
}

//6502 has a base location in memory for the stack pointer which is located in page 0x0100
//Add a MACRO for this so that it explains it
//The stack pointer grows downwards and therefore everytime we push onto it, we decrement
u8 Processor::PHA(u16 addr_abs, u16 addr_rel){
    write(STACK_BASE_ADDR + SP, A);
    SP--;
    return 0;
}


//" "
// PHP and BRK both always set bits 4 and 5 of the u8 they push on the stack. NMI and IRQ both always clear bit 4 and set bit 5 of the u8 they push on the stack.
// Thus, the status flags register only has 6 bits, not 8.
//If you read the descriptions of the B flag, you'll see that based on most descriptions that say it exists, there's no way to actually read its contents, hence it doesn't actually exist.
// It's like the E bit hidden away in every 6502 processor, which enables a super 128-bit processing mode, but that can't ever be set by any instruction unfortunately.
//" "
//I think that the B and U flags are cleared and that the break flag is set to 1 before a push. Double check if this is the case. Check:
//https://wiki.nesdev.com/w/index.php/Status_flags
//http://forums.nesdev.com/viewtopic.php?p=64224#p64224
//
u8 Processor::PHP(u16 addr_abs, u16 addr_rel){
    u8 temp = status;
    temp |= BIT_FOUR;
    temp |= BIT_FIVE;
    write(0x0100 + SP, temp);
    SP--;
    return 0;
}

//We first increment the stack pointer
//Macro the value 0x80
u8 Processor::PLA(u16 addr_abs, u16 addr_rel){
    SP++;
    A = read(STACK_BASE_ADDR + SP);
    setOrClearFlag(Z, A == 0x00);
    setOrClearFlag(N, A & BIT_SEVEN);
    return 0;
}

//Just check because the unused register U might be set to 1. Check David Barr and NES forums
u8 Processor::PLP(u16 addr_abs, u16 addr_rel){
    SP++;
    status = read(STACK_BASE_ADDR + SP);
    return 0;
}

u8 Processor::ROL(u16 addr_abs, u16 addr_rel){
    u8 fetched = fetch(addr_abs);
    u8 newCarryFlagValue = fetched & (BIT_SEVEN);
    u8 result = (fetched << 1) | getFlag(C);
    setOrClearFlag(C, newCarryFlagValue);
    setOrClearFlag(Z, A == 0);
    setOrClearFlag(N, result & BIT_SEVEN);
    if(lookup[opcode].addrmode == &Processor::IMP){
        A = result;
    }
    else{
        write(addr_abs, result);//Remember that add_abs was set in the addr mode call prior to calling the actual instruction
        //Note: Do NOT have to say PC++ because this is already done in the addressing mode
    }
    return 0;
}

u8 Processor::ROR(u16 addr_abs, u16 addr_rel){
    u8 fetched = fetch(addr_abs);
    u8 newCarryFlagValue = fetched & BIT_ZERO;
    u8 result = (fetched >> 1) & ~BIT_SEVEN;//Force the 7th(leading) bit to be zero
    result |= (getFlag(C) << 7);
    setOrClearFlag(C, newCarryFlagValue);
    setOrClearFlag(Z, A == 0);
    setOrClearFlag(N, result & BIT_SEVEN);
    if(lookup[opcode].addrmode == &Processor::IMP){
        A = result;
    }
    else{
        write(addr_abs, result);//Remember that add_abs was set in the addr mode call prior to calling the actual instruction
        //Note: Do NOT have to say PC++ because this is already done in the addressing mode
    }
    return 0;
}

//Return from interrupt
//Pg 25 from Synertek:
//The interrupt disable, I, is set by the microprocessor during reset and interrupt commands
//The I bit is reset by the CLI instruction or the PLP instruction, or at a return from interrupt in which the interrupt disable was reset prior to the interrupt

//Pg 132 from Synertek document:
//Because the interrupt disable had to be off for an interrupt request to have been honored, the return from interrupt which loads the processor
//status from before the interrupt occured has the effect of clearing the interrupt disable bit.

//Ibid:
//All operations should end with a single instruction which reinitialises the microprocessor back to the point at which the interrupt
//occured. This instruction is called the RTI instruction

//Pg 135 of Synertek:
//This [interrupt] ignoring is done by the interrupt disable bit which can be set on by the programmer and is initialised on by the interrupt sequence or by the start sequence

//When interrupting, the current status is saved (PC) and the the registers
u8 Processor::RTI(u16 addr_abs, u16 addr_rel){
    SP++;
    status = read(STACK_BASE_ADDR + SP);//The status register was the last value pushed on and so its the first value popped off
    SP++;
    PC = (u16)read(STACK_BASE_ADDR + SP);
    SP++;
    PC |= (u16)read(STACK_BASE_ADDR + SP) << 8;
    return 0;
}

u8 Processor::RTS(u16 addr_abs, u16 addr_rel){
    SP++;
    u16 low = read(STACK_BASE_ADDR + (SP));
    SP++;
    u16 high = read(STACK_BASE_ADDR + (SP)) << 8;
    addr_abs = high | low;
    PC++;
    return 0;
    //Do NOT need to update addr_abs thats only for feeding arguments to instructions
}

u8 Processor::SEC(u16 addr_abs, u16 addr_rel){
    setOrClearFlag(C, true);
    return 0;
}

u8 Processor::SED(u16 addr_abs, u16 addr_rel){
    setOrClearFlag(D, true);
    return 0;
}

u8 Processor::SEI(u16 addr_abs, u16 addr_rel){
    setOrClearFlag(I, true);
    return 0;
}

u8 Processor::STA(u16 addr_abs, u16 addr_rel){
    write(addr_abs, A);
    return 0;
}

u8 Processor::STX(u16 addr_abs, u16 addr_rel){
    write(addr_abs, X);
    return 0;
}

u8 Processor::STY(u16 addr_abs, u16 addr_rel){
    write(addr_abs, Y);
    return 0;
}

u8 Processor::TAX(u16 addr_abs, u16 addr_rel){
    X = A;
    setOrClearFlag(C, X == 0);
    setOrClearFlag(N, X & BIT_SEVEN);
    return 0;
}

u8 Processor::TAY(u16 addr_abs, u16 addr_rel){
    Y = A;
    setOrClearFlag(C, Y == 0);
    setOrClearFlag(N, Y & BIT_SEVEN);
    return 0;
}

u8 Processor::TSX(u16 addr_abs, u16 addr_rel){
    X = SP;
    setOrClearFlag(C, X == 0);
    setOrClearFlag(N, X & BIT_SEVEN);
    return 0;
}

u8 Processor::TXA(u16 addr_abs, u16 addr_rel){
    A = X;
    setOrClearFlag(C, A == 0);
    setOrClearFlag(N, A & BIT_SEVEN);
    return 0;
}

u8 Processor::TXS(u16 addr_abs, u16 addr_rel){
    SP = X;
    return 0;
}

u8 Processor::TYA(u16 addr_abs, u16 addr_rel){
    A = Y;
    setOrClearFlag(C, A == 0);
    setOrClearFlag(N, A & BIT_SEVEN);
    return 0;
}

//Invalid Instruction
u8 Processor::XXX(u16 addr_abs, u16 addr_rel){
    return 0;
}