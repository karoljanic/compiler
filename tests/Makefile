CXX := clang++
CXXFLAGS := -Wall -Wextra -pedantic -std=c++17 -O3

all: vm compiler

vm:
	$(MAKE) -C ../virtual-machine all

compiler:
	$(MAKE) -C ../ all

clean:
	$(MAKE) -C ../virtual-machine clean
	$(MAKE) -C ../virtual-machine cleanall
	$(MAKE) -C ../ clean
	rm -f compiler
