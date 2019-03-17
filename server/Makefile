SOURCE = $(wildcard *.cpp)
TARGETS = $(patsubst %.cpp, %, $(SOURCE)) 
CC = g++
all:$(TARGETS) 
$(TARGETS):%:%.cpp    
	$(CC)  $< -o $@ -I /usr/include/mysql/  -lmysqlclient -L/usr/lib64/mysql
	
.PHONY:clean all
clean:    
	-rm $(TARGETS)

