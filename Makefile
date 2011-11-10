CC=g++
CFLAGS=-g

LIBNAME=libk3d.a

all: $(LIBNAME)

OBJS=mat4.o model.o vec3.o

libk3d.a: $(OBJS)
	ar -cvq $(LIBNAME) $(OBJS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf $(LIBNAME) $(OBJS)
