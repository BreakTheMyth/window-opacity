.PHONY: all clean run install uninstall

CC 	    = gcc
CFLAGS  = -Wall -MMD -MP -O2
LDFLAGS = -mwindows
TARGET  = window-opacity.exe
SRCS    = main.c config.c timer.c
OBJS    = $(SRCS:%.c=bin/%.o)
DEPS    = $(OBJS:.o=.d)

-include $(DEPS)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

bin/%.o: src/%.c
	if not exist bin mkdir bin
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	rmdir /s /q bin
	del $(TARGET)