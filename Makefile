
CC = g++
CFLAGS = -Wall
DEPENDENCIES = camera.o game.o main.o particles.o skier.o skybox.o terrain.o texture.o trees.o utility.o vectors.o
LIBS=-lGL -lGLU -lglut -lm


all: i3d

#compile & link
i3d: $(DEPENDENCIES)
	$(CC) $(CFLAGS) -dDEBUG $^ libSOIL.a -o i3d $(LIBS)

# compile file
%.o:%.c
	$(CC) $(CFLAGS) -c $^ $(LIBS)

# clean
clean:
	rm -f *.o *.gch i3d
