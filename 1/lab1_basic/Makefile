CC = cc
CFLAGS = -c -Wall -Wextra -Werror -g
TARGET = prog

OBJDIR = build
OBJ = $(addprefix $(OBJDIR)/, $(patsubst %.c, %.o, $(wildcard *.c)))

all: $(OBJDIR) $(TARGET)

$(OBJDIR):
	mkdir $(OBJDIR)

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

$(TARGET): $(OBJ)
	$(CC) -o $@ $^

clean:
	rm -Rf $(OBJDIR)
	rm -f prog
