#######################
#  cppcraft makefile  #
#######################

# build options
# -Ofast -msse4.1 -ffast-math -mfpmath=both -march=native -flto -fwhole-program
# -Ofast -msse4.1 -ffast-math -mfpmath=both -march=native
BUILDOPT = -ggdb3
# output file
OUTPUT   = ./Debug/cppcraft

# code folder
SOURCE = source

# resource file
ifeq ($(OS),Windows_NT)
RESOURCES = res/cppcraft.rc
endif

##############################################################

# compiler
CC = g++ $(BUILDOPT) -std=c++11
# compiler flags
CCFLAGS = -c -Wall -Wno-write-strings -Iinc
# linker flags
ifeq ($(OS),Windows_NT)
	LFLAGS  = -Llib -static -lpthread -lbassdll -llattice -lglfw3 -lgdi32 -lopengl32 -llzo2 -lws2_32 -ltcc
else
	LFLAGS  = -Llib -llibrary -lpthread -lbass -llzo2 -llattice -lGL -lglfw3 -lX11 -lXxf86vm -lXrandr -lXi -ltcc -ldl -Wl,-rpath,../lib
endif
# resource builder
RES = windres
# resource builder flags
RFLAGS = -O coff

##############################################################

# make pipeline
DIRECTORIES = $(SOURCE)
CCDIRS  = $(foreach dir, $(DIRECTORIES), $(dir)/*.c)
CCMODS  = $(wildcard $(CCDIRS))
CXXDIRS = $(foreach dir, $(DIRECTORIES), $(dir)/*.cpp)
CXXMODS = $(wildcard $(CXXDIRS))

# compile each .c to .o
.c.o:
	$(CC) $(CCFLAGS) $< -o $@

# compile each .cpp to .o
.cpp.o:
	$(CC) $(CCFLAGS) $< -o $@

# recipe for building .o from .rc files
%.o : %.rc
	$(RES) $(RFLAGS) $< -o $@

# convert .c to .o
CCOBJS  = $(CCMODS:.c=.o)
# convert .cpp to .o
CXXOBJS = $(CXXMODS:.cpp=.o)
# resource .rc to .o
CCRES   = $(RESOURCES:.rc=.o)

# link all OBJS using CC and link with LFLAGS, then output to OUTPUT
all: $(CXXOBJS) $(CCOBJS) $(CCRES)
	$(CC) $(CXXOBJS) $(CCOBJS) $(CCRES) $(LFLAGS) -o $(OUTPUT)

# remove each known .o file, and output
clean:
	$(RM) $(CXXOBJS) $(CCOBJS) $(CCRES) *~ $(OUTPUT).*
