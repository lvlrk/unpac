CXX=g++
CXXFLAGS=-std=c++2b -Ilib/raygui/src -Ilib/raylib/src -Wno-narrowing -Wno-enum-compare
LDFLAGS=-lraylib
TARGET=unpac

ifeq ($(OS),Windows_NT)
TARGET=unpac.exe
LDFLAGS+=-static-libstdc++
endif

all: $(TARGET)

$(TARGET):
	$(CXX) src/*.cpp $(CXXFLAGS) $(LDFLAGS) -o $(TARGET) 

install: $(TARGET)
ifeq ($(OS),Windows_NT)
	cp $(TARGET) C:\\Windows
else
	cp $(TARGET) /bin
endif

uninstall: $(TARGET)
ifeq ($(OS),Windows_NT)
	rm -f C:\\Windows\$(TARGET)
else
	rm -f /bin/$(TARGET)
endif

clean:
	rm -f $(TARGET)

.PHONY: clean all install uninstall