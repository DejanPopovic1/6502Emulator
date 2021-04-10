//
// Created by DejanP on 2021/04/10.
//
#include "Processor.h"
#include "Memory.h"

Processor::Processor(){

}

Processor::~Processor(){

}

void Processor::connectMemory(Memory *m){
    this->mem = m;
}

uint8_t Processor::read(uint16_t a){
    return this->mem->read(a, false);
}

void Processor::write(uint16_t a, uint8_t d){
    this->mem->write(a, d);
}