CC = g++

OUTPUT = run

all: 
	$(CC) src/*.cpp -o $(OUTPUT)
