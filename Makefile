CC = g++
CC_FLAGS = -I include/ -std=c++11 -g

OUTPUT = run
OUTPUT2 = run2

1: 
	$(CC) src/*.cpp -o $(OUTPUT) $(CC_FLAGS)

2:
	$(CC) src2/*.cpp -o $(OUTPUT2) $(CC_FLAGS)
