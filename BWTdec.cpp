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
const std::string inputFileName = "bwtout.txt";
const std::string outputFileName = "bwtdecoded.txt";
std::string InputString;
std::string decodedout;

//Read file

void ReadFile()
{
    // read file and put it to string
    std::ifstream inputStream(inputFileName);
    std::stringstream sstream;
    sstream << inputStream.rdbuf();
    InputString = sstream.str();
    size_t dataSize = InputString.size();
    std::cout << dataSize << std::endl;

}

void WriteFile()
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
	int i,len_bwt = bwt_arr.size(); 
	std::string sorted_bwt = bwt_arr;
	int* l_shift = new int[len_bwt]; 

	// Index at which original string appears 
	int x = bwt_arr.find( (char)2 ); 
    if( x == std::string::npos) std::cout<<"ERROR: char not found";

	// Sorts the characters of bwt_arr[] alphabetically 
	std::sort(sorted_bwt.begin(), sorted_bwt.end()); 
    std::cout<<"\n Sorted \n";

    // Vector of Lists for every Character
	std::vector< std::list<int> > arr(128);

    // Adds the indices of every character to its list in arr ascendingly
	for (i = 0; i < len_bwt; i++) { 
	    arr[ bwt_arr[i] ].push_back(i);
	} 
    std::cout<<"\n Added All to last \n";

    // takes the shifted version of each character
	for (i = 0; i < len_bwt; i++) {
        l_shift[i] = arr[ sorted_bwt[i] ].front();   
        arr[ sorted_bwt[i] ].pop_front();   
    }
    std::cout<<"\n Shifted All characters \n";

    // set size of string
    decodedout.reserve(len_bwt);
	// Decodes the bwt 
	for (i = 0; i < len_bwt; i++) { 
		x = l_shift[x]; 
		decodedout += bwt_arr[x];
	} 
    // remove last character
    decodedout.pop_back();
} 

// Driver program to test functions above 
int main() 
{ 
    ReadFile();
	invert(&InputString[0]); 
    WriteFile();
	return 0; 
} 
