CC = gcc
CFLAGS = -Wall -Wextra

all: bank

bank: bank_account.c
	$(CC) $(CFLAGS) -o bank bank_account.c

clean:
	rm -f bank
