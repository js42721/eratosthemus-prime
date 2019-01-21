CC     = gcc
CFLAGS = -O2 -Wall -fopenmp
LDLIBS = -lm

TARG1  = eratosthemus-count
OBJS1  = count.o magic.o segment.o sieve.o utils.o wheel.o

TARG2  = eratosthemus-gen
OBJS2  = gen.o magic.o segment.o sieve.o utils.o wheel.o

TARGS  = $(TARG1) $(TARG2)
OBJS   = $(OBJS1) $(OBJS2)

.PHONY: all
all: $(TARGS)

$(TARG1): $(OBJS1)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

$(TARG2): $(OBJS2)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

.PHONY: clean
clean:
	@rm -f $(OBJS) $(TARGS)
