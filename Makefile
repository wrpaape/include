.PHONY: all clean

INC_DIR = $(C_INCLUDE_PATH)
LIB_DIR = $(LIBRARY_PATH)


UTILS_DIR = $(INC_DIR)/utils

CC     = gcc
CFLAGS = -g -I$(INC_DIR) -std=c99 -Wall -D__USE_FIXED_PROTOTYPES__
AR     = ar
AFLAGS = rcs

PCGB_PROG = pcg_basic
PCGB_DIR  = $(UTILS_DIR)
PCGB_HDRS = $(PCGB_PROG)
PCGB_DEPS = $(addprefix $(PCGB_DIR)/, $(PCGB_PROG).c $(addsuffix .h, $(PCGB_HDRS)))
PCGB_OBJ  = $(addprefix $(PCGB_DIR)/, $(addsuffix .o, $(PCGB_PROG)))

RAND_PROG = rand
RAND_DIR  = $(UTILS_DIR)
RAND_HDRS = $(RAND_PROG) $(PCGB_PROG)
RAND_DEPS = $(addprefix $(RAND_DIR)/, $(RAND_PROG).c $(addsuffix .h, $(RAND_HDRS)))
RAND_OBJ  = $(addprefix $(RAND_DIR)/, $(addsuffix .o, $(RAND_PROG)))
RAND_LIB  = $(addprefix $(LIB_DIR)/,  $(addsuffix .a, $(addprefix lib, $(RAND_PROG))))


all: $(RAND_LIB)

$(RAND_LIB): $(RAND_OBJ) $(PCGB_OBJ)
	$(AR) $(AFLAGS) $@ $^

$(RAND_OBJ): $(RAND_DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(PCGB_OBJ): $(PCGB_DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(LIB_DIR)/*.a $(INC_DIR)/**/*.o $(INC_DIR)/**/*~ $(INC_DIR)/*~ 
