// C program to find Burrows Wheeler transform
// of a given text

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

// Parameters
const std::string inputFileName = "enwik8";
const std::string outputFileName = "bwtout.txt";
std::string InputString;
std::string encodedOut;

//Read file

void ReadFile()
{
    // read file and put it to string
    std::ifstream inputStream(inputFileName);
    std::stringstream sstream;
    sstream << inputStream.rdbuf();
    InputString = sstream.str();
    InputString += (char)2;
    size_t dataSize = InputString.size();
    std::cout << dataSize << std::endl;

}

void WriteFile()
{
    // write the vector output to file
    std::ofstream outStream(outputFileName, std::ofstream::out | std::ofstream::binary);
    long outSize = encodedOut.size();
    for (int i = 0; i < outSize; i++)
    {
        outStream.write((&encodedOut[i]), sizeof(char));
    }
}

// Structure to store data of a rotation
struct rotation
{
    int index;
    char *suffix;
};

// Compares the rotations and
// sorts the rotations alphabetically
int cmpfunc(const void *x, const void *y)
{
    struct rotation *rx = (struct rotation *)x;
    struct rotation *ry = (struct rotation *)y;
    return strcmp(rx->suffix, ry->suffix);
}

// Takes text to be transformed and its length as
// arguments and returns the corresponding suffix array
int *computeSuffixArray(char *input_text, int len_text)
{
    // Array of structures to store rotations and
    // their indexes
    struct rotation *suff = new struct rotation[len_text];

    // Structure is needed to maintain old indexes of
    // rotations after sorting them
    for (int i = 0; i < len_text; i++)
    {
        suff[i].index = i;
        suff[i].suffix = (input_text + i);
    }

    // Sorts rotations using comparison
    // function defined above
    qsort(suff, len_text, sizeof(struct rotation),
          cmpfunc);

    // Stores the indexes of sorted rotations
    int *suffix_arr = (int *)malloc(len_text * sizeof(int));
    for (int i = 0; i < len_text; i++)
        suffix_arr[i] = suff[i].index;

    // deallocate memory
    delete[] suff;
    // Returns the computed suffix array
    return suffix_arr;
}

// Takes suffix array and its size
// as arguments and returns the
// Burrows - Wheeler Transform of given text
char *findLastChar(char *input_text,
                   int *suffix_arr, int n)
{
    // Iterates over the suffix array to find
    // the last char of each cyclic rotation
    char *bwt_arr = (char *)malloc(n * sizeof(char));
    int i;
    for (i = 0; i < n; i++)
    {
        // Computes the last char which is given by
        // input_text[(suffix_arr[i] + n - 1) % n]
        int j = suffix_arr[i] - 1;
        if (j < 0)
            j = j + n;

        bwt_arr[i] = input_text[j];
    }

    bwt_arr[i] = '\0';

    // Returns the computed Burrows - Wheeler Transform
    return bwt_arr;
}

// Driver program to test functions above
int main()
{
    ReadFile();
    int len_text = InputString.size();

    // Computes the suffix array of our text
    int *suffix_arr = computeSuffixArray(&InputString[0], len_text);

    // Adds to the output array the last char
    // of each rotation
    char *bwt_arr = findLastChar(&InputString[0], suffix_arr, len_text);

    encodedOut = bwt_arr;
    WriteFile();

    return 0;
}
