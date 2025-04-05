.PHONY: all clean run install uninstall

CC 	    = gcc
CFLAGS  = -Wall -MMD -MP -g -O2
LDFLAGS = -mwindows
TARGET  = window-opacity.exe
SRCS    = main.c
OBJS    = $(SRCS:%.c=%.o)
DEPS    = $(OBJS:.o=.d)

-include $(DEPS)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $^ -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	del $(TARGET) $(OBJS) $(DEPS)

run: all
	./$(TARGET)

install: all
	copy $(TARGET) "C:\Users\%USERNAME%\AppData\Roaming\Microsoft\Windows\Start Menu\Programs\Startup"

uninstall:
	del "C:\Users\%USERNAME%\AppData\Roaming\Microsoft\Windows\Start Menu\Programs\Startup\$(TARGET)"