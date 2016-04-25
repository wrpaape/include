.PHONY: all clean

.EXPORT_ALL_VARIABLES:

SHELL := /bin/zsh
INC_DIR = $(C_INCLUDE_PATH)
LIB_DIR = $(LIBRARY_PATH)


UTILS_DIR = $(INC_DIR)/utils

CC     = gcc
CFLAGS = -g -I$(INC_DIR) -std=c99 -Wall -D__USE_FIXED_PROTOTYPES__
AR     = ar
AFLAGS = rcs

RAND_PROG = rand
RAND_DIR  = $(UTILS_DIR)
RAND_HDRS = $(RAND_PROG) pcg_basic
RAND_DEPS = $(addprefix $(RAND_DIR)/, $(RAND_PROG).c $(addsuffix .h, $(RAND_HDRS)))
RAND_OBJ  = $(addprefix $(RAND_DIR)/, $(addsuffix .o, $(RAND_PROG)))
RAND_LIB  = $(addprefix $(LIB_DIR)/,  $(addsuffix .a, $(addprefix lib, $(RAND_PROG))))


all : $(RAND_LIB)

$(RAND_LIB) : $(RAND_OBJ)
	$(AR) $(AFLAGS) $@ $^

$(RAND_OBJ) : $(RAND_DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

# pcg_basic.o: pcg_basic.c pcg_basic.h
# 		$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(INC_DIR)/**/*.o $(INC_DIR)/**/*~ $(LIB_DIR)/*.a

