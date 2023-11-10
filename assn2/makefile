Flag=-Wall -Wextra -g
CC=g++

all: task_scheduling

task_scheduling: task_scheduling_main.o task_scheduling.o 
	${CC} -g task_scheduling_main.o task_scheduling.o -o $@

task_scheduling_main.o: task_scheduling_main.cpp task_scheduling.hpp 
	${CC} ${Flag} -c $< -o $@

task_scheduling.o: task_scheduling.cpp task_scheduling.hpp
	${CC} ${Flag} -c $< -o $@


.PHONY: clean
	clean:
		rm *.o task_scheduling
