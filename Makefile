CXX=g++
CXXFLAGS=-O2 -Ilib/fmt/include -Ilib/raylib/src -Ilib/raygui/src
LDFLAGS=-lfmt -lraylib
SRC=src/util.cpp src/vcra.cpp src/ap.cpp src/app.cpp \
src/archive.cpp src/error.cpp src/lzss.cpp src/main.cpp \
src/guiapp.cpp
TARGET=unpac

ifeq ($(OS),Windows_NT)
TARGET+=.exe
CXXFLAGS+=-static-libstdc++
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
