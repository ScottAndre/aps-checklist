
CC=g++-4.9

task-test : Date.cpp Task.cpp main.cpp 
	$(CC) -o $@ $^ --std=c++11

date-test : Date.cpp main.cpp 
	$(CC) -o $@ $^ --std=c++11
