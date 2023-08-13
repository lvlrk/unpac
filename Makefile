CXX=g++
CXXFLAGS=-O2
LDFLAGS=
SRC=src/util.cpp src/vcra.cpp src/ap.cpp src/app.cpp \
src/archive.cpp src/error.cpp src/lzss.cpp src/main.cpp
TARGET=unpac

ifeq ($(OS),Windows_NT)
TARGET=unpac.exe
CXXFLAGS+=-static-libstdc++ -Ilib/fmt/include
endif

all: $(TARGET)

build: $(SRC)
	$(CXX) $(CXXFLAGS) $^ -c

$(TARGET): build
	$(CXX) $(LDFLAGS) *.o -o $(TARGET)

install: all
	cp $(TARGET) /bin

uninstall: all
	rm -f /bin/$(TARGET)

clean:
	rm -f *.o $(TARGET)

.PHONY: clean all install uninstall
