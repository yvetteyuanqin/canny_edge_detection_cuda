
CC = gcc
CFLAGS = -std=c++11
COPTFLAGS = -O3 -g -lpng
LDFLAGS =



canny_par: main_p.cu timer.c canny_p.cu 
	nvCC -std=c++11 -O3 -g -lpng -I/data/apps/boost/1.57/include -o canny_parallel main_p.cu timer.c canny_p.cu -I.



canny_seq: main.cc timer.c canny_sequential.cc 
	g++ -std=c++11 -O0 -g -lpng -I/data/apps/boost/1.57/include -o canny_seq main.cc timer.c canny_sequential.cc -I.


clean:
	rm -f

# eof
