CC=g++
CFLAGS=-g -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC

LIBNAME=libk3d.a

all: $(LIBNAME)

OBJS=mat4.o model.o vec3.o vec2.o gl.o

libk3d.a: $(OBJS)
	ar -cvq $(LIBNAME) $(OBJS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf $(LIBNAME) $(OBJS)
