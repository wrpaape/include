.PHONY: all clean

INC_DIR = $(C_INCLUDE_PATH)
LIB_DIR = $(LIBRARY_PATH)

UTILS_DIR = $(INC_DIR)/utils
PCGB_DIR  = $(UTILS_DIR)
RAND_DIR  = $(UTILS_DIR)
BHEAP_DIR = $(INC_DIR)/bheap

CC     = gcc
CFLAGS = -g -I$(INC_DIR) -std=c99 -Wall -D__USE_FIXED_PROTOTYPES__
AR     = ar
AFLAGS = rcs

UTILS_NAME = utils
UTILS_SRC  = $(addprefix $(UTILS_DIR)/, $(addsuffix .c, $(UTILS_NAME)))
UTILS_HDR  = $(addprefix $(UTILS_DIR)/, $(addsuffix .h, $(UTILS_NAME)))
UTILS_OBJ  = $(addprefix $(UTILS_DIR)/, $(addsuffix .o, $(UTILS_NAME)))
UTILS_LIB  = $(addprefix $(LIB_DIR)/,   $(addsuffix .a, $(addprefix lib, $(UTILS_NAME))))
UTILS_ODEP = $(UTILS_SRC) $(UTILS_HDR)
UTILS_LDEP = $(UTILS_OBJ)

PCGB_NAME = pcg_basic
PCGB_SRC  = $(addprefix $(PCGB_DIR)/, $(addsuffix .c, $(PCGB_NAME)))
PCGB_HDR  = $(addprefix $(PCGB_DIR)/, $(addsuffix .h, $(PCGB_NAME)))
PCGB_OBJ  = $(addprefix $(PCGB_DIR)/, $(addsuffix .o, $(PCGB_NAME)))
PCGB_ODEP = $(PCGB_SRC) $(PCGB_HDR)

RAND_NAME = rand
RAND_SRC  = $(addprefix $(RAND_DIR)/, $(addsuffix .c, $(RAND_NAME)))
RAND_HDR  = $(addprefix $(RAND_DIR)/, $(addsuffix .h, $(RAND_NAME)))
RAND_OBJ  = $(addprefix $(RAND_DIR)/, $(addsuffix .o, $(RAND_NAME)))
RAND_LIB  = $(addprefix $(LIB_DIR)/,  $(addsuffix .a, $(addprefix lib, $(RAND_NAME))))
RAND_ODEP = $(RAND_SRC) $(RAND_HDR) $(PCG_HDR)
RAND_LDEP = $(RAND_OBJ) $(PCGB_OBJ)

BHEAP_NAME = bheap
BHEAP_SRC  = $(addprefix $(BHEAP_DIR)/, $(addsuffix .c, $(BHEAP_NAME)))
BHEAP_HDR  = $(addprefix $(BHEAP_DIR)/, $(addsuffix .h, $(BHEAP_NAME)))
BHEAP_OBJ  = $(addprefix $(BHEAP_DIR)/, $(addsuffix .o, $(BHEAP_NAME)))
BHEAP_LIB  = $(addprefix $(LIB_DIR)/,   $(addsuffix .a, $(addprefix lib, $(BHEAP_NAME))))
BHEAP_ODEP = $(BHEAP_SRC) $(BHEAP_HDR) $(UTILS_HDR)
BHEAP_LDEP = $(BHEAP_OBJ) $(UTILS_OBJ)

ALL_LIBS = $(UTILS_LIB) $(RAND_LIB) $(BHEAP_LIB)

all: $(ALL_LIBS)

$(UTILS_LIB): $(UTILS_LDEP)
	$(AR) $(AFLAGS) $@ $^

$(RAND_LIB): $(RAND_LDEP)
	$(AR) $(AFLAGS) $@ $^

$(BHEAP_LIB): $(BHEAP_LDEP)
	$(AR) $(AFLAGS) $@ $^

$(UTILS_OBJ): $(UTILS_ODEP)
	$(CC) $(CFLAGS) -c -o $@ $<

$(PCGB_OBJ): $(PCGB_ODEP)
	$(CC) $(CFLAGS) -c -o $@ $<

$(RAND_OBJ): $(RAND_ODEP)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BHEAP_OBJ): $(BHEAP_ODEP)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(LIB_DIR)/*.a $(INC_DIR)/**/*.o $(INC_DIR)/**/*~ $(INC_DIR)/*~
