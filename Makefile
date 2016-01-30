OBJDIR  = obj

CSRCS  	= $(wildcard *.c)
COBJS  	= $(CSRCS:%.c=$(OBJDIR)/%.o)
CFLAGS 	= -m32 -std=gnu99 -Wall -Wno-unused-result -O3 -g

ASMFLG 	= -f elf32 -F dwarf -g
LIBSRCS = $(wildcard matlib/*.asm)
LIBOBJS = $(LIBSRCS:matlib/%.asm=$(OBJDIR)/%.o)
LIBOUT  = matlib.lib

FGDB 	= -tui

LFLG 	= -m32 -Iinc/
OUT		= executable
LIBS 	= -lGLEW -lGL libs/libSOIL.a -lm -lglfw



all: $(OUT)
	@echo
clean:
	rm -rf $(OBJDIR) $(OUT) $(LIBOUT)
run: all
	./$(OUT)
debug: all
	gdb -q -x gdbinit $(FGDB) $(OUT)

install:
	sudo apt-get install libgl1-mesa-dev:i386
	sudo apt-get install libglu1-mesa-dev:i386
	sudo apt-get install libglew-dev:i386


# OUT depends on OBJDIR, MATLIB, C-OBJS
$(OUT): $(OBJDIR) $(LIBOBJS) $(COBJS)
	@echo link $(OUT)
	@gcc $(LFLG) -o $(OUT) $(LIBOBJS) $(COBJS) $(LIBS)


$(OBJDIR)/%.o: matlib/%.asm
	@echo nasm $(LIBSRCS)
	@nasm -imatlib/ $(ASMFLG) $(LIBSRCS) -o $(LIBOBJS)

$(OBJDIR)/%.o: %.c
	@echo " gcc $*"
	@gcc $(CFLAGS) -c $*.c -o $(OBJDIR)/$*.o -MD

$(OBJDIR):
	@mkdir $(OBJDIR)
