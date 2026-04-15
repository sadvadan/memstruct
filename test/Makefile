# compiler and flags
CC = gcc
CFLAG = -O0 -g -Wall -Wextra
TARGET = out

# source and header files
SRCS = 6_reassign.c
DEPS = ../src/mstrct.h

# default rule
all: $(TARGET)

# linking the executable
$(TARGET) : $(SRCS) $(DEPS)
	$(CC) $(CFLAG) $(SRCS) -o $(TARGET)

# clean up build files
clean:
	rm -f $(TARGET)

#run the program
run: all
	./$(TARGET)
