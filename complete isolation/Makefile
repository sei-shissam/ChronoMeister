CC=gcc
CFLAGS=-I.
LIBS=-lm
DEPS = completeisolation.c
OBJ = completeisolation.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

completeisolation: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

