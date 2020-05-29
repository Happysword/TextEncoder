
all: lzwencode lzwdecode

lzwencode: enclzw.cpp
	g++ -std=c++17 -O3 enclzw.cpp -o lzwencode

lzwdecode: declzw.cpp
	g++ -std=c++17 -O3 declzw.cpp -o lzwdecode

bwtencode: BWTenc.cpp
	g++ -std=c++17 -O3 BWTenc.cpp -o bwtencode

bwtdecode: BWTdec.cpp
	g++ -std=c++17 -O3 BWTdec.cpp -o bwtdecode