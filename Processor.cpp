//
// Created by DejanP on 2021/04/10.
//
#include "Processor.h"
#include "Memory.h"

//Derive from an abstract class to derive the 6502 processor class
//There are too many internal class variables. Following program and function execution is therefore very difficult.
//Rather pass around variables between the functions and cut down on the class variables

//Use setIf flag and setOrClearFlag

//Be careful when handling flags. Must change some of them. Sometimes they must be explicitly set and otherwise nothing is done. Other times, the value of the flag must be equal to
//the value of the bit
//When adding the dis assembler, it should be in a separate class called disassembler. In any case this should be in backlog
Processor::Processor(){
    using a = Processor;
    //Rather add this in a separate text resource file
    //The name is one to one correlation to the function name. Perhaps give the full name instead? Will need to align in this case
    this->lookup =
            {
                    { "BRK", &a::BRK, &a::IMM, 7 },{ "ORA", &a::ORA, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::ZP0, 3 },{ "ASL", &a::ASL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHP", &a::PHP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::IMM, 2 },{ "ASL", &a::ASL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABS, 4 },{ "ASL", &a::ASL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
                    { "BPL", &a::BPL, &a::REL, 2 },{ "ORA", &a::ORA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ZPX, 4 },{ "ASL", &a::ASL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLC", &a::CLC, &a::IMP, 2 },{ "ORA", &a::ORA, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABX, 4 },{ "ASL", &a::ASL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
                    { "JSR", &a::JSR, &a::ABS, 6 },{ "AND", &a::AND, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "BIT", &a::BIT, &a::ZP0, 3 },{ "AND", &a::AND, &a::ZP0, 3 },{ "ROL", &a::ROL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLP", &a::PLP, &a::IMP, 4 },{ "AND", &a::AND, &a::IMM, 2 },{ "ROL", &a::ROL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "BIT", &a::BIT, &a::ABS, 4 },{ "AND", &a::AND, &a::ABS, 4 },{ "ROL", &a::ROL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
                    { "BMI", &a::BMI, &a::REL, 2 },{ "AND", &a::AND, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ZPX, 4 },{ "ROL", &a::ROL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEC", &a::SEC, &a::IMP, 2 },{ "AND", &a::AND, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ABX, 4 },{ "ROL", &a::ROL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
                    { "RTI", &a::RTI, &a::IMP, 6 },{ "EOR", &a::EOR, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "EOR", &a::EOR, &a::ZP0, 3 },{ "LSR", &a::LSR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHA", &a::PHA, &a::IMP, 3 },{ "EOR", &a::EOR, &a::IMM, 2 },{ "LSR", &a::LSR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::ABS, 3 },{ "EOR", &a::EOR, &a::ABS, 4 },{ "LSR", &a::LSR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
                    { "BVC", &a::BVC, &a::REL, 2 },{ "EOR", &a::EOR, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ZPX, 4 },{ "LSR", &a::LSR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLI", &a::CLI, &a::IMP, 2 },{ "EOR", &a::EOR, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ABX, 4 },{ "LSR", &a::LSR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
                    { "RTS", &a::RTS, &a::IMP, 6 },{ "ADC", &a::ADC, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ADC", &a::ADC, &a::ZP0, 3 },{ "ROR", &a::ROR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLA", &a::PLA, &a::IMP, 4 },{ "ADC", &a::ADC, &a::IMM, 2 },{ "ROR", &a::ROR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::IND, 5 },{ "ADC", &a::ADC, &a::ABS, 4 },{ "ROR", &a::ROR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
                    { "BVS", &a::BVS, &a::REL, 2 },{ "ADC", &a::ADC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ZPX, 4 },{ "ROR", &a::ROR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEI", &a::SEI, &a::IMP, 2 },{ "ADC", &a::ADC, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ABX, 4 },{ "ROR", &a::ROR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
                    { "???", &a::NOP, &a::IMP, 2 },{ "STA", &a::STA, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZP0, 3 },{ "STA", &a::STA, &a::ZP0, 3 },{ "STX", &a::STX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "DEY", &a::DEY, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 2 },{ "TXA", &a::TXA, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "STY", &a::STY, &a::ABS, 4 },{ "STA", &a::STA, &a::ABS, 4 },{ "STX", &a::STX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
                    { "BCC", &a::BCC, &a::REL, 2 },{ "STA", &a::STA, &a::IZY, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZPX, 4 },{ "STA", &a::STA, &a::ZPX, 4 },{ "STX", &a::STX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "TYA", &a::TYA, &a::IMP, 2 },{ "STA", &a::STA, &a::ABY, 5 },{ "TXS", &a::TXS, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::NOP, &a::IMP, 5 },{ "STA", &a::STA, &a::ABX, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::XXX, &a::IMP, 5 },
                    { "LDY", &a::LDY, &a::IMM, 2 },{ "LDA", &a::LDA, &a::IZX, 6 },{ "LDX", &a::LDX, &a::IMM, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "LDY", &a::LDY, &a::ZP0, 3 },{ "LDA", &a::LDA, &a::ZP0, 3 },{ "LDX", &a::LDX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "TAY", &a::TAY, &a::IMP, 2 },{ "LDA", &a::LDA, &a::IMM, 2 },{ "TAX", &a::TAX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "LDY", &a::LDY, &a::ABS, 4 },{ "LDA", &a::LDA, &a::ABS, 4 },{ "LDX", &a::LDX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
                    { "BCS", &a::BCS, &a::REL, 2 },{ "LDA", &a::LDA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "LDY", &a::LDY, &a::ZPX, 4 },{ "LDA", &a::LDA, &a::ZPX, 4 },{ "LDX", &a::LDX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "CLV", &a::CLV, &a::IMP, 2 },{ "LDA", &a::LDA, &a::ABY, 4 },{ "TSX", &a::TSX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 4 },{ "LDY", &a::LDY, &a::ABX, 4 },{ "LDA", &a::LDA, &a::ABX, 4 },{ "LDX", &a::LDX, &a::ABY, 4 },{ "???", &a::XXX, &a::IMP, 4 },
                    { "CPY", &a::CPY, &a::IMM, 2 },{ "CMP", &a::CMP, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPY", &a::CPY, &a::ZP0, 3 },{ "CMP", &a::CMP, &a::ZP0, 3 },{ "DEC", &a::DEC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INY", &a::INY, &a::IMP, 2 },{ "CMP", &a::CMP, &a::IMM, 2 },{ "DEX", &a::DEX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "CPY", &a::CPY, &a::ABS, 4 },{ "CMP", &a::CMP, &a::ABS, 4 },{ "DEC", &a::DEC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
                    { "BNE", &a::BNE, &a::REL, 2 },{ "CMP", &a::CMP, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ZPX, 4 },{ "DEC", &a::DEC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLD", &a::CLD, &a::IMP, 2 },{ "CMP", &a::CMP, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ABX, 4 },{ "DEC", &a::DEC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
                    { "CPX", &a::CPX, &a::IMM, 2 },{ "SBC", &a::SBC, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPX", &a::CPX, &a::ZP0, 3 },{ "SBC", &a::SBC, &a::ZP0, 3 },{ "INC", &a::INC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INX", &a::INX, &a::IMP, 2 },{ "SBC", &a::SBC, &a::IMM, 2 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::SBC, &a::IMP, 2 },{ "CPX", &a::CPX, &a::ABS, 4 },{ "SBC", &a::SBC, &a::ABS, 4 },{ "INC", &a::INC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
                    { "BEQ", &a::BEQ, &a::REL, 2 },{ "SBC", &a::SBC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ZPX, 4 },{ "INC", &a::INC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SED", &a::SED, &a::IMP, 2 },{ "SBC", &a::SBC, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ABX, 4 },{ "INC", &a::INC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
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
        addr_abs = 0xFFFE;
        uint16_t low = read(addr_abs + 0);
        uint16_t high = read(addr_abs + 1);
        PC = (high << 8) | low;
        cycles = 7;
    }
}

//Special dedicated purpose address used. Encode into MACRO
void Processor::nmi(){
    if(!getFlag(I)) {
        write(0x0100 + SP, (PC >> 8) & 0x00FF);
        SP--;
        write(0x0100 + SP, PC & 0x00FF);
        SP--;
        setOrClearFlag(B, 0);
        setOrClearFlag(U, 1);
        setOrClearFlag(I, 1);
        write(0x0100 + SP, status);
        SP--;
        addr_abs = 0xFFFA;
        uint16_t low = read(addr_abs + 0);
        uint16_t high = read(addr_abs + 1);
        PC = (high << 8) | low;
        cycles = 8;
    }
}

void Processor::connectMemory(Memory *m){
    this->mem = m;
}

uint8_t Processor::read(uint16_t a){
    return this->mem->read(a, false);
}

void Processor::write(uint16_t addressSpace, uint8_t b){
    this->mem->write(addressSpace, b);
}

//Next two functions. Clock calls the instruction and the instruction calls fetch

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//Understand this in more detail
//Use AND() as an example to run through this
//This is the main "runner" of the processor and the function pointer runs the instruction
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void Processor::clock(){
    //If cycles == 0, then it means that the next instruction is due, together with its operands.
    //If its non-zero, it means an instruction, and the fetching of its operands, is currently being executed and we just decrement our way through it
    if(cycles == 0){
        opcode = read(this->PC);
        this->PC++;
        cycles = lookup[opcode].cycles;
        uint8_t additional_cycle1 = (this->*lookup[opcode].addrmode)();//We must fix this. This function addrmode changes the class variable state addr_abs and other variables including PC
        //At this point in time, we have fetched the absolute address depending on the address mode above. Only now can we operate on the addr_abs
        uint8_t additional_cycle2 = (this->*lookup[opcode].operate)();
        cycles += (additional_cycle1 & additional_cycle2);//We must fix this. Using bit operations on this seems like overkill. Add_cycle_1 and add_cycle_2 either returns a 0 or 1. Make this explicit here.
    }
    cycles--;
}

//Fetch returns a value we hardly ever use. It rather changes internal class variables. Should it therefore not be redesigned? (it also adds to confusion and code tracing)
uint8_t Processor::fetch(){
    if(!(lookup[opcode].addrmode == &Processor::IMP)){
        fetched = read(addr_abs);
    }
    return this->fetched;


}

//Refactor into read Byte
void Processor::reset(){
    A = 0;
    X = 0;
    Y = 0;
    SP = 0xFD;
    status = 0x00;
    addr_abs = 0xFFFC;
    uint16_t low = read(addr_abs + 0);
    uint16_t high = read(addr_abs + 1);
    PC = (high << 8) | low;
    addr_rel = 0x0000;
    addr_abs = 0x0000;
    fetched = 0x00;

    cycles = 8;

}

void Processor::setOrClearFlag(enum flagsRegister, bool isSet){
    if(isSet){
        setFlag(Processor::flagsRegister);
    }
    else if(!isSet) {
        clearFlag(flagsRegister);
    }
}

void Processor::setFlag(enum flagsRegister f){
    status |= f;
}

void Processor::clearFlag(enum flagsRegister f){
    status &= ~f;
}

Byte Processor::getFlag(enum flagsRegister f){
    Byte test = (status & f);
    if(test > 0){
        return 1;
    }
    else{
        return 0;
    }
}

//The addressing modes all return the absolute address and secondary they tell us if there are any additional instructions required. They do nothing else

//Implied still "fetches" the operand, albeit not from memory. Its in the accumulator.
uint8_t Processor::IMP(){
    this->fetched = this->A;
    return 0;
}

uint8_t Processor::ZP0(){
    addr_abs = read(PC);
    PC++;
    addr_abs &= 0x00FF;
    return 0;
}

//Implement wrap around
uint8_t Processor::ZPY(){
    addr_abs = read(PC) + this->Y;
    PC++;
    addr_abs &= 0x00FF;
    return 0;
}

uint8_t Processor::ABS(){
    uint16_t lowByte = read(PC);
    PC++;
    uint16_t highByte = read(PC);
    PC++;
    this->addr_abs = (highByte << 8) | lowByte;
    return 0;
}

uint8_t Processor::ABY(){
    uint16_t lowByte = read(this->PC);
    this->PC++;
    uint16_t highByte = read(this->PC);
    this->PC++;
    this->addr_abs = (highByte << 8) | lowByte;
    this->addr_abs += this->Y;
    //Cross page boundary
    if((addr_abs & 0xFF00) != (highByte << 8)){
        return 1;//"May" need an additional clock cycle
    }
    else {
        return 0;
    }
}

//"Both memory locations specifying the effective address must be in zero page"
uint8_t Processor::IZX(){
    uint16_t t = read(this->PC);
    this->PC++;
    uint16_t low = read((uint16_t)(t + (uint16_t)X) & 0x00FF);
    uint16_t high = read((uint16_t)(t + (uint16_t)X + 1) & 0x00FF);
    this->addr_abs = (high << 8) | low;
    return 0;
}

uint8_t Processor::IMM(){
    addr_abs = PC++;
    return 0;
}

//Must implement a wrap around here!
uint8_t Processor::ZPX(){
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
uint8_t Processor::REL(){
    addr_rel = read(PC);
    PC++;
    if(addr_rel & 0x80){
        addr_rel |= 0xFF00;
    }
    return 0;
}

uint8_t Processor::ABX(){
    uint16_t lowByte = read(this->PC);
    this->PC++;
    uint16_t highByte = read(this->PC);
    this->PC++;
    this->addr_abs = (highByte << 8) | lowByte;
    this->addr_abs += this->X;
    //Cross page boundary
    if((addr_abs & 0xFF00) != (highByte << 8)){//Cross page boundary
        return 1;//"May" need an additional clock cycle
    }
    else {
        return 0;
    }
}

uint8_t Processor::IND(){
    uint16_t ptrLow = read(this->PC);
    this->PC++;
    uint16_t ptrHigh = read(this->PC);
    this->PC++;
    uint16_t ptr = (ptrHigh << 8) | ptrLow;
    if(ptrLow == 0x00FF){
        this->addr_abs = (read(ptr & 0xFF00) << 8) | read(ptr + 0);
    }
    else {
        this->addr_abs = (read(ptr + 1) << 8) | read(ptr + 0);
    }

    return 0;
}

uint8_t Processor::IZY(){
    uint16_t t = read(this->PC);
    this->PC++;
    uint16_t low = read(t & 0x00FF);
    uint16_t high = read((t + 1) & 0x00FF);
    this->addr_abs = (high << 8) | low;
    addr_abs += Y;
    if(addr_abs & 0xFF00 != (high << 8)){
        return 1;//"May" need an additional clock cycle
    }
    else{
        return 0;
    };
    return 0;
}


//====================
//INSTRUCTIONS
//====================

uint8_t Processor::ADC(){
    fetch();
    uint16_t temp = (uint16_t)A + (uint16_t)fetched + (uint16_t)getFlag(C);
    setOrClearFlag(C, temp > 255);
    setOrClearFlag(Z, (temp & 0x00FF) == 0);
    setOrClearFlag(N, temp & 0x80);
    setOrClearFlag(V, (~((uint16_t)A ^ (uint16_t)fetched) & ((uint16_t)A ^ (uint16_t)temp)) & 0x0080);
    A = temp & 0x00FF;
    return 1;
}



uint8_t Processor::SBC(){
    fetch();
    uint16_t value = ((uint16_t)fetched) ^ 0x00FF;
    uint16_t temp = (uint16_t)A + value + (uint16_t)getFlag(C);
    setOrClearFlag(C, temp & 0xFF00);
    setOrClearFlag(Z, ((temp & 0x00FF) == 0));
    setOrClearFlag(V, (temp ^ (uint16_t)A) & (temp ^ value) & 0x0080);
    setOrClearFlag(N, temp & 0x0080);
    A = temp & 0x00FF;
    return 1;
}

uint8_t Processor::AND(){
    fetch();
    this->A = this->A & this->fetched;
    setOrClearFlag(Z, A == 0x00);
    setOrClearFlag(N, A & 0x80);
    return 1;
}


uint8_t Processor::ASL(){
    fetch();//If its implied addressing mode, it wont update any internal variables. One of the addressing modes is implied.
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


uint8_t Processor::BCC(){
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

uint8_t Processor::BCS(){
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

uint8_t Processor::BEQ(){
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

uint8_t Processor::BIT(){
    fetch();
    setOrClearFlag(Z, !fetched & A);
    setOrClearFlag(V, fetched & 0x40);//Sixth bit. Add macro's. Rather use 1 << 6 as the macro
    setOrClearFlag(N, fetched & 0x80);//Seventh bit. Add macro's. Rather use 1 << 7 as the macro
    return 0;
}


uint8_t Processor::BMI(){
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

uint8_t Processor::BNE(){
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

uint8_t Processor::BPL(){
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

uint8_t Processor::BRK(){
    fetch();//The only mode for BRK is implied and therefore this statement could be ignored.
    PC++;

    write(0x0100 + SP, PC >> 8);//The second argument gets converted to a byte & therefore no reason to narrow the passed argument
    SP--;
    write(0x0100 + SP, PC);//The second argument gets converted to a byte & therefore no reason to narrow the passed argument
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
    PC = (uint16_t)read(0xFFFE) | ((uint16_t)read(0xFFFF) << 8);
    return 0;
}


uint8_t Processor::BVC(){
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


uint8_t Processor::BVS(){
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


uint8_t Processor::CLC(){
    setOrClearFlag(C, false);
    return 0;
}


uint8_t Processor::CLD(){
    setOrClearFlag(D, false);
    return 0;
}


uint8_t Processor::CLI(){
    setOrClearFlag(I, false);
    return 0;
}


uint8_t Processor::CLV(){
    setOrClearFlag(V, false);
    return 0;
}

uint8_t Processor::CMP(){
    fetch();
    setOrClearFlag(C, A >= fetched);
    setOrClearFlag(Z, A == fetched);
    setOrClearFlag(N, (A - fetched) & (1 << 7));//MACRO this. Also be consistent with 1 << 7 and using 0x0080
    return 1;
}

uint8_t Processor::CPX(){
    fetch();
    setOrClearFlag(C, X >= fetched);
    setOrClearFlag(Z, X == fetched);
    setOrClearFlag(N, (X - fetched) & (1 << 7));//MACRO this. Also be consistent with 1 << 7 and using 0x0080
    return 0;
}

uint8_t Processor::CPY(){
    fetch();
    setOrClearFlag(C, Y >= fetched);
    setOrClearFlag(Z, Y == fetched);
    setOrClearFlag(N, (Y - fetched) & (1 << 7));//MACRO this. Also be consistent with 1 << 7 and using 0x0080
    return 0;
}

uint8_t Processor::DEC(){
    fetch();
    fetched--;
    write(addr_abs, fetched);
    setOrClearFlag(Z, !fetched);
    setOrClearFlag(N, fetched & (1 << 7));
    return 0;
}

uint8_t Processor::DEX(){
    X--;
    setOrClearFlag(Z, !X);
    setOrClearFlag(N, X & (1 << 7));
    return 0;
}

uint8_t Processor::DEY(){
    Y--;
    setOrClearFlag(Z, !Y);
    setOrClearFlag(N, Y & (1 << 7));
    return 0;
}

uint8_t Processor::EOR(){
    fetch();
    A ^= fetched;
    setOrClearFlag(Z, !A);
    setOrClearFlag(N, A & (1 << 7));
    return 1;
}

uint8_t Processor::INC(){
    fetch();
    fetched++;
    write(addr_abs, fetched);
    setOrClearFlag(Z, !fetched);
    setOrClearFlag(N, fetched & (1 << 7));
    return 0;
}

uint8_t Processor::INX(){
    X++;
    setOrClearFlag(Z, !X);
    setOrClearFlag(N, X & (1 << 7));
    return 0;
}

uint8_t Processor::INY(){
    Y++;
    setOrClearFlag(Z, !Y);
    setOrClearFlag(N, Y & (1 << 7));
    return 0;
}

uint8_t Processor::JMP(){
    PC = addr_abs;
    return 0;
}

uint8_t Processor::JSR(){
    uint16_t addressToBePushed = PC - 1;
    write(0x0100 + SP, addressToBePushed >> 8);//If this is an error, then try (addressToBePushed >> 8) & 0x00FF
    SP--;
    write(0x0100 + SP, addressToBePushed);//If this is an error, then try (addressToBePushed) & 0x00FF
    SP--;
    PC = addr_abs;
    return 0;
}

uint8_t Processor::LDA(){
    fetch();
    A = fetched;
    setOrClearFlag(Z, A == 0);
    setOrClearFlag(N, A & (1 << 7));
    return 1;
}

uint8_t Processor::LDX(){
    fetch();
    X = fetched;
    setOrClearFlag(Z, X == 0);
    setOrClearFlag(N, X & (1 << 7));
    return 1;
}

uint8_t Processor::LDY(){
    fetch();
    Y = fetched;
    setOrClearFlag(Z, Y == 0);
    setOrClearFlag(N, Y & (1 << 7));
    return 1;
}

uint8_t Processor::LSR(){
    fetch();
    uint16_t originalFetched = fetched;
    fetched = fetched >> 1;
    fetched &= 0x7F;
    setOrClearFlag(C, originalFetched & 0x01);
    setOrClearFlag(Z, fetched == 0);
    setOrClearFlag(N, fetched & (1 << 7));
    if(lookup[opcode].addrmode == & Processor::IMM){
        A = fetched;
    }
    else{
        write(addr_abs, fetched);
    }
    return 0;
}

uint8_t Processor::NOP(){
    return 0;
}

uint8_t Processor::ORA(){
    fetch();
    A |= fetched;
    setOrClearFlag(Z, A == 0);
    setOrClearFlag(N, A & (1 << 7));
    return 1;
}

//6502 has a base location in memory for the stack pointer which is located in page 0x0100
//Add a MACRO for this so that it explains it
//The stack pointer grows downwards and therefore everytime we push onto it, we decrement
uint8_t Processor::PHA(){
    write(0x0100 + SP, A);
    SP--;
    return 0;
}


//" "
// PHP and BRK both always set bits 4 and 5 of the byte they push on the stack. NMI and IRQ both always clear bit 4 and set bit 5 of the byte they push on the stack.
// Thus, the status flags register only has 6 bits, not 8.
//If you read the descriptions of the B flag, you'll see that based on most descriptions that say it exists, there's no way to actually read its contents, hence it doesn't actually exist.
// It's like the E bit hidden away in every 6502 processor, which enables a super 128-bit processing mode, but that can't ever be set by any instruction unfortunately.
//" "
//I think that the B and U flags are cleared and that the break flag is set to 1 before a push. Double check if this is the case. Check:
//https://wiki.nesdev.com/w/index.php/Status_flags
//http://forums.nesdev.com/viewtopic.php?p=64224#p64224
//
uint8_t Processor::PHP(){
    uint8_t temp = status;
    temp |= (1 << 4);
    temp |= (1 << 5);
    write(0x0100 + SP, temp);
    SP--;
    return 0;
}

//We first increment the stack pointer
//Macro the value 0x80
uint8_t Processor::PLA(){
    SP++;
    A = read(0x0100 + SP);
    setOrClearFlag(Z, A == 0x00);
    setOrClearFlag(N, A & 0x80);
    return 0;
}

//Just check because the unused register U might be set to 1. Check David Barr and NES forums
uint8_t Processor::PLP(){
    SP++;
    status = read(0x0100 + SP);
    return 0;
}

uint8_t Processor::ROL(){
    fetch();
    uint8_t newCarryFlagValue = fetched & (1 << 7);
    uint8_t result = (fetched << 1) | getFlag(C);
    setOrClearFlag(C, newCarryFlagValue);
    setOrClearFlag(Z, A == 0);
    setOrClearFlag(N, result & (1 << 7));
    if(lookup[opcode].addrmode == &Processor::IMP){
        A = result;
    }
    else{
        write(addr_abs, result);//Remember that add_abs was set in the addr mode call prior to calling the actual instruction
        //Note: Do NOT have to say PC++ because this is already done in the addressing mode
    }
    return 0;
}

uint8_t Processor::ROR(){
    fetch();
    uint8_t newCarryFlagValue = fetched & (1 << 0);
    uint8_t result = (fetched >> 1) & ~(1 << 7);//Force the 7th(leading) bit to be zero
    result |= (getFlag(C) << 7);
    setOrClearFlag(C, newCarryFlagValue);
    setOrClearFlag(Z, A == 0);
    setOrClearFlag(N, result & (1 << 7));
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
uint8_t Processor::RTI(){
    SP++;
    status = read(0x0100 + SP);//The status register was the last value pushed on and so its the first value popped off
    SP++;
    PC = (uint16_t)read(0x0100 + SP);
    SP++;
    PC |= (uint16_t)read(0x0100 + SP) << 8;
    return 0;
}

uint8_t Processor::RTS(){
    SP++;
    uint16_t low = read(0x0100 + (SP));
    SP++;
    uint16_t high = read(0x0100 + (SP)) << 8;
    addr_abs = high | low;
    PC++;
    return 0;
    //Do NOT need to update addr_abs thats only for feeding arguments to instructions
}

uint8_t Processor::SEC(){
    setOrClearFlag(C, true);
    return 0;
}

uint8_t Processor::SED(){
    return 0;
}

uint8_t Processor::SEI(){

    return 0;
}

uint8_t Processor::STA(){

    return 0;
}

uint8_t Processor::STX(){

    return 0;
}

uint8_t Processor::STY(){

    return 0;
}

uint8_t Processor::TAX(){

    return 0;
}

uint8_t Processor::TAY(){

    return 0;
}

uint8_t Processor::TSX(){

    return 0;
}

uint8_t Processor::TXA(){

    return 0;
}

uint8_t Processor::TXS(){
    return 0;
}

uint8_t Processor::TYA(){
    return 0;
}

uint8_t Processor::XXX(){
    return 0;
}