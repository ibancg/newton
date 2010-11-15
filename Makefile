
PROJECT  = newton
SOURCES   = *.cc

C_FLAGS   = -Wall -g -O2 $(INCDIR)
LIBRARIES = -lm -lstdc++

SRCS = $(wildcard $(SOURCES))
OBJS = $(SRCS:.cc=.o)

all: $(PROJECT)

$(PROJECT): .depend $(OBJS)
	g++ -o $(PROJECT) $(OBJS) $(LIBRARIES)

%.o: %.cc .depend
	g++ $(C_FLAGS) -c $< -o $@

run: $(PROJECT)
clean:
	@rm -f $(OBJS)
	@rm -f .depend

.PHONY: all run clean

.DEFAULT:
	@g++ -M *.cc > .depend

sinclude .depend
