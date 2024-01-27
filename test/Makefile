CXX := clang++
CXXFLAGS := -Wall -Wextra -pedantic -std=c++17 -O3 -g

all: vm compiler test-ast test-bblocks test-machine-code test-meaningful-programs

vm:
	$(MAKE) -C ../virtual-machine all

compiler:
	$(MAKE) -C ../ all

test-ast:
	$(CXX) $(CXXFLAGS) ../src/core/*.cpp ../src/ast/*.cpp ../src/bblocks/*.cpp ../out/*.cpp main.cpp to-ast-conversion.cpp -o ast-test -lgtest

test-bblocks:
	$(CXX) $(CXXFLAGS) ../src/core/*.cpp ../src/ast/*.cpp ../src/bblocks/*.cpp ../out/*.cpp main.cpp to-bblocks-conversion.cpp -o bblocks-test -lgtest

test-machine-code:
	$(CXX) $(CXXFLAGS) ../src/core/*.cpp ../src/ast/*.cpp ../src/bblocks/*.cpp ../out/*.cpp main.cpp to-machine-code-conversion.cpp -o machine-code-test -lgtest

test-meaningful-programs:
	$(CXX) $(CXXFLAGS) ../src/core/*.cpp ../src/ast/*.cpp ../src/bblocks/*.cpp ../out/*.cpp main.cpp to-meaningful-programs.cpp -o meaningful-programs-test -lgtest

clean:
	rm -f ast-test
	rm -f bblocks-test
	rm -f machine-code-test
	rm -f tmp.ast

cleanall:
	$(MAKE) -C ../virtual-machine clean
	$(MAKE) -C ../virtual-machine cleanall
	$(MAKE) -C ../ cleanall
