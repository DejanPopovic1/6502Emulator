#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>

using Byte = uint8_t;
using Word = uint16_t;
using u32 = uint32_t;
using s32 = int32_t;

class Memory{
public:
    void Initialise();
    Byte operator[](u32 Address) const;//Can we get away with 16 bits?
    Byte & operator[](u32 Address);//Can we get away with 16 bits?
    void WriteWord(Word Value, u32 Address, s32 & Cycles);//Can we get away with 16 bits?
private:
    static constexpr uint32_t MAX_MEM = 1024 * 64;
    Byte Data[MAX_MEM];
};

#endif //MEMORY_H
