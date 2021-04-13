//
// Created by DejanP on 2021/04/10.
//
#include "Processor.h"
#include "Memory.h"

Processor::Processor(){
    using a = Processor;
    //Rather add this in a separate text resource file
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

void Processor::connectMemory(Memory *m){
    this->mem = m;
}

uint8_t Processor::read(uint16_t a){
    return this->mem->read(a, false);
}

void Processor::write(uint16_t a, uint8_t d){
    this->mem->write(a, d);
}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//Understand this in more detail
//Use AND() as an example to run through this
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void Processor::clock(){
    if(cycles == 0){
        opcode = read(this->PC);
        this->PC++;

        cycles = lookup[opcode].cycles;

        uint8_t additional_cycle1 = (this->*lookup[opcode].addrmode)();
        uint8_t additional_cycle2 = (this->*lookup[opcode].operate)();
        cycles += (additional_cycle1 & additional_cycle2);

    }

    cycles--;
}

//Come back to this and understand this a bit better
//Need to understand the opcode field. Seems to be indexed number
uint8_t Processor::fetch(){
    if(!(lookup[opcode].addrmode == &Processor::IMP)){
        fetched = read(addr_abs);
    }
    return this->fetched;


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
    if((addr_abs & 0xFF00) != (highByte << 8)){
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
        return 1;
    }
    else{
        return 0;
    };
    return 0;
}

uint8_t Processor::ADC()
{

    return 1;
}



uint8_t Processor::SBC()
{

    return 1;
}

uint8_t Processor::AND()
{
    fetch();
    this->A = this->A & this->fetched;
    setOrClearFlag(Z, A == 0x00);
    setOrClearFlag(N, A & 0x80);
    return 1;
}


uint8_t Processor::ASL()
{

    return 0;
}


uint8_t Processor::BCC()
{
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

uint8_t Processor::BCS()
{
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

uint8_t Processor::BEQ()
{
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

uint8_t Processor::BIT()
{

    return 0;
}


uint8_t Processor::BMI()
{
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

uint8_t Processor::BNE()
{
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

uint8_t Processor::BPL()
{
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


uint8_t Processor::BRK()
{
    return 0;
}


uint8_t Processor::BVC()
{
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


uint8_t Processor::BVS()
{
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


uint8_t Processor::CLC()
{
    setOrClearFlag(C, false);
    return 0;
}


uint8_t Processor::CLD()
{

    return 0;
}


uint8_t Processor::CLI()
{
    return 0;
}


uint8_t Processor::CLV()
{

    return 0;
}


uint8_t Processor::CMP()
{

    return 1;
}


uint8_t Processor::CPX()
{

    return 0;
}


uint8_t Processor::CPY()
{

    return 0;
}


uint8_t Processor::DEC()
{

    return 0;
}


uint8_t Processor::DEX()
{

    return 0;
}


uint8_t Processor::DEY()
{

    return 0;
}


uint8_t Processor::EOR()
{

    return 1;
}


uint8_t Processor::INC()
{

    return 0;
}


uint8_t Processor::INX()
{

    return 0;
}

uint8_t Processor::INY()
{

    return 0;
}


uint8_t Processor::JMP()
{

    return 0;
}

uint8_t Processor::JSR()
{

    return 0;
}


uint8_t Processor::LDA()
{

    return 1;
}


uint8_t Processor::LDX()
{

    return 1;
}


uint8_t Processor::LDY()
{

    return 1;
}

uint8_t Processor::LSR()
{

    return 0;
}

uint8_t Processor::NOP()
{

    return 0;
}


uint8_t Processor::ORA()
{

    return 1;
}


uint8_t Processor::PHA()
{

    return 0;
}


uint8_t Processor::PHP()
{

    return 0;
}


uint8_t Processor::PLA()
{

    return 0;
}


uint8_t Processor::PLP()
{

    return 0;
}

uint8_t Processor::ROL()
{

    return 0;
}

uint8_t Processor::ROR()
{

    return 0;
}

uint8_t Processor::RTI()
{

    return 0;
}

uint8_t Processor::RTS()
{

    return 0;
}


uint8_t Processor::SEC()
{

    return 0;
}


uint8_t Processor::SED()
{
    return 0;
}


uint8_t Processor::SEI()
{

    return 0;
}


uint8_t Processor::STA()
{

    return 0;
}

uint8_t Processor::STX()
{

    return 0;
}

uint8_t Processor::STY()
{

    return 0;
}

uint8_t Processor::TAX()
{

    return 0;
}



uint8_t Processor::TAY()
{

    return 0;
}

uint8_t Processor::TSX()
{

    return 0;
}

uint8_t Processor::TXA()
{

    return 0;
}

uint8_t Processor::TXS()
{
    return 0;
}

uint8_t Processor::TYA()
{
    return 0;
}

uint8_t Processor::XXX()
{
    return 0;
}


