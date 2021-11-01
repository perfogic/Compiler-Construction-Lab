gcc -c -Wall reader.c
gcc -c -Wall charcode.c
gcc -c -Wall token.c
gcc -c -Wall error.c
gcc -c -Wall scanner.c
gcc -c -Wall parser.c
gcc -c main.c
gcc main.o parser.o scanner.o reader.o charcode.o token.o error.o -o main