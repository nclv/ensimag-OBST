CC = gcc
LD = gcc

LATEXC=pdflatex
DOCC=doxygen

# -O0 désactive les optimisations à la compilation
# C'est utile pour débugger, par contre en "production"
# on active au moins les optimisations de niveau 2 (-O2).
CFLAGS = -std=c99 -Wall -Wextra -ggdb -O3 -Wshadow -Wconversion -Wdouble-promotion -Wformat=2 -Wformat-truncation=2 -Wundef -fno-common -fstack-usage -fstack-protector-all -Wfloat-equal -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5 -Wwrite-strings -Waggregate-return -ffunction-sections -fdata-sections -Iincludes/
LDFLAGS = -Wl,--gc-sections -Wl,--print-gc-sections -Wl,-z,relro,-z,now

# Par défaut, on compile tous les fichiers source (.c) qui se trouvent dans le
# répertoire src/
SRC_FILES=$(wildcard src/*.c)

# Par défaut, la compilation de src/toto.c génère le fichier objet obj/toto.o
OBJ_FILES=$(patsubst src/%.c,obj/%.o,$(SRC_FILES))

REFDIR=.
SRCDIR=$(REFDIR)/src
BINDIR=$(REFDIR)/bin
DOCDIR=$(REFDIR)/doc
REPORTDIR=$(REFDIR)/rapport

LATEXSOURCE=$(wildcard $(REPORTDIR)/*.tex)
CSOURCE=$(wildcard $(SRCDIR)/compileBST.c)
PDF=$(LATEXSOURCE:.tex=.pdf)

all: binary report doc 

############# Binary #############

obj/%.o: src/%.c
	$(CC) -c $(CFLAGS) $< -o $@

$(BINDIR)/compileBST: $(OBJ_FILES)
	$(LD) $(LDFLAGS) $(OBJ_FILES) -o $@

binary: $(BINDIR)/compileBST

############# Report #############

%.pdf: $(LATEXSOURCE)
	$(LATEXC) -output-directory $(REPORTDIR) $^ 

# $(DOCDIR)/index.html: $(SRCDIR)/Doxyfile $(CSOURCE) 
# 	$(DOCC) $(SRCDIR)/Doxyfile

report: $(PDF) 

# doc: $(DOCDIR)/index.html

clean:
	rm -rf $(DOCDIR) $(BINDIR)/* $(REPORTDIR)/*.aux $(REPORTDIR)/*.log  $(REPORTDIR)/rapport.pdf obj/*


.PHONY: all doc binary report clean
