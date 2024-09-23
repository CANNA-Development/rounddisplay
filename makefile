CC=g++ -D ASIM=1 -c -Wall -Wextra -I. -I./arduino/src 
all: bin/asim

bin/%.o: ./arduino/src/%.cpp
	$(CC) -o $@ $^

bin/%.o: ./%.cpp
	$(CC) -o $@ $^


bin/asim: bin/font8.o bin/font16.o bin/font20.o bin/font24.o bin/GUI_Paint.o bin/sketch.o bin/image_70x70.o bin/LCD_Driver.o bin/main.o
	g++ -o $@ $^ -lraylib

run : bin/asim
	./bin/asim

clean:
	rm -f bin/*.o
	rm -f bin/asim	