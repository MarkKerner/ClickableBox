OBJDIR  = obj
SRCS  = $(wildcard *.c)
OBJS  = $(SRCS:%.c=$(OBJDIR)/%.o)
CFLAGS = -m32 -std=gnu99 -I headers/ -L libs/ -Wall -Wno-unused-result -g -O3

OUT	= executable
LIBS 	= -lgsl -lkazmath
OPENGL 	= -lGLEW -lGL -lglfw -lSOIL -lm

all: $(OUT)
	@echo
clean:
	@rm -rf ./$(OBJDIR) ./$(OUT)
run: all
	./$(OUT)
debug: all
	gdb -q -x gdbinit $(FGDB) $(OUT)

ifeq ($(OS),Windows_NT)
install:
else
install:
	sudo apt-get install libgl1-mesa-dev:i386
	sudo apt-get install libglu1-mesa-dev:i386
	sudo apt-get install freeglut3-dev:i386
endif

	
# OUT depends on OBJDIR, C-OBJS
$(OUT): $(OBJDIR) $(OBJS)
	@echo link $(OUT)
	@gcc -m32 -o $(OUT) $(OBJDIR)/*.o $(LIBS) $(OPENGL)  
$(OBJDIR)/%.o: %.c
	@echo " gcc $*"
	@gcc $(CFLAGS) -c $*.c -o $(OBJDIR)/$*.o -MD

$(OBJDIR):
	@mkdir $(OBJDIR)
