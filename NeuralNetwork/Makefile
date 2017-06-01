IDIR     = .
CXX      = g++
CXXFLAGS = -I$(IDIR) --std=c++11

ODIR     = test

_DEPS    = NN/ANN.o
DEPS     = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ     = ../NN/ANN.o Memory.o main.o
OBJ      = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: test.out

$(ODIR)/%.o: %.c $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

test.out: $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS)

.PHONY: clean test

clean:
	rm -f $(ODIR)/*.o *~ $(INCDIR)/*~ test.out

test:
	./test.out