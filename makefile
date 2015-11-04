CC=g++
CFLAGS=-std=c++0x -O2 -pipe -m64 -c
OBJECTS=neuron.o network.o main.o

all: vision_bc.exe


vision_bc.exe : $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $<

clean:
	rm vision_bc.exe $(OBJECTS)

