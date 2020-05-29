#include <string.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>

// Parameters
std::string InputString;
std::string encodedOut;

//Read file

void ReadFile(std::string inputFileName)
{
    // read file and put it to string
    std::ifstream inputStream(inputFileName);
    std::stringstream sstream;
    sstream << inputStream.rdbuf();
    InputString = sstream.str();
    InputString += (char)2;
    size_t dataSize = InputString.size();
}

// Write file
void WriteFile(std::string outputFileName)
{
    // write the vector output to file
    std::ofstream outStream(outputFileName, std::ofstream::out | std::ofstream::binary);
    long outSize = encodedOut.size();
    for (int i = 0; i < outSize; i++)
    {
        outStream.write((&encodedOut[i]), sizeof(char));
    }
}

// class to store the rotations of strings
class rotation
{
public:
    int index;
    char *suffix;
};

// Compares the rotations and sorts alphabetically
int cmpfunc(const void *x, const void *y)
{
    rotation *rx = (rotation *)x;
    rotation *ry = (rotation *)y;
    return strcmp(rx->suffix, ry->suffix);
}

// Computes Suffix array from the Input and returns it
int *computeSuffixArray(char *input_text, int len_text)
{
    // Array of rotations to store them
    rotation *suff = new rotation[len_text];

    // Save the Indices and values for every rotation
    for (int i = 0; i < len_text; i++)
    {
        suff[i].index = i;
        suff[i].suffix = (input_text + i);
    }

    // Sorting rotations
    qsort(suff, len_text, sizeof(rotation), cmpfunc);

    // Stores the indexes of sorted rotations
    int *suffix_arr = new int[len_text];
    for (int i = 0; i < len_text; i++)
        suffix_arr[i] = suff[i].index;

    return suffix_arr;
}

// Computes Burrows Wheeler from Suffix array
char *BWTfromSuffix(char *input_text, int *suffix_arr, int n)
{
    //Find Last char of every rotation
    char *bwt_arr = new char[n];
    int i;
    for (i = 0; i < n; i++)
    {
        // Computes the last char which is given by [(suffix_arr[i] + n - 1) % n]
        int j = suffix_arr[i] - 1;
        if (j < 0)
            j = j + n;

        bwt_arr[i] = input_text[j];
    }

    bwt_arr[i] = '\0';
    return bwt_arr;
}


int BWTenc(std::string inputFileName, std::string outputFileName)
{
    ReadFile(inputFileName);
    int len_text = InputString.size();

    // Computes the suffix array of our text
    int *suffix_arr = computeSuffixArray(&InputString[0], len_text);

    // Takes the Last char from every rotation to Create BWT
    char *bwt_arr = BWTfromSuffix(&InputString[0], suffix_arr, len_text);

    encodedOut = bwt_arr;
    WriteFile(outputFileName);

    return 0;
}
