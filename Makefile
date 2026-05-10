# Compile with gcc
CC = gcc

# Compiler options:
CFLAGS = -g

# Linker options:
LDFLAGS +=

# set the name of the executable file to compile here
PROGRAM = mvote

# Get a list of all .c source files 
SRC_FILES := $(wildcard *.c) $(wildcard commands/*.c) $(wildcard modules/*.c)

# Specify the corresponding object file directory structure
OBJ_DIRS := $(patsubst %.c,%.o,$(SRC_FILES))
OBJ_DIRS := $(addprefix obj/,$(OBJ_DIRS))

# Generate a list of corresponding .o object files
OBJS := $(SRC_FILES:.c=.o)
OBJS := $(addprefix obj/,$(OBJS))

# Create a rule to build object files
obj/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(PROGRAM): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(PROGRAM)

clean:
	rm -f $(PROGRAM) $(OBJS)

# Create a rule to remove object files
clean-objs:
	rm -f $(OBJS)

