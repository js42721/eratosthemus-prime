CC     = gcc
CFLAGS = -O3
LDLIBS = -lm
OBJS   = magic.o main.o prime.o sieve.o utils.o wheel.o
TARGET = primecounter

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDLIBS)

clean:
	@rm -f $(OBJS) $(TARGET)
