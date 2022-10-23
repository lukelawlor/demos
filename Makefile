.DELETE_ON_ERROR:
.PHONY: clean
all: demo
demo:
	gcc codingclub_demo.c -o demo -lm -lSDL2 -lSDL2_ttf -O2
clean:
	rm demo
