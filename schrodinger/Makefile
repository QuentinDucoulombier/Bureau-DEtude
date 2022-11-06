## Configuration

# Compilateur
CC = gcc -c
CFLAGS = -Wall -pedantic

# Editeur de liens
LD = gcc
LDFLAGS = -lgsl -lgslcblas -lm

# gnuplot
GP = gnuplot
GPFLAGS = -p -c

## Fichiers
PROJECT = schrodinger

# Fichiers
HDR = $(wildcard *.h)
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
BIN = ./$(PROJECT)
DAT = $(PROJECT).dat
GPI = $(PROJECT).gpi
IMG = $(PROJECT).png

# 'clean' et 'reset' ne sont pas des noms de vrais fichiers
.PHONY : clean reset

## Dependances

# Graphique gnuplot
$(IMG) : $(GPI) $(DAT)
	$(GP) $(GPFLAGS) $<

# Fichier de donnees
$(DAT) : $(BIN)
	$(BIN)

# Edition des liens
$(BIN) : $(OBJ)
	$(LD) $^ $(LDFLAGS) -o $@

# Compilation
equadiff.o : equadiff.c equadiff.h consts.h
	$(CC) $(CFLAGS) $< -o $@
tir.o : tir.c equadiff.o equadiff.h consts.h
	$(CC) $(CFLAGS) $< -o $@

# Compilation : regle par default
%.o : %.c
	$(CC) $(CFLAGS) $< -o $@

# Regles de nettoyage
clean :
	rm -f $(OBJ)
reset : clean
	rm $(BIN) $(DAT) $(IMG)
