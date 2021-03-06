#include <algorithm>
#include <array>
#include <climits>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <ios>
#include <iostream>
#include <istream>
#include <limits>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>


using CodeType = std::uint32_t;

/// Dictionary Maximum Size
// Used as a BreakPoint instead of NULL for BST
const CodeType DictSizemax = 13000 * 1024;

// character to control decoding
const CodeType Eof_enc = 1u << CHAR_BIT; ///< End-of-file.

// Encoder Class
class EncoderDictionary
{
    // Node for BST
    struct Node
    {
        explicit Node(char c) : first(DictSizemax), c(c), left(DictSizemax), right(DictSizemax) {}

        CodeType first; // Code of first child string.
        char c;
        CodeType left;  // Code of child node with byte < c.
        CodeType right; // Code of child node with byte > c.
    };

public:
    // Fill the Dictionary with all possible chars
    EncoderDictionary()
    {
        const int minc = std::numeric_limits<char>::min();
        const int maxc = std::numeric_limits<char>::max();
        CodeType k{0};

        vn.reserve(DictSizemax);
        vn.clear();

        for (int c = minc; c <= maxc; ++c) {
            initials[static_cast<unsigned char>(c)] = k++;
            vn.push_back(Node(c));
        }

        // add node for metacode
        vn.push_back(Node('\x00'));
    }

    // Searches for a pair (i, c) and inserts the pair if it wasn't found.
    // if found returns index , else return DictSizemax
    CodeType search_and_insert(CodeType i, char c)
    {
        if (i == DictSizemax)
            return search_initials(c);

        const CodeType vn_size = vn.size();
        CodeType ci{vn[i].first}; // Current Index

        // Searching the BST 
        if (ci != DictSizemax)
        {
            while (true)
                if (c < vn[ci].c)
                {
                    if (vn[ci].left == DictSizemax)
                    {
                        vn[ci].left = vn_size;
                        break;
                    }
                    else
                        ci = vn[ci].left;
                }
                else if (c > vn[ci].c)
                {
                    if (vn[ci].right == DictSizemax)
                    {
                        vn[ci].right = vn_size;
                        break;
                    }
                    else
                        ci = vn[ci].right;
                }
                else // c == vn[ci].c
                    return ci;
        }
        else
            vn[i].first = vn_size;

        vn.push_back(Node(c));
        return DictSizemax;
    }

    // Returns Code of char c from the initial one-byte part of the dictionary
    CodeType search_initials(char c) const
    {
        return initials[static_cast<unsigned char>(c)];
    }

    // Return size of Dictionary
    std::vector<Node>::size_type size() const
    {
        return vn.size();
    }

private:
    // Vector of nodes for the BST.
    std::vector<Node> vn;

    // maps one-byte strings to their codes.
    std::array<CodeType, 1u << CHAR_BIT> initials;
};

// helper class to store leftover bits
struct ByteCache_enc
{
    ByteCache_enc() : used(0), data(0x00) {}

    std::size_t used;   ///< Bits currently in use.
    unsigned char data; ///< The bits of the cached byte.
};

// class to write bits to file
class CodeWriter
{
public:
    explicit CodeWriter(std::ostream &os) : os(os), bits(CHAR_BIT + 1)
    {
    }

    // Destructor to write the leftover bits when finished
    ~CodeWriter()
    {
        write(static_cast<CodeType>(Eof_enc));

        // write the incomplete leftover byte
        if (lo.used != 0)
            os.put(static_cast<char>(lo.data));
    }

    // getter for number of bits
    std::size_t get_bits() const
    {
        return bits;
    }

    // increases bits number
    void increase_bits()
    {
        ++bits;
    }

    // Writes the code k with the current bit width.
    bool write(CodeType k)
    {
        std::size_t remaining_bits{bits};

        if (lo.used != 0)
        {
            lo.data |= k << lo.used;
            os.put(static_cast<char>(lo.data));
            k >>= CHAR_BIT - lo.used;
            remaining_bits -= CHAR_BIT - lo.used;
            lo.used = 0;
            lo.data = 0x00;
        }

        while (remaining_bits != 0)
            if (remaining_bits >= CHAR_BIT)
            {
                os.put(static_cast<char>(k));
                k >>= CHAR_BIT;
                remaining_bits -= CHAR_BIT;
            }
            else
            {
                lo.used = remaining_bits;
                lo.data = k;
                break;
            }

        return true;
    }

private:
    ByteCache_enc lo;     ///< LeftOvers.
    std::ostream &os; ///< Output Stream.
    std::size_t bits; ///< Binary width of codes.
};

// Computes the number of bits required to store n.
std::size_t required_bits_enc(unsigned long int n)
{
    std::size_t r{1};

    while ((n >>= 1) != 0)
        r++;

    return r;
}

//Compresses the contents of "is" and writes the result to "os".
void compress(std::istream &is, std::ostream &os)
{
    EncoderDictionary ed;
    CodeWriter cw(os);
    CodeType i{DictSizemax}; // Index
    char c;

    while (is.get(c))
    {
        const CodeType temp{i};

        if ((i = ed.search_and_insert(temp, c)) == DictSizemax)
        {
            cw.write(temp);
            i = ed.search_initials(c);

            if (required_bits_enc(ed.size() - 1) > cw.get_bits())
                cw.increase_bits();
        }

    }

    if (i != DictSizemax)
        cw.write(i);
}

int LZWenc(std::string inputFileName, std::string outputFileName)
{
    std::ifstream input_file;
    std::ofstream output_file;

    input_file.open(inputFileName, std::ios_base::binary);
    output_file.open(outputFileName, std::ios_base::binary);

    // main compress function
    compress(input_file, output_file);
    return EXIT_SUCCESS;
}