LIBS_PATH = /lib/x86_64-linux-gnu/ /usr/lib/x86_64-linux-gnu/
INCS =
LIBS = -l pthread

SRC_QUEUE = pa_list.c
OBJ_QUEUE = ${SRC_QUEUE:.c=.o}

CC = gcc
CFLAGS = -std=c11 -c -g -Wall -Werror -pie -fPIC -pedantic ${INCS}
LDFLAGS = ${LIBS}

all: libpa_queue.so

.c.o:
		@echo CC $<
		@${CC} ${CFLAGS} $<

libpa_queue.so: ${OBJ_QUEUE}
		@echo CC -o $@
		@${CC} -shared -o $@ ${OBJ_QUEUE} ${LDFLAGS}

clean:
		@echo cleaning
		@rm -f ${OBJ_QUEUE}
		@rm -f test_list
		@rm -f test_tpool
