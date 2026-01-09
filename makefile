COMPILER = gcc
FLAGS = -Wall -Wextra -g -Isrc/data_structures -Iinclude
EXEC = server

SRC = \
	src/data_structures/strings/str.c \
	src/data_structures/array/array.c \
	src/data_structures/lists/list.c \
	src/data_structures/lists/concurrent_list.c \
	src/data_structures/hashtable/hashtable.c \
	src/main.c


build:
	$(COMPILER) $(FLAGS) $(SRC) -o $(EXEC)

clean:
	rm $(EXEC)
