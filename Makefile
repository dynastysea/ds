CC=gcc
CFLAGS=-g -Wall 
#OBJPATH=./build
SOURCES=$(wildcard *.c ./core/*.c)
OBJECTS=$(addprefix ./, $(addsuffix .o, $(basename $(SOURCES))))

TARGET=ds
all: $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) -lpthread 

%.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $<
clean : 
	-rm $(OBJECTS) $(TARGET)
