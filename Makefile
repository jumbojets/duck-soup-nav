TARGET    = ducksoup
CPP       = g++
STD       = -std=c++17
WALL      = -Wall
WPEDANTIC = -Wpedantic
OPTIM     = -O3
BIN       = -o
INCDIR    = -I
OBJ       = -c
AFLAGS    = $(WALL) $(WPEDANTIC) ${STD} $(OPTIM) $(BIN)
OFLAGS    = $(WALL) $(WPEDANTIC) ${STD} $(OPTIM) $(OBJ)
LINK      = -lm

all: main.o RoadNet.o
	$(CPP) build/main.o build/RoadNet.o $(AFLAGS) $(TARGET) $(LINK)
	mv $(TARGET) ./build

main.o: src/main.cpp directories
	$(CPP) $(OFLAGS) $(INCDIR) include/$(TARGET) src/main.cpp
	mv main.o ./build/

RoadNet.o: src/RoadNet.cpp directories
	$(CPP) $(OFLAGS) $(INCDIR) include/$(TARGET) src/RoadNet.cpp
	mv RoadNet.o ./build/

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