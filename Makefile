
CC=g++-4.9

task-test : Date.cpp Task.cpp main.cpp 
	$(CC) -o $@ $^ --std=c++11

test : Date.cpp Task.cpp unittest.cpp 
	$(CC) -o $@ $^ --std=c++11
