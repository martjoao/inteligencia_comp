CC = g++
CC_FLAGS = -I include/

OUTPUT = run

all: 
	$(CC) src/*.cpp -o $(OUTPUT) $(CC_FLAGS)
