CC = cc
CFLAGS = -c -Wall -Wextra -g
LDFLAGS = -lreadline
TARGET ?= array
LIBTYPE ?= static
LIBDIR = .
OBJDIR = build
OBJ = $(addprefix $(OBJDIR)/, $(patsubst %.c, %.o, $(filter-out stack_%, $(wildcard *.c))))

all: $(OBJDIR) $(LIBTYPE) $(TARGET)

$(OBJDIR):
	mkdir $(OBJDIR)

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

$(OBJDIR)/$(TARGET).o: stack_$(TARGET).c
	$(CC) -fpic $(CFLAGS) $< -o $@

static: $(OBJDIR)/$(TARGET).o
	ar rcs lib$(TARGET).a $<

dinamic: $(OBJDIR)/$(TARGET).o
	$(CC) -shared $(OBJDIR)/$(TARGET).o -o lib$(TARGET).so

$(TARGET): $(OBJ) $(OBJDIR)/$(TARGET).o
	$(CC) -o $@ $^ $(LDFLAGS) -L$(LIBDIR) -l$(TARGET)

clean:
	rm -Rf $(OBJDIR)
	rm -f array
	rm -f list
