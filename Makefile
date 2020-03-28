LIBS_PATH = /lib/x86_64-linux-gnu/ /usr/lib/x86_64-linux-gnu/
INCS =
LIBS = -l pthread

SRC_PA = palist.c patpool.c
OBJ_PA = ${SRC_PA:.c=.o}
PA_LDFLAGS = ${LIBS}

SRC_TEST_PALIST = test_palist.c
OBJ_TEST_PALIST = ${SRC_TEST_PALIST:.c=.o}
SRC_TEST_PATPOOL = test_patpool.c
OBJ_TEST_PATPOOL = ${SRC_TEST_PATPOOL:.c=.o}
TEST_LDFLAGS = -L $(CURDIR) -l paqueue -Wl,-rpath,$(CURDIR)

CC = gcc
CFLAGS = -std=c99 -c -g -Wall -Werror -pie -fPIC -pedantic ${INCS}

all: libpaqueue.so test_palist test_patpool

.c.o:
		@echo CC $<
		@${CC} ${CFLAGS} $<

libpaqueue.so: ${OBJ_PA}
		@echo CC -o $@
		@${CC} -shared -o $@ ${OBJ_PA} ${PA_LDFLAGS}

test_palist: ${OBJ_TEST_PALIST}
		@echo CC -o $@
		@${CC} -o $@ ${OBJ_TEST_PALIST} ${TEST_LDFLAGS} 

test_patpool: ${OBJ_TEST_PATPOOL}
		@echo CC -o $@
		@${CC} -o $@ ${OBJ_TEST_PATPOOL} ${TEST_LDFLAGS}

clean:
		@echo Cleaning
		@rm -f ${OBJ_PA} ${OBJ_TEST_PALIST} ${OBJ_TEST_PATPOOL}
		@rm -f test_palist test_patpool
