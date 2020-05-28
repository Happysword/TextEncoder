#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>

const std::string inputFileName = "test.txt";
const std::string outputFileName = "output.bin";
std::string InputString;
std::vector<uint32_t> encodedOut;
std::unordered_map<std::string,uint32_t> dictionary;
void lzwEncode();
void ReadFile();
void WriteFile();

int main()
{
    // encode
    lzwEncode();
    return 0;
}

void lzwEncode() {
    ReadFile();
}

void ReadFile()
{
    // read file and put it to string
    std::ifstream inputStream(inputFileName);
    std::stringstream sstream;
    sstream << inputStream.rdbuf();
    InputString = sstream.str();
    size_t dataSize = InputString.size();
    std::cout << dataSize << std::endl<<InputString<<std::endl;
}

void WriteFile()
{
    // write the vector output to file
    std::ofstream outStream(outputFileName, std::ofstream::out | std::ofstream::binary);
    long outSize = encodedOut.size();
    for (int i = 0; i < outSize; i++)
    {
        outStream.write(reinterpret_cast<char *>(&encodedOut[i]), sizeof(uint32_t));
    }
}