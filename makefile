#######################
#  cppcraft makefile  #
#######################

# code folders
SOURCE_DIR  = source
SOURCE_DIRS = . tests
LIBRARY_DIRS = library library/bitmap library/compression library/math     \
				library/network library/noise library/opengl library/sound \
				library/threading library/timing library/voxels
RESOURCES = res/cppcraft.rc

# build options
# -Ofast -msse4.1 -ffast-math
BUILDOPT = -Ofast -msse4.1 -ffast-math
# output file
OUTPUT   = ./Debug/cppcraft

##############################################################

# compiler
CC = g++ $(BUILDOPT) -std=c++11
# compiler flags
CCFLAGS = -c -Wall -Wno-write-strings -Iinc
# linker flags
LFLAGS  = -Llib -static -lbassdll -lglfw3 -lgdi32 -lopengl32 -llzo2 -lws2_32 -llattice
# resource builder
RES = windres
# resource builder flags
RFLAGS = -O coff

##############################################################

# make pipeline
DIRECTORIES = $(LIBRARY_DIRS) $(SOURCE_DIRS)
CCDIRS  = $(foreach dir, $(DIRECTORIES), $(SOURCE_DIR)/$(dir)/*.c)
CCMODS  = $(wildcard $(CCDIRS))
CXXDIRS = $(foreach dir, $(DIRECTORIES), $(SOURCE_DIR)/$(dir)/*.cpp)
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
