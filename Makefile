CC = clang++
CFLAGS = -march=native

default: build run clean

all: build run clean build-release install

build:
	mkdir -p ./bin/
	$(CC) ${CFLAGS} ./src/*.cpp -o ./bin/mcserversoftware

run:
	clear
	./bin/mcserversoftware

clean:
	rm -r ./bin/

build-release:
	mkdir -p ./bin/
	$(CC) ${CFLAGS} ./src/*.cpp -Ofast -o ./bin/mcserversoftware

install:
	mkdir -p ./bin/
	$(CC) ${CFLAGS} ./src/*.cpp -Ofast -o ./bin/mcserversoftware
	sudo mv ./bin/mcserversoftware /bin/mcserversoftware