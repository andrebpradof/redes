IDIR =../include
CC=g++
CFLAGS=-I$(IDIR) -Wall

ODIR=obj
LIBS=-lm

_DEPS = meioDeComunicacao.hpp enlace.hpp wrapper.hpp
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = simulador.o meioDeComunicacao.o enlace.o wrapper.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -std=c++11 -c -o $@ $< $(CFLAGS)

simulador: $(OBJ)
	$(CC) -std=c++11 -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ simulador *~
