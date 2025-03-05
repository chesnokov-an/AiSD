CC = cc
CFLAGS = -c -Wall -Wextra -g
LDFLAGS = -lreadline
TARGET_1 = array
TARGET_2 = list

OBJDIR = build
OBJ = $(addprefix $(OBJDIR)/, $(patsubst %.c, %.o, $(filter-out stack_%, $(wildcard *.c))))

all: $(TARGET_1) $(TARGET_2)

$(OBJDIR):
	mkdir $(OBJDIR)

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

$(OBJDIR)/$(TARGET_1).o: stack_$(TARGET_1).c
	$(CC) $(CFLAGS) $< -o $@

$(OBJDIR)/$(TARGET_2).o: stack_$(TARGET_2).c
	$(CC) $(CFLAGS) $< -o $@

$(TARGET_1): $(OBJDIR) $(OBJ) $(OBJDIR)/$(TARGET_1).o
	$(CC) -o $@ $(filter %.o,$^) $(LDFLAGS)

$(TARGET_2): $(OBJDIR) $(OBJ) $(OBJDIR)/$(TARGET_2).o
	$(CC) -o $@ $(filter %.o,$^) $(LDFLAGS)

clean:
	rm -Rf $(OBJDIR)
	rm -f array
	rm -f list
