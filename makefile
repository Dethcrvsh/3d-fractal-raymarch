commondir = common/
guidir = SimpleGUI/
SRCS := $(wildcard src/*.cpp)
TARGET = fractals
VERSION = c++17

all: $(TARGET)

$(TARGET): $(SRCS)
	g++ -std=$(VERSION) -Wall -ggdb -o $@ $^ $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c $(guidir)SimpleGUI.c -I$(guidir) -I$(commondir) -I$(commondir)Linux -DGL_GLEXT_PROTOTYPES -lXt -lX11 -lGL -lm -lstdc++

clean:
	rm -f $(TARGET)

