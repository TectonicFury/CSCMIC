CC = /opt/homebrew/Cellar/llvm/13.0.1_1/bin/clang
CFLAGS = -Wall -g -std=c11 -fsanitize=address -fno-omit-frame-pointer
CFLAGS1 = -Wall -g -std=c11 -fsanitize=address

eval:Expr.o Token.o prime.o
	$(CC) -o eval Token.o prime.o Expr.o $(CFLAGS1)
	
Expr.o: Expr.c
	$(CC) -c Expr.c $(CFLAGS)

prime.o: prime.c
	$(CC) -c prime.c $(CFLAGS)

Token: Token.o
	$(CC) -o Token Token.o $(CFLAGS)
Token.o: Token.c
	$(CC) -c Token.c $(CFLAGS)
test_scanner: test_scanner.o 
resizable_array: test_resizable_array.o
	$(CC) -o resizable_array test_resizable_array.o $(CFLAGS1)
test_resizable_array.o: test_resizable_array.c
	$(CC) -c test_resizable_array.c $(CFLAGS)


clean:
	rm *.o test_expr
