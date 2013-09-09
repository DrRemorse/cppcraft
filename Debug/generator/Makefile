OUTPUT = ../generator.dll
CC = c:\mingw64\bin\gcc
CFLAGS = -c -Iinc -Wall -Wno-write-strings -m32 -Ofast -msse4.1 -mfpmath=both
LFLAGS = -shared -Llib -Wall -Wno-write-strings -m32 -Ofast -msse4.1 -mfpmath=both

DIRECTORIES = ./generators/std . ./biome ./noise ./objects
CDIRS   = $(foreach dir, $(DIRECTORIES), $(dir)/*.c)
CMODS   = $(wildcard $(CDIRS))
COBJS   = $(CMODS:.c=.o)
CXXDIRS = $(foreach dir, $(DIRECTORIES), $(dir)/*.cpp)
CXXMODS = $(wildcard $(CXXDIRS))
CXXOBJS = $(CXXMODS:.cpp=.o)

.c.o:
	$(CC) -std=gnu99 $(CFLAGS) $< -o $@

.cpp.o:
	$(CC) -std=c++11 $(CFLAGS) $< -o $@

all: $(COBJS) $(CXXOBJS)
	$(CC) $(COBJS) $(CXXOBJS) $(LFLAGS) -o $(OUTPUT)

