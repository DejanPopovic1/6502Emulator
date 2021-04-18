#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "Processor.h"
#include <stdint.h>

using u16 = uint16_t;

class Processor;

struct Instruction{
    uint8_t(Processor::*operate)(u16 addr_abs, u16 addr_rel) = nullptr;
    uint8_t(Processor::*addrmode)(u16 &PC, u16 &addr_abs, u16 &addr_rel) = nullptr;
    uint8_t cycles = 0;
};

#endif //INSTRUCTIONS_H
