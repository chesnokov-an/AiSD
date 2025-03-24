CC = cc
CFLAGS = -c -Wall -Wextra -g
LDFLAGS = -lreadline
TARGET ?= input
OBJDIR = build
LIBDIR = ld
OBJ = $(addprefix $(OBJDIR)/, $(patsubst %.c, %.o, $(wildcard *.c)))

all: $(OBJDIR) $(LIBDIR) dynamic

$(OBJDIR):
	mkdir $(OBJDIR)

$(LIBDIR):
	mkdir $(LIBDIR)

$(OBJDIR)/$(TARGET).o: $(TARGET).c
	$(CC) -fpic $(CFLAGS) $< -o $@

dynamic: $(OBJDIR)/$(TARGET).o
	$(CC) -shared $(OBJDIR)/$(TARGET).o -o $(LIBDIR)/lib$(TARGET).so

clean:
	rm -Rf $(OBJDIR)
	rm -Rf $(LIBDIR)
