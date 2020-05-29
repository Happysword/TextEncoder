
all: main

main: LZWdec.cpp LZWenc.cpp BWTenc.cpp BWTdec.cpp Main.cpp
	g++ -std=c++17 -O3 Main.cpp -o TextEncoder