INCLUDE := -I./include -I./extern
LIBS := -lraylib -lm -ldl -lpthread -lGL -lrt -lX11

obj/main.o: src/main.c
	gcc -c src/main.c -o obj/main.o $(INCLUDE)

game: obj/main.o
	gcc obj/main.o -o game $(INCLUDE) $(LIBS)
