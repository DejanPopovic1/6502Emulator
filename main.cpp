#include "Processor.h"
#include "Memory.h"

int main() {
    Processor p;
    Memory m;
    p.connectMemory(&m);
    return 0;
}
