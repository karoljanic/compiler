CXX := clang++
CXXFLAGS := -Wall -Wextra -pedantic -std=c++17 -O3

all: vm compiler test

vm:
	$(MAKE) -C ../virtual-machine all

compiler:
	$(MAKE) -C ../ all

test:
	$(CXX) $(CXXFLAGS) main.cpp to-ast-conversion.cpp ../out/*.cpp -o ast-test -lgtest

clean:
	rm -f ast-test

cleanall:
	$(MAKE) -C ../virtual-machine clean
	$(MAKE) -C ../virtual-machine cleanall
	$(MAKE) -C ../ cleanall
