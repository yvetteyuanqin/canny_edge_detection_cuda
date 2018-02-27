
CC = gcc
CFLAGS = -std=c++11
COPTFLAGS = -O3 -g -lpng
LDFLAGS =




readtest: readtest.cc
        gcc  $(CC) $(CFLAGS) $(COPTFLAGS) -I/data/apps/boost/1.57/include -o readtest readtest.cc


clean:
	rm -f

# eof
