tests:
	gcc -o tests tests/conf_test.c -Wall -Wextra
clean:
	rm tests

example:
	gcc example.c  -o example -Wall -Wextra