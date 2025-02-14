CC = cc
CFLAGS = -c -Wall -Wextra -Werror -g -lreadline
TARGET = prog
LDFLAGS = -lreadline
OBJDIR = build
OBJ = $(addprefix $(OBJDIR)/, $(patsubst %.c, %.o, $(wildcard *.c)))

all: $(OBJDIR) $(TARGET)

$(OBJDIR):
	mkdir $(OBJDIR)

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	rm -Rf $(OBJDIR)
	rm -f prog
