CXX = g++
CXXFLAGS = -lX11
SOURCES = main.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: graph

graph: $(OBJECTS)
	$(CXX) -o $@ $^ $(CXXFLAGS)

%.o: %.cpp
	$(CXX) -c $< $(CXXFLAGS)

clean:
	rm -f $(OBJECTS) graph