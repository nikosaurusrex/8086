all: assembler disassembler

CXXFLAGS = -std=c++14 -D_CRT_SECURE_NO_WARNINGS

assembler: src/assembler.cpp src/common.h
	clang++ $(CXXFLAGS) -o assembler.exe src/assembler.cpp

disassembler: src/disassembler.cpp src/common.h
	clang++ $(CXXFLAGS) -o disassembler.exe src/disassembler.cpp
