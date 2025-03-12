CC = cc
CFLAGS = -c -Wall -Wextra -g
LDFLAGS = -lreadline
TARGET ?= array
LIBTYPE ?= static
OBJDIR = build
LIBDIR = build
OBJ = $(addprefix $(OBJDIR)/, $(patsubst %.c, %.o, $(filter-out stack_%, $(wildcard *.c))))

all: $(OBJDIR) $(LIBTYPE) $(TARGET)

$(OBJDIR):
	mkdir $(OBJDIR)

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

$(OBJDIR)/$(TARGET).o: stack_$(TARGET).c
	$(CC) -fpic $(CFLAGS) $< -o $@

static: $(OBJDIR)/$(TARGET).o
	ar rcs $(OBJDIR)/libstack.a $<

dinamic: $(OBJDIR)/$(TARGET).o
	$(CC) -shared $(OBJDIR)/$(TARGET).o -o $(OBJDIR)/libstack.so

$(TARGET): $(OBJ) $(OBJDIR)/$(TARGET).o
	$(CC) -o prog $^ $(LDFLAGS) -L$(LIBDIR) -lstack -Wl,-rpath=$(LIBDIR)

clean:
	rm -Rf $(OBJDIR)
	rm -f prog
