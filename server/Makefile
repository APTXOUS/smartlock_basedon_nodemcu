SOURCE = $(wildcard *.cpp)
TARGETS = $(patsubst %.cpp, %, $(SOURCE)) 
CC = g++
all:$(TARGETS) 
$(TARGETS):%:%.cpp    
	$(CC) $< -o $@ 
	
.PHONY:clean all
clean:    
	-rm $(TARGETS)

