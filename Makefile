FF := flex
FFLAGS :=
BB := bison
BFLAGS :=
CXX := clang++
CXXFLAGS := -Wall -Wextra -pedantic -std=c++17 -O3


all: lexer parser compiler

lexer:
	$(FF) $(FFLAGS) -o out/lexer.cpp --header-file=out/lexer.hpp src/lexer.l

parser:
	$(BB) $(BFLAGS) -Wcounterexamples -o out/parser.cpp --defines=out/parser.hpp -v src/parser.y

compiler:
	$(CXX) $(CXXFLAGS) out/*.cpp src/*.cpp -o compiler

clean:
	rm -f out/* compiler