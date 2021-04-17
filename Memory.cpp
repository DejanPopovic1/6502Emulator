#include "Memory.h"

Memory::Memory(){
    for(auto &i : ram) {
        i = 0x00;
    }
}

Memory::~Memory(){

}

//Rather use operator
void Memory::write(Word addr, Byte data){
    if(addr >= 0x0000 && addr <= 0xFFFF){
        this->ram[addr] = data;
    }
}

//Rather use operator
Byte Memory::read(Word addr, bool bReadOnly){
    if(addr >= 0x0000 && addr <= 0xFFFF){
        return this->ram[addr];
    }
    return 0x00;
}



//Byte Memory::operator[](u32 Address) const{
//    return Data[Address];
//}
//
//Byte & Memory::operator[](u32 Address){
//    return Data[Address];
//}
//
//void Memory::WriteWord(Word Value, u32 Address, s32 & Cycles){//Dean Note, add endianness options
//    Data[Address] = Value & 0xFF;
//    Data[Address + 1] = (Value >> 8);
//    Cycles -= 2;
//}