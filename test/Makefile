# compiler and flags
CC = gcc
CFLAG = -O0 -g -Wall -Wextra
TARGET = out

# source and header files 1_malloc.c 2_multidim_arr.c 3_UAF.c 4_ptr_arith.c 5_metadata.c 6_reassign.c
SRCS = 1_malloc.c # change this to your test file name, goto this directory, run make
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
