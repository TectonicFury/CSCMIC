CFLAGS = -Wall -g -std=c11 -fsanitize=address -fsanitize=leak -fno-omit-frame-pointer
CFLAGS1 = -Wall -g -std=c11 -fsanitize=address
OBJECTS = simple_interpreter.o Expr.o

simple-scheme: $(OBJECTS)
	clang -o simple-scheme $(OBJECTS) $(CFLAGS)
simple_interpreter.o: simple_interpreter.c Expr.h
	clang -c simple_interpreter.c $(CFLAGS)
Expr.o: Expr.h Expr.c
	clang -c Expr.c $(CFLAGS)

test_scanner: test_scanner.o Token.o prime.o
	clang -o test_scanner test_scanner.o Token.o prime.o $(CFLAGS1)
test_scanner.o: test_scanner.c
	clang -c test_scanner.c $(CFLAGS)

prime.o: prime.c
	clang -c prime.c $(CFLAGS)

Token: Token.o
	clang -o Token Token.o $(CFLAGS)
Token.o: Token.c
	clang -c Token.c $(CFLAGS)

resizable_array: test_resizable_array.o
	clang -o resizable_array test_resizable_array.o $(CFLAGS1)
test_resizable_array.o: test_resizable_array.c
	clang -c test_resizable_array.c $(CFLAGS)


clean:
	rm *.o simple-scheme test_scanner
