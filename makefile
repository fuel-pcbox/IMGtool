CXX = g++
CXXFLAGS = -std=c++11 
prefix=/usr

all:
	$(CXX) $(CXXFLAGS) imgtool.cpp -o imgtool

install: all
	install -m 755 imgtool $(prefix)/bin

.PHONY: install