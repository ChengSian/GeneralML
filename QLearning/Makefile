IDIR     = .
CXX      = g++
CXXFLAGS = -I$(IDIR) --std=c++11

ODIR     = test

_DEPS    = QLearn.hpp
DEPS     = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ     = Memory.o RewardLambda.o RewardTable.o main.o
OBJ      = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: test.out

test:
	./test.out

$(ODIR)/%.o: %.c $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

test.out: $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS)

.PHONY: clean test

clean:
	rm -f $(ODIR)/*.o *~ $(INCDIR)/*~ test.out