FF := flex
FFLAGS :=
BB := bison
BFLAGS :=
CXX := clang++
CXXFLAGS := -Wall -Wextra -pedantic -std=c++17 -O3 -g


all: clean lexer parser compiler

lexer:
	$(FF) $(FFLAGS) -o out/lexer.cpp --header-file=out/lexer.hpp src/core/lexer.l

parser:
	$(BB) $(BFLAGS) -Wcounterexamples -o out/parser.cpp --defines=out/parser.hpp -v src/core/parser.y

compiler:
	$(CXX) $(CXXFLAGS) src/core/*.cpp src/ast/*.cpp src/bblocks/*.cpp out/*.cpp src/main.cpp -o compiler

clean:
	rm -f out/*
	rm -f compiler
