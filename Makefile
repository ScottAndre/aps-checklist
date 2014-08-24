
CC=g++-4.9

checklist : Date.cpp Task.cpp main.cpp 
	$(CC) -o $@ $^ --std=c++11

test : unittest.cpp Date.cpp Task.cpp 
	$(CC) -D UNIT_TEST -o $@ $^ --std=c++11
