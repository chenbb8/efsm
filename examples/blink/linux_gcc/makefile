CC = gcc

CINCLUDES = -I. -I../../../efsm
CODE_DIR := .
EFSM_DIR := ../../../efsm

CFLAGS = $(CINCLUDES) -O0

EXE = blink

CFILES += $(CODE_DIR)/main.c \
	$(EFSM_DIR)/efsm.c \
	$(EFSM_DIR)/efsm_hal.c \

OBJFILES = $(CFILES:.c=.o)

.PHONY: all clean

all : $(EXE)

$(EXE) : $(OBJFILES)
	$(CC) $(CFLAGS) -o $(EXE) $(CFILES)

$(OBJFILES) : $(CFILES)
	$(CC) $(CFLAGS) -c $^

clean:
	rm -rf *.o
	rm -rf $(EXE)

