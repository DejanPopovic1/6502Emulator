#include "Memory.h"

#define START_ADDRESS 0x0000
#define END_ADDRESS 0xFFFF

Memory::Memory(){
    for(auto &i : this->ram) {
        i = 0x00;
    }
}

Memory::~Memory(){

}

u8 Memory::operator[](u16 addr) const{
    if(isAddrInRange(addr)){
        return (this->ram)[addr];
    }
    return 0x00;
}

u8 & Memory::operator[](u16 addr){
    if(isAddrInRange(addr)){
        return (this->ram)[addr];
    }
    return (this->ram)[START_ADDRESS];
}

bool Memory::isAddrInRange(const u16 addr)const{
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