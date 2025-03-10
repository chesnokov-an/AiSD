CC = cc
CFLAGS = -c -Wall -Wextra -g
LDFLAGS = -lreadline
TARGET ?= array
LIBTYPE ?= static
OBJDIR = build
LIBDIR = ./build/
OBJ = $(addprefix $(OBJDIR)/, $(patsubst %.c, %.o, $(filter-out stack_%, $(wildcard *.c))))

all: $(OBJDIR) $(LIBTYPE) $(TARGET)

$(OBJDIR):
	mkdir $(OBJDIR)

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

$(OBJDIR)/$(TARGET).o: stack_$(TARGET).c
	$(CC) -fpic $(CFLAGS) $< -o $@

static: $(OBJDIR)/$(TARGET).o
	ar rcs $(OBJDIR)/lib$(TARGET).a $<

dinamic: $(OBJDIR)/$(TARGET).o
	$(CC) -shared $(OBJDIR)/$(TARGET).o -o $(OBJDIR)/lib$(TARGET).so

$(TARGET): $(OBJ) $(OBJDIR)/$(TARGET).o
	$(CC) -o $@ $^ $(LDFLAGS) -L$(LIBDIR) -l$(TARGET) -Wl,-rpath=$(LIBDIR)

clean:
	rm -Rf $(OBJDIR)
	rm -f array
	rm -f list
