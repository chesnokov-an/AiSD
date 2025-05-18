CC = cc
CFLAGS = -c -Wall -Wextra -g
TARGET = prog
LIB = graph
DFLAGS ?= -DDEBUG
OBJDIR = build
LIBDIR = ../libs/ld
LDFLAGS = -l$(LIB) -lreadline -linput -lmy_readline -lm -lcgraph -lgvc
OBJ = $(addprefix $(OBJDIR)/, $(patsubst %.c, %.o, $(filter-out $(LIB).c, $(wildcard *.c))))
HDIR = ../libs

all: $(MAKE) $(OBJDIR) $(OBJDIR)/lib$(LIB).so $(TARGET)

$(OBJDIR):
	mkdir $(OBJDIR)

$(MAKE):
	cd $(HDIR) && make

$(OBJDIR)/%.o: %.c
	$(CC) -I$(HDIR) $(CFLAGS) $< -o $@ $(DFLAGS)

$(OBJDIR)/$(LIB).o: $(LIB).c
	$(CC) -fpic -I$(HDIR) $(CFLAGS) $< -o $@ $(DFLAGS)

$(OBJDIR)/lib$(LIB).so: $(OBJDIR)/$(LIB).o
	$(CC) -shared $(OBJDIR)/$(LIB).o -o $(OBJDIR)/lib$(LIB).so $(DFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o prog -I$(HDIR) $^ $(LDFLAGS) -L$(LIBDIR) -L$(OBJDIR) -Wl,-rpath=$(LIBDIR),-rpath=$(OBJDIR)

clean:
	rm -Rf $(OBJDIR)
	rm -f prog
	rm -f $(OBJDIR)/lib$(LIB).so
