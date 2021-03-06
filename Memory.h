#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>
#include <array>
#include "Processor.h"

#define NUM_BYTES_IN_KB 1024
#define NUMBER_OF_KB 64

using u8 = uint8_t;
using u16 = uint16_t;

class Memory{
public:
    Memory();
    ~Memory();
    u8 operator[](u16 addr)const;
    u8 & operator[](u16 addr);
private:
    static constexpr uint32_t MAX_MEM = NUM_BYTES_IN_KB * NUMBER_OF_KB;
    std::array<u8, MAX_MEM> ram;
    bool isAddrInRange(const u16 addr)const;
};

#endif //MEMORY_H