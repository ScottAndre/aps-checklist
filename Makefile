
CC=g++-4.9

date-test : main.cpp Date.h
	$(CC) -o $@ $^ --std=c++11
