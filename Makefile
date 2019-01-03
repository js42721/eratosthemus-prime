CC     = gcc
CFLAGS = -O3
LDLIBS = -lm
OBJS   = magic.o main.o prime.o sieve.o utils.o wheel.o
TARGET = primecounter

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

.PHONY: clean
clean:
	@rm -f $(OBJS) $(TARGET)
