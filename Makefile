
CC = gcc
CFLAGS = -std=c++11
COPTFLAGS = -O3 -g -lpng
LDFLAGS =


canny_seq: main.o canny_sequential.o
	g++ -std=c++11 -O3 -g -lpng -I/data/apps/boost/1.57/include -o canny_seq main.o canny_sequential.o -I.





clean:
	rm -f

# eof
