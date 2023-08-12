CXX=g++
CXXFLAGS=-O2
LDFLAGS=
SRC=src/*.cpp
TARGET=unpac

all: $(TARGET)

build: $(SRC)
	$(CXX) $(CXXFLAGS) $^ -c

$(TARGET): build
	$(CXX) $(LDFLAGS) *.o -o $(TARGET)

clean:
	rm -f *.o $(TARGET)

.PHONY: clean all
