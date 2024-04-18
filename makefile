commondir = common/
guidir = SimpleGUI/
SRCS := $(wildcard src/*.cpp)
OBJS := $(SRCS:.cpp=.o)
TARGET = Fractals

all: $(TARGET)

$(TARGET): $(OBJS)
	gcc -Wall -ggdb -o $@ $^ $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c $(guidir)SimpleGUI.c -I$(guidir) -I$(commondir) -I$(commondir)Linux -DGL_GLEXT_PROTOTYPES -lXt -lX11 -lGL -lm -lstdc++

%.o: %.cpp
	gcc -c -Wall -ggdb -o $@ $< -I$(guidir) -I$(commondir) -I$(commondir)Linux -I../common/Linux -DGL_GLEXT_PROTOTYPES

clean:
	rm -f $(OBJS) $(TARGET)

