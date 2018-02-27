.DEFAULT_GOAL := all

CC = gcc
CFLAGS = -std=c++11
COPTFLAGS = -O3 -g -lpng
LDFLAGS =

TARGETS = readtest$(EXEEXT)

all: $(TARGETS)

readtest$(EXEEXT): readtest.cc $(CC) $(CFLAGS) $(COPTFLAGS) -I/data/apps/boost/1.57/include \
	    -o $@ readtest.cc


clean:
	rm -f $(TARGETS) 

# eof
