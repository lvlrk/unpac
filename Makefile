INCLUDES=-Iinclude -Ilib/zlib/include
ERRORS=-Wno-narrowing -Wno-enum-compare
CXX=g++
CXXFLAGS=-std=c++2b $(INCLUDES) $(ERRORS)
LDFLAGS=-lraylib -lz
TARGET=unpac
LIBTARGET=lib$(TARGET).so

ifeq ($(OS),Windows_NT)
TARGET=unpac.exe
LIBTARGET=lib$(TARGET).dll
LDFLAGS+=-static-libstdc++
endif

all: $(TARGET)

$(TARGET):
	$(CXX) src/*.cpp $(CXXFLAGS) $(LDFLAGS) -o $(TARGET)

lib:
	$(CXX) src/ap.cpp src/archive.cpp src/estream.cpp src/g03.cpp src/lzss.cpp src/util.cpp src/vcra.cpp -Iinclude -Ilib/raygui/src -fPIC -c -std=c++2b
	make clean
	$(CXX) -shared -o $(LIBTARGET) *.o

install: $(TARGET)
ifeq ($(OS),Windows_NT)
	cp $(TARGET) C:\\Windows
else
	cp $(TARGET) /bin
endif

install-lib: lib$(TARGET)
ifeq ($(OS),Windows_NT)
	cp $(LIBTARGET) C:\\Windows
else
	cp $(LIBTARGET) /usr/lib

	rm -rf /usr/include/$(TARGET)
	mkdir /usr/include/$(TARGET)
	cp include/ap.h include/archive.h include/estream.h include/g03.h include/lzss.h include/museum.h include/types.h include/util.h include/vcra.h /usr/include/$(TARGET) -r
endif

uninstall:
ifeq ($(OS),Windows_NT)
	rm C:\\Windows\$(TARGET)
else
	rm -f /bin/$(TARGET)
endif

uninstall-lib:
ifeq ($(OS),Windows_NT)
	rm C:\\Windows\$(LIBTARGET)
else
	rm -f /usr/lib/$(LIBTARGET)
endif

clean:
	rm -f $(TARGET) *.o *.so *.dll

.PHONY: clean all install uninstall
