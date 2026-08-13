# compiler and flags
CC = gcc
CFLAG = -O2 -g -Wall -Wextra -std=gnu99
TARGET = out

# source and header files
# 1_malloc.c 2_multidim_arr.c 3_UAF.c 4_ptr_arith.c 5_metadata.c
# 6_reassign.c 7_hardening.c 8_multithread.c 9_arena.c 10_mmap.c
# 11_realloc.c 12_alias.c 13_alloca.c 14_globals.c 15_non_arrays.c
SRCS = 2_multidim_arr.c # change this to your test file name, goto examples folder, run make
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
