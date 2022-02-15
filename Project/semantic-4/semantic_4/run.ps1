gcc -c charcode.c 
gcc -c debug.c 
gcc -c error.c 
gcc -c main.c 
gcc -c parser.c 
gcc -c reader.c 
gcc -c scanner.c 
gcc -c symtab.c 
gcc -c token.c 
gcc -c semantics.c
gcc -o 1 main.o semantics.o charcode.o debug.o error.o parser.o reader.o scanner.o symtab.o token.o
./1 "./tests/example4.kpl"