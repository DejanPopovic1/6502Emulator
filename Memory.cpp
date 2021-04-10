//
// Created by DejanP on 2021/04/10.
//
#include "Memory.h"

void Memory::Initialise(){
    for(u32 i = 0; i < MAX_MEM; i++) {
        Data[i] = 0;
    }
}

Byte Memory::operator[](u32 Address) const{
    return Data[Address];
}

Byte & Memory::operator[](u32 Address){
    return Data[Address];
}

void Memory::WriteWord(Word Value, u32 Address, s32 & Cycles){//Dean Note, add endianness options
    Data[Address] = Value & 0xFF;
    Data[Address + 1] = (Value >> 8);
    Cycles -= 2;
}