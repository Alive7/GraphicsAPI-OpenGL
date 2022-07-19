CXX=g++
STD=-std=c++17
WAR=-Wall -Wextra -pedantic
BUG=-g
OPT=
CXX_FLAGS=$(BUG) $(WAR) $(OPT) $(STD)
.PHONY: all clean

NSIM=NSim/
INCLUDE=Include/
LIBS=Libs/

TARGETS=OpenGL
OBJECTS=Source.o Camera.o Geometry.o Shader.o Texture.o glad.o

# A note on variables:
# $@: the target filename.
# $*: the target filename without the file extension.
# $<: the first prerequisite filename.
# $^: the filenames of all the prerequisites, separated by spaces, discard duplicates.
# $+: similar to $^, but includes duplicates.
# $?: the names of all prerequisites that are newer than the target, separated by spaces.

all: $(TARGETS)

%.o: %.cpp
	$(CXX) $(CXX_FLAGS) -I$(NSIM) -I$(INCLUDE) -L$(LIBS) -c $<

Source.o: Source.cpp
Camera.o: Camera.cpp Camera.h
Geometry.o: Geometry.cpp Geometry.h
Shader.o: Shader.cpp Shader.h
Texture.o: Texture.cpp Texture.h

glad.o: gladsrc/glad.c Include/glad/glad.h
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -L$(LIBS) -c $<

OpenGL: $(OBJECTS)
	$(CXX) $(CXX_FLAGS) -o $@ $^
	rm -f *.o

clean:
	rm -f *.o *~ $(TARGETS)
