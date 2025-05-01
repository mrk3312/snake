INCLUDE := -I./include -I./extern
LIBS := -lraylib -lm -ldl -lpthread -lGL -lrt -lX11

CFLAGS := -O0 $(INCLUDE)
LDFLAGS := $(INCLUDE) $(LIBS)

obj/main.o: src/main.c
	gcc -c src/main.c -o obj/main.o $(CFLAGS)

obj/main.s: src/main.c
	gcc -S src/main.c -o obj/main.s $(CFLAGS)

game: obj/main.o
	gcc obj/main.o -o game $(LDFLAGS) $(CFLAGS)
