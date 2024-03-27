commondir = common/
SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:.cpp=.o)
TARGET = Fractals

all: $(TARGET)

$(TARGET): $(OBJS)
	gcc -Wall -ggdb -o $@ $^ $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -I$(commondir) -I$(commondir)Linux -DGL_GLEXT_PROTOTYPES -lXt -lX11 -lGL -lm -lstdc++

%.o: %.cpp
	gcc -c -Wall -ggdb -o $@ $< -I$(commondir) -I$(commondir)Linux -I../common/Linux -DGL_GLEXT_PROTOTYPES

clean:
	rm -f $(OBJS) $(TARGET)

