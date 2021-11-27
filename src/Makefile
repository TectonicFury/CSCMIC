CFLAGS = -Wall -g -std=c11 -fsanitize=address -fsanitize=leak -fno-omit-frame-pointer
CFLAGS1 = -Wall -g -std=c11 -fsanitize=address

Expr.o: Expr.c
	clang -c Expr.c $(CFLAGS)

eval:Expr.o Token.o prime.o
	clang -o eval Token.o prime.o Expr.o $(CFLAGS1)

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
	rm *.o test_expr
