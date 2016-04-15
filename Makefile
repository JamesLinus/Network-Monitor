all: piggy

piggy: piggy.o
		gcc -Wall -g -o pig *.c src/*.c headers/*.h

clean:
		@echo -n Cleaning...
		@rm *.o *~ pig 2>/dev/null || true
		@echo done 