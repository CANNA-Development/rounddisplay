CC=g++ -D ASIM=1 -c -Wall -Wextra -I. -I./arduino/src 
all: bin/asim

bin/%.o: ./arduino/src/%.cpp
	$(CC) -o $@ $^

bin/%.o: ./%.cpp
	$(CC) -o $@ $^


bin/asim: bin/AsimDriver.o bin/font16.o bin/font24.o bin/Paint.o bin/sketch.o bin/main.o
	g++ -o $@ $^ -lraylib

run : bin/asim
	./bin/asim

clean:
	rm -f bin/*.o
	rm -f bin/asim	