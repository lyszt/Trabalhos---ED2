CC = gcc
CFLAGS = -Wall -g -O2



TARGET2 = Trabalho_T2/t2_app
SRCS2 = Trabalho_T2/main.c

TARGET3 = Trabalho_T3/t3_app
SRCS3 = Trabalho_T3/main.c

.PHONY: all
all: $(TARGET1) $(TARGET2) $(TARGET3)


$(TARGET2): $(SRCS2)
	$(CC) $(CFLAGS) -o $@ $^

$(TARGET3): $(SRCS3)
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean
clean:
	-rm -f $(TARGET1) $(TARGET2) $(TARGET3)