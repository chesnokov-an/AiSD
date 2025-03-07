CC = cc
CFLAGS = -c -Wall -Wextra -g
LDFLAGS = -lreadline
TARGET = array

OBJDIR = build
OBJ = $(addprefix $(OBJDIR)/, $(patsubst %.c, %.o, $(filter-out stack_%, $(wildcard *.c))))

all: $(OBJDIR) $(TARGET)

$(OBJDIR):
	mkdir $(OBJDIR)

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

$(OBJDIR)/$(TARGET).o: stack_$(TARGET).c
	$(CC) $(CFLAGS) $< -o $@

$(TARGET): $(OBJ) $(OBJDIR)/$(TARGET).o
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -Rf $(OBJDIR)
	rm -f array
	rm -f list
