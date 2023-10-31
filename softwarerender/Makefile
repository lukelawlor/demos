CXX := g++
CXXFLAGS := -g
LDFLAGS := -lSDL2

flib: flib.o
	$(CXX) $(LDFLAGS) $< -o $@

flib.o: flib.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm *.o
	rm flib
