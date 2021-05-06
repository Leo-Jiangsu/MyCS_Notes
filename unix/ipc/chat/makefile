CC := gcc
RM := rm -f

srcs := $(wildcard *.c)
outs := $(patsubst %.c, %, $(srcs))

#all:$(outs)	
#	@for i in $(outs); do $(CC) $$i.c -o $$i;done

all:$(outs)
% : %.c header/func.c
	$(CC) $^ -o $@ -Wall

.PHONY:clean rebuild
rebuild:clean all
clean:
	$(RM) $(outs)



