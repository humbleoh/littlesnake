.PHONY: all

all:
	g++ -Wall main.cpp snake.cpp `sdl2-config --cflags --libs`
