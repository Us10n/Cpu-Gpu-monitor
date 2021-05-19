.PHONY: all
all: 
	g++ -pthread -c *.cpp
	g++ -pthread *.o -o monitor
	rm -f *.o

.PHONY: clean
clean:
	rm -f *.o monitor

