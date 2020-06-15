TARGET    = ducksoup
CPP       = g++
WALL      = -Wall
WPEDANTIC = -Wpedantic
OPTIM     = -O3
BIN       = -o
INCDIR    = -I.
OBJ       = -c
AFLAGS    = $(WALL) $(WPEDANTIC) $(OPTIM) $(BIN)
OFLAGS    = $(WALL) $(WPEDANTIC) $(OPTIM) $(INCDIR) $(OBJ)
LINK      = -lm

all: main.o
	$(CPP) build/main.o $(AFLAGS) $(TARGET) $(LINK)
	mv $(TARGET) ./build

main.o: src/main.cpp directories
	$(CPP) $(OFLAGS) src/main.cpp
	mv main.o ./build/

.PHONY: clean
clean:
	rm -rf build/*

.PHONY: run
run:
	./build/$(TARGET)

.PHONY: leak-test
leak-test:
	valgrind --leak-check=full --track-origins=yes ./build/$(TARGET)

.PHONY: directories
directories:
	mkdir -p build