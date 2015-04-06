ifeq ($(shell uname -s), Darwin)
CXX = clang++
else
CXX = g++
endif

CXXFLAG = -O2

all: src/Main.cpp
	$(CXX) $(CXXFLAG) src/Main.cpp -o bin/Main

clean:
	rm -r bin/*
