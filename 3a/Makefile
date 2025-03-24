CC = cc
CFLAGS = -c -Wall -Wextra -g
TARGET = prog
LIB = table
LIBTYPE ?= dynamic
OBJDIR = build
LIBDIR = ../libs/ld
LDFLAGS = -l$(LIB) -lreadline -linput
OBJ = $(addprefix $(OBJDIR)/, $(patsubst %.c, %.o, $(filter-out $(LIB).c, $(wildcard *.c))))
HDIR = ../libs

all: $(OBJDIR) $(LIBTYPE) $(TARGET)

$(OBJDIR):
	mkdir $(OBJDIR)

$(OBJDIR)/%.o: %.c
	$(CC) -I$(HDIR) $(CFLAGS) $< -o $@

$(OBJDIR)/$(LIB).o: $(LIB).c
	$(CC) -fpic -I$(HDIR) $(CFLAGS) $< -o $@

static: $(OBJDIR)/$(LIB).o
	ar rcs $(OBJDIR)/lib$(LIB).a $<

dynamic: $(OBJDIR)/$(LIB).o
	$(CC) -shared $(OBJDIR)/$(LIB).o -o $(LIBDIR)/lib$(LIB).so

$(TARGET): $(OBJ)
	$(CC) -o prog -I$(HDIR) $^ $(LDFLAGS) -L$(LIBDIR) -Wl,-rpath=$(LIBDIR)

clean:
	rm -Rf $(OBJDIR)
	rm -f prog
