#include <iostream>
#include <string>

struct CPU{
    using Byte = unsigned char;
    using Word = unsigned short;
    Word PC;
    Word SP;
    Byte A, X, Y;
    Byte C : 1;
    Byte Z : 1;
    Byte I : 1;
    Byte D : 1;
    Byte B : 1;
    Byte V : 1;
    Byte N : 1;

    void Reset(){
        PC = 0xFFFC;
        SP = 0x0100;
        C = Z = I = D = B = V = N = 0;
        A = X = Y = 0;
    }

};

int main() {
    CPU cpu;
    cpu.Reset();
    std::string test;
    std::cin >> test;
    return 0;
}
