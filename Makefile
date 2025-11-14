CFLAGS = -Wall -Wpedantic
LDFLAGS = -lglfw -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo


# Make commands
all: game

game: main.o window.o textures.o blocks.o cJSON.o
	gcc $(CFLAGS) main.o window.o textures.o blocks.o cJSON.o -o game $(LDFLAGS)

check_textures: textures.o check_textures.o
	gcc $(CFLAGS) textures.o check_textures.o -o check_textures

check_blocks: blocks.o check_blocks.o textures.o cJSON.o
	gcc $(CFLAGS) blocks.o check_blocks.o textures.o cJSON.o -o check_blocks

# files to make
main.o: src/main.c
	echo "Compiling main file"
	gcc $(CFLAGS) -c src/main.c

window.o: src/window.c
	echo "Compiling window file"
	gcc $(CFLAGS) -c src/window.c

textures.o: src/textures.c
	echo "Compiling textures file"
	gcc $(CFLAGS) -c src/textures.c

blocks.o: src/blocks.c
	echo "Compiling blocks file"
	gcc $(CFLAGS) -c src/blocks.c

cJSON.o: src/cJSON.c
	echo "Compiling cJSON file"
	gcc $(CFLAGS) -c src/cJSON.c

# check files:
check_textures.o: tests/check_textures.c
	echo "Compiling check_textures file"
	gcc $(CFLAGS) -c tests/check_textures.c

check_blocks.o: tests/check_blocks.c
	echo "Compiling check_blocks file"
	gcc $(CFLAGS) -c tests/check_blocks.c

clean:
	echo "removing all .o files and game"
	rm -f *.o game check_textures check_blocks
