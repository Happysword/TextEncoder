#include <iostream>
#include <fstream>
#include <ctime>
#include "LZWenc.cpp"
#include "LZWdec.cpp"
#include "BWTenc.cpp"
#include "BWTdec.cpp"

int LZWdec(std::string inputFileName, std::string outputFileName);
int LZWenc(std::string inputFileName, std::string outputFileName);
int BWTdec(std::string inputFileName, std::string outputFileName);
int BWTenc(std::string inputFileName, std::string outputFileName);

bool fileExists(const std::string &name)
{
    std::ifstream f(name.c_str());
    return f.good();
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "wrong number of parameters. Check Readme File\n";
        return 0;
    }
    if (std::string(argv[1]) == "-c")
    {
        if (!fileExists("enwik8"))
        {
            std::cout << "Can not find enwik8. Check Readme File\n";
            return 0;
        }
        std::cout << "Start Encode\n";
        const std::clock_t begin_time = std::clock();

        BWTenc("enwik8", "encodedtemp.txt");
        LZWenc("encodedtemp.txt", "encoded.bin");
        if (remove("encodedtemp.txt") != 0)
            std::cout << "Couldnt delete temp file\n";

        std::cout << "Finished Encoding\n";
        std::cout << "Time Elapsed: "<< float(clock() - begin_time) / CLOCKS_PER_SEC;
    }

    else if (std::string(argv[1]) == "-d")
    {
        if (!fileExists("encoded.bin"))
        {
            std::cout << "Can not find encoded.bin. Check Readme File\n";
            return 0;
        }
        std::cout << "Start Decode\n";
        const std::clock_t begin_time = std::clock();

        LZWdec("encoded.bin", "decodedtemp.txt");
        BWTdec("decodedtemp.txt", "decoded.txt");
        if (remove("decodedtemp.txt") != 0)
            std::cout << "Couldnt delete temp file\n";

        std::cout << "Finished Decoding\n";
        std::cout << "Time Elapsed: "<< float(clock() - begin_time) / CLOCKS_PER_SEC;
    }
    else
    {
        std::cout << "Wrong parameters. Check Readme File\n";
        return 0;
    }

    return 0;
}