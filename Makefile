
CC=g++-4.8

date-test : main.cpp Date.h
	$(CC) -o $@ $^ --std=c++11
