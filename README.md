# CSCMIC [\ ˈsīz-mik ] -- Coding a SCheMe Interpreter in C
I completed [SICP](https://web.mit.edu/alexmv/6.037/sicp.pdf) in October 2021 and while I didn't cover all the sections and do all the exercises, I did enough to want to write an interpreter for Scheme in C (perhaps the lowest of the high level languages). This will probably be a long term project, as most hobby language implementation projects tend to be.

The code draws heavily from the `eval` function in SICP ch 4.1. The functions too have names similar to the ones in the book.

### Progress so far:
- [x] Basic arithmetic (+ - * /)
- [x] Floating point numbers
- [x] Strings
- [x] `if` expressions
- [x] Variable definition and lookup
- [x] `lambda`, function definitions (including nested function definitions)
- [x] Function calls
- [x] Recursion
- [x] Lexical scope (I use hash tables instead of searching through linked list)
- [x] `<` added (`>`, `<=`, `>=`, `=` remain to be added)
- [x] math functions `abs`, `sqrt` (`sin`, `cos` etc to be added)

### What remains:
- [ ] Integer and big integer types (less of a priority in the short term)
- [ ] `cons` and `list` functions
- [ ] `quote`d expressions
- [ ] `let` expressions
- [ ] `begin` expressions
- [ ] garbage collection (doing something like a naive `(fib 40)` will probably **eat up a couple dozen GB**)
- [ ] making it faster
- [ ] better error handling
- [ ] making a (bytecode?) scheme compiler
- [ ] much more ...

Things like fibonacci numbers and factorial work as one would expect:

```scheme
]=> (define (fib n)
      (if (< n 2)
          n
          (+ (fib (- n 1)) (fib (- n 2)))))

;value:
ok

]=> (fib 10)

;value:
55.000000

]=> (define (fact n)
      (if (< n 2)
          n
          (* n (fact (- n 1)))))

;value:
ok

]=> (fact 10)

;value:
3628800.000000
```
You can do nested stuff like this too:
```scheme
]=> (define f (lambda (x)
                (lambda (y)
                  (lambda (z)
                    (* x y z)))))

]=> (((f 2) 4) 8)

;value:
64.000000
```
Square-root using Newton's method:
```scheme
]=> (define (sqrt-newton x)
      (define (improve g)
        (average g (/ x g)))
      (define (average y z)
        (/ (+ y z) 2))
	    (define (square a) (* a a))
      (define (sqrt-iter guess)
	     (if (< (abs (- (square guess) x)) 0.001)
	         guess
	         (sqrt-iter (improve guess))))
      (sqrt-iter x))

;value:
ok

]=> (sqrt-newton 2)

;value:
1.414216
```
### Test Drive
Try it here: [Scheme-interpreter](https://replit.com/@tectonicfury/Scheme-Interpreter-1?v=1)
### Comments
The code organisation is messy and asks for improvement.
