// Inverse BWT Transform

#include <string.h>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <list>

// Parameters
std::string EncInputString;
std::string decodedout;

//Read file

void DecReadFile(std::string inputFileName)
{
    // read file and put it to string
    std::ifstream inputStream(inputFileName);
    std::stringstream sstream;
    sstream << inputStream.rdbuf();
    EncInputString = sstream.str();
    size_t dataSize = EncInputString.size();

}

void DecWriteFile(std::string outputFileName)
{
    // write the vector output to file
    std::ofstream outStream(outputFileName, std::ofstream::out | std::ofstream::binary);
    long outSize = decodedout.size();
    for (int i = 0; i < outSize; i++)
    {
        outStream.write((&decodedout[i]), sizeof(char));
    }
}

void invert(std::string bwt_arr)
{
    int i, len_bwt = bwt_arr.size();
    std::vector<uint8_t> sorted_bwt(len_bwt);
    std::copy(bwt_arr.begin(),bwt_arr.end(),sorted_bwt.begin());
    int *l_shift = new int[len_bwt];

    // Index at which original string appears
    int x = bwt_arr.find((char)2);
    if (x == std::string::npos)
        std::cout << "ERROR: char not found";

    // Sorts the characters of bwt_arr[] alphabetically
    std::sort(sorted_bwt.begin(), sorted_bwt.end());

    // Vector of Lists for every Character
    std::vector<std::list<int>> arr(256);
    // Adds the indices of every character to its list in arr ascendingly
    for (i = 0; i < len_bwt; i++)
    {
        arr[ (uint8_t)bwt_arr[i] ].push_back(i);
    }

    // takes the shifted version of each character
    for (i = 0; i < len_bwt; i++)
    {
        l_shift[i] = arr[(uint8_t)sorted_bwt[i] ].front();
        arr[(uint8_t)sorted_bwt[i] ].pop_front();
    }

    // set size of string
    decodedout.reserve(len_bwt);
    // Decodes the bwt
    for (i = 0; i < len_bwt; i++)
    {
        x = l_shift[x];
        decodedout += bwt_arr[x];
    }
    // remove last character
    decodedout.pop_back();
}


int BWTdec(std::string inputFileName, std::string outputFileName)
{
    DecReadFile(inputFileName);
    invert(&EncInputString[0]);
    DecWriteFile(outputFileName);
    return 0;
}
