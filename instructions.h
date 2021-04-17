#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "Processor.h"
#include <stdint.h>

class Processor;

struct Instruction{
    uint8_t(Processor::*operate)() = nullptr;
    uint8_t(Processor::*addrmode)() = nullptr;
    uint8_t cycles = 0;
};

#endif //INSTRUCTIONS_H
