BINDIR=./bin
SRCDIR=./src
OBJDIR=./obj
INCDIR=./include

EXEC=main
ARGS=-g -std=c++20 -Wall -Wextra
LDLIBS=-lSDL2
CC=g++

# ----------------------------------------------

SRC=$(wildcard $(SRCDIR)/*.cpp)

INC:=$(wildcard $(INCDIR)/*.hpp) $(wildcard $(INCDIR)/*.h)
OBJ:=$(SRC:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

$(BINDIR)/$(EXEC): $(OBJ) $(INC) Makefile
	@mkdir -p $(BINDIR)
	$(CC) -o $@ $(OBJ) $(ARGS) $(LDLIBS)

$(OBJ): $(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(INC) Makefile
	@mkdir -p $(OBJDIR)
	$(CC) -o $@ -c $< -I$(INCDIR) $(ARGS)

.PHONY: test_makefile
test_makefile:
	@echo "SRC : " ${SRC}
	@echo "INC : " ${INC}
	@echo "OBJ : " ${OBJ}

.PHONY: exec
exec: $(BINDIR)/$(EXEC)
	@echo -n "début exécution de "
	$(BINDIR)/$(EXEC)

.PHONY: clean
clean:
	rm -rf $(BINDIR)/* $(OBJDIR)/*