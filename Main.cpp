#include <iostream>
#include "LZWenc.cpp"
#include "LZWdec.cpp"

int LZWenc();
int LZWdec();

int main () {
    
    LZWenc();
    std::cout<<"Finished Encoding\n";
    LZWdec();
    std::cout<<"Finished Decoding";
}