CXX := clang++
CXXFLAGS := -Wall -Wextra -pedantic -std=c++17 -O3 -g

all: vm compiler test

vm:
	$(MAKE) -C ../virtual-machine all

compiler:
	$(MAKE) -C ../ all

test:
	$(CXX) $(CXXFLAGS) ../src/core/*.cpp ../src/ast/*.cpp ../src/bblocks/*.cpp ../out/*.cpp main.cpp to-ast-conversion.cpp -o ast-test -lgtest

clean:
	rm -f ast-test
	rm -f tmp.ast

cleanall:
	$(MAKE) -C ../virtual-machine clean
	$(MAKE) -C ../virtual-machine cleanall
	$(MAKE) -C ../ cleanall
