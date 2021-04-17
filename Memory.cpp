#include "Memory.h"

#define START_ADDRESS 0x0000
#define END_ADDRESS 0xFFFF

Memory::Memory(){
    for(auto &i : ram) {
        i = 0x00;
    }
}

Memory::~Memory(){

}

//Rather use operator
void Memory::write(u16 addr, u8 data){
    if(isAddrInRange(addr)){
        this->ram[addr] = data;
    }
}

//Rather use operator
Byte Memory::read(u16 addr){
    if(isAddrInRange(addr)){
        return this->ram[addr];
    }
    return 0x00;
}

bool Memory::isAddrInRange(u16 addr){
    return addr >= START_ADDRESS && addr <= END_ADDRESS;
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