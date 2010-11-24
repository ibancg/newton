
PROJECT  = newton
SOURCES   = *.c

C_FLAGS   = -Wall -g -O2 $(INCDIR)
LIBRARIES = -lm -lstdc++ -lSDL

SRCS = $(wildcard $(SOURCES))
OBJS = $(SRCS:.c=.o)

all: $(PROJECT)

$(PROJECT): .depend $(OBJS)
	gcc -o $(PROJECT) $(OBJS) $(LIBRARIES)

%.o: %.c .depend
	gcc $(C_FLAGS) -c $< -o $@

run: $(PROJECT)
clean:
	@rm -f $(OBJS)
	@rm -f .depend

.PHONY: all run clean

.DEFAULT:
	@gcc -M *.c > .depend

sinclude .depend
