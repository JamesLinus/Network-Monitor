all: piggy1

piggy1: piggy1.o
		gcc -Wall -g -o piggy1 *.c src/*.c headers/*.h

clean:
		@echo -n Cleaning...
		@rm *.o *~ piggy1 2>/dev/null || true
		@echo done 