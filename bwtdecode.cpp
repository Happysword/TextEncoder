// C program to find inverse of Burrows 
// Wheeler transform
#include <string.h> 
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>

// Parameters
const std::string inputFileName = "enwik8";
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


// Structure to store info of a node of 
// linked list 
struct node { 
	int data; 
	struct node* next; 
}; 

// Creates the new node 
struct node* getNode(int i) 
{ 
	struct node* nn = 
		(struct node*)malloc(sizeof(struct node)); 
	nn->data = i; 
	nn->next = NULL; 
	return nn; 
} 

// Does insertion at end in the linked list 
void addAtLast(struct node** head, struct node* nn) 
{ 
	if (*head == NULL) { 
		*head = nn; 
		return; 
	} 
	struct node* temp = *head; 
	while (temp->next != NULL) 
		temp = temp->next; 
	temp->next = nn; 
} 

// Computes l_shift[] 
void computeLShift(struct node** head, int index, 
					int* l_shift) 
{ 
	l_shift[index] = (*head)->data; 
	(*head) = (*head)->next; 
} 

void invert(char bwt_arr[]) 
{ 
	int i,len_bwt = strlen(bwt_arr); 
	char* sorted_bwt = (char*)malloc(len_bwt * sizeof(char)); 
	strcpy(sorted_bwt, bwt_arr); 
	int* l_shift = (int*)malloc(len_bwt * sizeof(int)); 

	// Index at which original string appears 
	// in the sorted rotations list 
    std::string BWT(bwt_arr);
	int x = BWT.find( (char)2 ); 
    if( x == std::string::npos) std::cout<<"ERROR: char not found";

	// Sorts the characters of bwt_arr[] alphabetically 
	std::sort(sorted_bwt, sorted_bwt + len_bwt); 

    std::cout<<"\n Sorted \n";

	// Array of pointers that act as head nodes 
	// to linked lists created to compute l_shift[] 
	struct node* arr[128] = { NULL }; 

	// Takes each distinct character of bwt_arr[] as head 
	// of a linked list and appends to it the new node 
	// whose data part contains index at which 
	// character occurs in bwt_arr[] 
	for (i = 0; i < len_bwt; i++) { 
		struct node* nn = getNode(i); 
		addAtLast(&arr[bwt_arr[i]], nn); 
	} 

    std::cout<<"\n Added All to last \n";

	// Takes each distinct character of sorted_arr[] as head 
	// of a linked list and finds l_shift[] 
	for (i = 0; i < len_bwt; i++) 
		computeLShift(&arr[sorted_bwt[i]], i, l_shift); 

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
