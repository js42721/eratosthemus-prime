CFLAGS = -O3
LDLIBS = -lm
TARGET = primecounter

$(TARGET):

clean:
	@rm -f $(TARGET)
