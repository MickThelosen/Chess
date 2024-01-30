CXX = g++
CXXFLAGS = -std=c++17 -Wall
INCLUDES = -Iinclude -Iinclude/headers -Iinclude/SFML
LDFLAGS = -Linclude/lib
LIBS = -lsfml-graphics -lsfml-window -lsfml-system

SRCDIR = src
OBJDIR = obj
BINDIR = bin

SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))
EXECUTABLE = $(BINDIR)/Chess

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS) $(LIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

run: $(EXECUTABLE)
	./$(EXECUTABLE)

clean:
	del /Q $(BINDIR)\Chess.exe $(OBJDIR)\*.o
