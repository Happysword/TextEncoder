
all: bwtdecode bwtencode

lzwencode: enclzw.cpp
	g++ -std=c++17 -O3 enclzw.cpp -o lzwencode

lzwdecode: declzw.cpp
	g++ -std=c++17 -O3 declzw.cpp -o lzwdecode

bwtencode: bwtencode.cpp
	g++ -std=c++17 -O3 bwtencode.cpp -o bwtencode

bwtdecode: BWTdec.cpp
	g++ -std=c++17 -O3 BWTdec.cpp -o bwtdecode
