OUTPUT = generator.dll
CC = c:\mingw64\bin\g++ -std=c++11
CFLAGS = -std=c++11 -Igcc -Wall -Wno-write-strings -m32 -Ofast -msse4.1 -mfpmath=both -fpermissive -Wl,--add-stdcall-alias
LFLAGS = -std=c++11 -shared -Llib -Wall -Wno-write-strings -m32 -Ofast -msse4.1 -mfpmath=both -fpermissive -Wl,--add-stdcall-alias

DIRECTORIES = gcc/generators/flat gcc/generators/std gcc gcc/biome gcc/noise gcc/objects
CDIRS   = $(foreach dir, $(DIRECTORIES), $(dir)/*.c)
CMODS   = $(wildcard $(CDIRS))
COBJS   = $(CMODS:.c=.o)
CXXDIRS = $(foreach dir, $(DIRECTORIES), $(dir)/*.cpp)
CXXMODS = $(wildcard $(CXXDIRS))
CXXOBJS = $(CXXMODS:.cpp=.o)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

all: $(COBJS) $(CXXOBJS)
	$(CC) $(COBJS) $(CXXOBJS) $(LFLAGS) -o $(OUTPUT)

clean:
	$(RM) $(COBJS) $(CXXOBJS) $(OUTPUT)
