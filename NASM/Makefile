ifeq ($(OS),Windows_NT)
	AFMT = win32 -g
	LFLG = -mi386pe -subsystem console --discard-locals
	LIBS = -LC:\MinGW\lib -lkernel32 -luser32
	FGDB =
	OUT  = test.exe
else
	AFMT = -Wall -g -f elf32
	LFLG = -m32
	LIBS =
	FGDB = -tui
	OUT  = ./test
endif

LIBSRCS = $(wildcard libprax/*.asm)
LIBOBJS = $(LIBSRCS:libprax/%.asm=obj/%.o)
LIBOUT  = libprax.lib

CSRCS  = $(wildcard *.c)
COBJS  = $(CSRCS:%.c=obj/%.o)
CFLAGS = -m32 -Wall -O3 -g

#ld: -nostdlib
all: obj makelib $(COBJS)
	gcc $(LFLG) -o $(OUT) $(COBJS) $(LIBOUT) $(LIBS)
	
gdb: all
	gdb -q -x gdbinit $(FGDB) $(OUT)
	
# create the static library with >ar rcs out.lib in.o in.o
# rebuild asm objs if sources or macro include changes
makelib: $(LIBOBJS)
	ar rcs $(LIBOUT) $(LIBOBJS)
obj:
	mkdir obj
	
obj/%.o: libprax/%.asm libprax/macros.inc
	nasm -ilibprax/ $(AFMT) libprax/$*.asm -o obj/$*.o
obj/%.o: %.c
	gcc $(CFLAGS) -c $*.c -o obj/$*.o
	
clean:
	rm -rf obj $(LIBOUT) $(OUT)

run: all
	$(OUT)