TARGET  = contest

all:	$(TARGET)

SRCDIR := src
OBJDIR := obj

TARGET_BIN      = $(TARGET).bin
TARGET_HEX      = $(TARGET).hex

TARGET_MAP      = obj/$(TARGET).map
TARGET_LST      = obj/$(TARGET).lst

SRCLIST	= \
	main \
	bmpread \
	contest \
	draw \

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

draw.o: draw.c
bmpread.o: bmpread.c
main.o:	main.c

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
