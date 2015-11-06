CC=g++
CFLAGS=-std=c++0x -O2 -pipe -m64 -Wall -W -c
OBJECTS= error.o featureKeeper.o neuron.o network.o region.o pathway.o nfe_l.o vs_t0.o

all: vision_bc.exe


vision_bc.exe : $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $<

clean:
	rm vision_bc.exe $(OBJECTS)

