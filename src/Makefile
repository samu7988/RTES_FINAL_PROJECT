#make all or make => then gcc will be used
#make CROSS_COMPILE=aarch64-none-linux-gnu-

CC = $(CROSS_COMPILE)gcc
CFLAGS = -Wall -Werror
SRCS = main.c thread.c v412_driver.c
LIBRARY = -lpthread -lrt -lm
OBJS = $(SRCS:.c=.o) #replace .c file 
TARGET = main #$(SRCS:.c=) #replace .c with nothing"" for eg. writer.c => writer


all: $(TARGET) 
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBRARY)

.c.o:
	$(CC) -c  $(SRCS)

clean:
	rm -f $(TARGET) *.o 