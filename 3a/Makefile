CC = cc
CFLAGS = -c -Wall -Wextra -g
TARGET = prog
LIB = table
LIBTYPE ?= dynamic
OBJDIR = build
LIBDIR = build
LDFLAGS = -l$(LIB)
OBJ = $(addprefix $(OBJDIR)/, $(patsubst %.c, %.o, $(filter-out $(LIB).c, $(wildcard *.c))))

all: $(OBJDIR) $(LIBTYPE) $(TARGET)

$(OBJDIR):
	mkdir $(OBJDIR)

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

$(OBJDIR)/$(LIB).o: $(LIB).c
	$(CC) -fpic $(CFLAGS) $< -o $@

static: $(OBJDIR)/$(LIB).o
	ar rcs $(OBJDIR)/lib$(LIB).a $<

dynamic: $(OBJDIR)/$(LIB).o
	$(CC) -shared $(OBJDIR)/$(LIB).o -o $(OBJDIR)/lib$(LIB).so

$(TARGET): $(OBJ) $(OBJDIR)/$(LIB).o
	$(CC) -o prog $^ $(LDFLAGS) -L$(LIBDIR) -Wl,-rpath=$(LIBDIR)

clean:
	rm -Rf $(OBJDIR)
	rm -f prog
