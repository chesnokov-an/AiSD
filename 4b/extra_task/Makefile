CC = cc
CFLAGS = -c -Wall -Wextra -g
TARGET = prog
OBJDIR = build
LIBDIR1 = ../../libs/ld
LIBDIR2 = ../build
LDFLAGS = -lmy_readline -lreadline -linput -lm -ltree -lcgraph -lgvc
OBJ = $(addprefix $(OBJDIR)/, $(patsubst %.c, %.o, $(wildcard *.c)))
HDIR = ../../libs

all: $(MAKE) $(OBJDIR) $(TARGET)

$(OBJDIR):
	mkdir $(OBJDIR)

$(MAKE):
	cd ../ && make

$(OBJDIR)/%.o: %.c
	$(CC) -I$(HDIR) -I../ $(CFLAGS) $< -o $@

$(TARGET): $(OBJ)
	$(CC) -o prog -I$(HDIR) -I../ -I../libs $^ -L$(LIBDIR1) -L$(LIBDIR2) -Wl,-rpath=$(LIBDIR1),-rpath=$(LIBDIR2) $(LDFLAGS)

clean:
	rm -Rf $(OBJDIR)
	rm -f prog
	rm -f $(OBJDIR)/lib$(LIB).so
