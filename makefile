export CFLAGS = -Wall -Wextra -Werror

.PHONY: initializer

all: initializer run clean

initializer:
	gcc $(CFLAGS) $(CURDIR)/*.c -o tomasulo

run:
	@./tomasulo

clean:
	@rm -f out
