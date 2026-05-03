test:
	gcc -o test tests/conf_test.c -Wall -Wextra
clean:
	rm test example

example:
	gcc example.c  -o example -Wall -Wextra