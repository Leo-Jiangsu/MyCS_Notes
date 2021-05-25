CC := gcc
RM := rm -f

srcs := $(wildcard src/*.c)
# outs := $(patsubst srcs/%.c, %, $(srcs))
# test := $(wildcard *.c)
# test := test.c
test := $(wildcard *.c)
outs := $(patsubst %.c, %, $(test))

all:$(outs)
$(outs) : $(srcs)
	for i in $(outs); do $(CC) $$i.c $^ -o $$i -Wall -lpthread;done

# all:$(outs)
# % : $(srcs)
# $(outs) : $(test) $(srcs)
# 	$(CC) $^ -o $@ -Wall -lpthread

.PHONY:clean rebuild
rebuild:clean all
clean:
	$(RM) $(outs)



