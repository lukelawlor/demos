.DELETE_ON_ERROR:
.PHONY: clean
all: demo
demo: codingclub_demo.cpp
	g++ codingclub_demo.cpp -o demo -std=c++20 -lm -lSDL2 -lSDL2_ttf -O2
clean:
	rm demo
