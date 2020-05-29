#include <algorithm>
#include <array>
#include <climits>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <exception>
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

// Parameters
const std::string inputFileName = "lzwenc.bin";
const std::string outputFileName = "lzwdecoded.txt";

/// Type used to store and retrieve codes.
using CodeType = std::uint32_t;

/// Dictionary Maximum Size
const CodeType maxDictSize = 13000 * 1024;

// character to control decoding
const CodeType Eof = 1u << CHAR_BIT; ///< End-of-file.

// helper class to store leftover bits
struct ByteCache
{
    ByteCache() : used(0), data(0x00) {}

    std::size_t used;   ///< Bits currently in use.
    unsigned char data; ///< The bits of the cached byte.
};

// Class to Read the variable width binary codes
class CodeReader
{
public:
    explicit CodeReader(std::istream &is) : is(is), bits(CHAR_BIT + 1) {}

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

    // Reads the code k with the current bit width.
    // returns false when Eof is found
    bool read(CodeType &k)
    {
        // bit masks for the reading of bits
        static const std::array<unsigned long int, 9> masks{
            {0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF}};

        std::size_t remaining_bits{bits};
        std::size_t offset{lo.used};
        unsigned char temp;

        k = lo.data;
        remaining_bits -= lo.used;
        lo.used = 0;
        lo.data = 0x00;

        while (remaining_bits != 0 && is.get(reinterpret_cast<char &>(temp)))
            if (remaining_bits >= CHAR_BIT)
            {
                k |= static_cast<CodeType>(temp) << offset;
                offset += CHAR_BIT;
                remaining_bits -= CHAR_BIT;
            }
            else
            {
                k |= static_cast<CodeType>(temp & masks[remaining_bits]) << offset;
                lo.used = CHAR_BIT - remaining_bits;
                lo.data = temp >> remaining_bits;
                break;
            }

        if (k == static_cast<CodeType>(Eof))
            return false;

        return true;
    }

private:
    ByteCache lo;     ///< LeftOvers.
    std::istream &is; ///< Input Stream.
    std::size_t bits; ///< Binary width of codes.
};

// Computes the number of bits required to store n.
std::size_t required_bits(unsigned long int n)
{
    std::size_t r{1};
    while ((n >>= 1) != 0)
        ++r;
    return r;
}

// Construct the String from the Dictionary and Code k
std::vector<char> * rebuild_string(CodeType k, std::vector<std::pair<CodeType, char>> &dictionary) {
    static std::vector<char> s; // String

    s.clear();

    // the length of a string cannot exceed the dictionary's number of entries
    s.reserve(maxDictSize);

    while (k != maxDictSize)
    {
        s.push_back(dictionary[k].second);
        k = dictionary[k].first;
    }

    std::reverse(s.begin(), s.end());
    return &s;
};

// Decompresses the contents of is and writes the result to os.
void decompress(std::istream &is, std::ostream &os)
{
    std::vector<std::pair<CodeType, char>> dictionary;

    CodeReader cr(is);
    CodeType i{maxDictSize}; // Index
    CodeType k;              // Key

    // Initializing Dictionary
    dictionary.clear();
    dictionary.reserve(maxDictSize);

    const long int minc = std::numeric_limits<char>::min();
    const long int maxc = std::numeric_limits<char>::max();

    for (long int c = minc; c <= maxc; ++c)
        dictionary.push_back({maxDictSize, static_cast<char>(c)});

    // add Eof code
    dictionary.push_back({0, '\x00'});

    // decoding Input Stream
    while (true)
    {
        if (required_bits(dictionary.size()) > cr.get_bits())
            cr.increase_bits();

        if (!cr.read(k))
            break;

        const std::vector<char> *s; // String

        if (k == dictionary.size())
        {
            dictionary.push_back({i, rebuild_string(i,dictionary)->front()});
            s = rebuild_string(k,dictionary);
        }
        else
        {
            s = rebuild_string(k,dictionary);

            if (i != maxDictSize)
                dictionary.push_back({i, s->front()});
        }

        os.write(&s->front(), s->size());
        i = k;
    }
}

int LZWdec()
{
    std::ifstream input_file;
    std::ofstream output_file;

    input_file.open(inputFileName, std::ios_base::binary);
    output_file.open(outputFileName, std::ios_base::binary);

    // main function
    decompress(input_file, output_file);

    return EXIT_SUCCESS;
}
