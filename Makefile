CXX=mpic++

CXXFLAGS=-Wall -std=c++11

TARGET=pi_calc

SOURCES=pi_calc.cpp

OBJECTS=$(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(TARGET) $(OBJECTS)

.PHONY: all clean
