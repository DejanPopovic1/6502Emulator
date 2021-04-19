#include "Memory.h"

#define START_ADDRESS 0x0000
#define END_ADDRESS 0xFFFF

//If the below is not included, it technically isn't valid C++ code. See:
//https://stackoverflow.com/questions/42756443/undefined-reference-with-gtest
constexpr uint32_t Memory::MAX_MEM;

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