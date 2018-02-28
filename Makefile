
CC = gcc
CFLAGS = -std=c++11
COPTFLAGS = -O3 -g -lpng
LDFLAGS =


readtest: readtest.cc 
	g++ -std=c++11 -O3 -g -lpng -I/data/apps/boost/1.57/include -o readtest readtest.cc


readtest2: readtest_2.cc 
	g++ -std=c++11 -O3 -g -lpng -I/data/apps/boost/1.57/include -o readtest_2 readtest_2.cc


clean:
	rm -f

# eof
