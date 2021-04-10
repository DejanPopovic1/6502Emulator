#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>
#include <array>
#include "Processor.h"

using Byte = uint8_t;
using Word = uint16_t;
using u32 = uint32_t;
using s32 = int32_t;

class Memory{
public:
    Memory();
    ~Memory();
//    void Initialise();
//    Byte operator[](u32 Address) const;//Can we get away with 16 bits?
//    Byte & operator[](u32 Address);//Can we get away with 16 bits?
//    void WriteWord(Word Value, u32 Address, s32 & Cycles);//Can we get away with 16 bits?

    void write(Word addr, Byte data);
    Byte read(Word addr, bool bReadOnly = false);

private:
    static constexpr uint32_t MAX_MEM = 1024 * 64;
    //Byte Data[MAX_MEM];
    std::array<Byte, MAX_MEM> ram;
};

#endif //MEMORY_H


//Byte operator[](u32 Address) const;//Can we get away with 16 bits?
//Byte & operator[](u32 Address);//Can we get away with 16 bits?
//void WriteWord(Word Value, u32 Address, s32 & Cycles);//Can we get away with 16 bits?
//private:
//static constexpr uint32_t MAX_MEM = 1024 * 64;
//Byte Data[MAX_MEM];
//};