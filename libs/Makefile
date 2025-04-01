CC = cc
CFLAGS = -c -Wall -Wextra -g
LDFLAGS = -lreadline
OBJDIR = build
LIBDIR = ld
OBJ = $(addprefix $(OBJDIR)/, $(patsubst %.c, %.o, $(wildcard *.c)))
LIB = $(addprefix $(LIBDIR)/lib, $(patsubst %.c, %.so, $(wildcard *.c)))
all: $(OBJDIR) $(LIBDIR) $(LIB)

$(OBJDIR):
	mkdir $(OBJDIR)

$(LIBDIR):
	mkdir $(LIBDIR)

$(OBJDIR)/%.o: %.c
	$(CC) -fpic $(CFLAGS) $< -o $@

$(LIBDIR)/lib%.so: $(OBJDIR)/%.o
	$(CC) -shared $< -o $@

clean:
	rm -Rf $(OBJDIR)
	rm -Rf $(LIBDIR)
