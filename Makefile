TARGET  = contest

all:	$(TARGET)

SRCDIR := src
OBJDIR := obj
LOGDIR := logs

TARGET_BIN      = $(TARGET).bin
TARGET_HEX      = $(TARGET).hex

TARGET_MAP      = obj/$(TARGET).map
TARGET_LST      = obj/$(TARGET).lst

SRCLIST	= \
	main \
	bmpread \
	contest \
	draw \
	dump \
	parse \
	search \
	vvvm \
	process

OBJ     = $(SRCLIST:%=$(OBJDIR)/%.o)

CFLAGS	= \
	-Wall -Werror -Wextra \
	-Wmissing-declarations -Wredundant-decls \
	-Wno-unused-parameter \
	-Wcast-qual -Wcast-align \
	-Wshadow \
	-Wstrict-prototypes -Wmissing-prototypes \
	-Wbad-function-cast -Wnested-externs \
	-Wredundant-decls \
	-Wno-unused-parameter \
	-O2 -g \
	-Wa,-adhlns=$(<:src/%.c=$(OBJDIR)/%.lst)

LDFLAGS	= \
	-Map=$(TARGET_MAP) --cref

.c.o:
	gcc $(CFLAGS) -c $< -o $@

$(OBJDIR)/bmpread.o:	$(SRCDIR)/bmpread.c $(SRCDIR)/contest.h
$(OBJDIR)/contest.o:	$(SRCDIR)/contest.c $(SRCDIR)/contest.h
$(OBJDIR)/draw.o:	$(SRCDIR)/draw.c $(SRCDIR)/contest.h
$(OBJDIR)/dump.o:	$(SRCDIR)/dump.c $(SRCDIR)/contest.h
$(OBJDIR)/parse.o:	$(SRCDIR)/parse.c $(SRCDIR)/contest.h
$(OBJDIR)/search.o:	$(SRCDIR)/search.c $(SRCDIR)/contest.h
$(OBJDIR)/main.o:	$(SRCDIR)/main.c $(SRCDIR)/contest.h
$(OBJDIR)/vvvm.o:	$(SRCDIR)/vvvm.c $(SRCDIR)/contest.h
$(OBJDIR)/process.o:	$(SRCDIR)/process.c $(SRCDIR)/contest.h

$(OBJ): | $(OBJDIR)
$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@  $<


$(TARGET): $(OBJ)
	gcc $(LDFLAGS) -o $(TARGET) $(OBJ)
	objdump -h -S -z $(TARGET) > $(TARGET_LST)

clean:
	rm -f $(TARGET)
	rm -f $(OBJ) $(OBJ:%.o=%.lst)
	rm -f $(TARGET_MAP) $(TARGET_LST)

tst:	$(TARGET)
	./$(TARGET) data/pic.bmp

$(LOGDIR):
	@mkdir -p $(LOGDIR)

bmp:	$(TARGET) | $(LOGDIR)
	./$(TARGET) data/pic.bmp $(LOGDIR)
