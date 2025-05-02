INCLUDE := -I./include -I./extern
LIBS := -lraylib -lm -ldl -lpthread -lGL -lrt -lX11

CFLAGS := -O0 $(INCLUDE)
LDFLAGS := $(INCLUDE) $(LIBS)

obj/main.o: src/main.c
	gcc -c src/main.c -o obj/main.o $(CFLAGS)

obj/movement.o: src/movement.c
	gcc -c src/movement.c -o obj/movement.o $(CFLAGS)

obj/map.o: src/map.c
	gcc -c src/map.c -o obj/map.o $(CFLAGS)

obj/main.s: src/main.c
	gcc -S src/main.c -o obj/main.s $(CFLAGS)

obj/movement.s: src/movement.c
	gcc -S src/movement.c -o obj/movement.s $(CFLAGS)

obj/map.s: src/map.c
	gcc -S src/map.c -o obj/map.s $(CFLAGS)

obj/all.s: obj/main.s obj/movement.s obj/map.s
	cat obj/main.s obj/movement.s obj/map.s > obj/all.s

game: obj/main.o obj/movement.o obj/map.o
	gcc obj/main.o obj/movement.o obj/map.o -o game $(LDFLAGS) $(CFLAGS)
