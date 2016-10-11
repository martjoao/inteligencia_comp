CC = g++
CC_FLAGS = -I include/ -std=c++11

OUTPUT = run

all: 
	$(CC) src/*.cpp -o $(OUTPUT) $(CC_FLAGS)
