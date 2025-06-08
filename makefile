CC = gcc
CFLAGS = -Wall -g -O2

TARGET1 = Trabalho_T1/t1_app
SRCS1 = Trabalho_T1/main.c $(wildcard Trabalho_T1/src/*.c)

TARGET2 = Trabalho_T2/t2_app
SRCS2 = Trabalho_T2/main.c

TARGET3 = Trabalho_T3/t3_app
SRCS3 = Trabalho_T3/main.c

.PHONY: all
all: $(TARGET1) $(TARGET2) $(TARGET3)

$(TARGET1): $(SRCS1)
	$(CC) $(CFLAGS) -o $@ $^

$(TARGET2): $(SRCS2)
	$(CC) $(CFLAGS) -o $@ $^

$(TARGET3): $(SRCS3)
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean
clean:
	-rm -f $(TARGET1) $(TARGET2) $(TARGET3)