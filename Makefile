CC = gcc
MPICC = mpicc
CFLAGS = -Wall -Wextra -std=c99

all: q2 q4 q3
	@echo "Compilation completed! Run the codes Like below:"
	@echo ">	./q2 <Num_of_Toss>"
	@echo ">	mpirun -np <Num_of_Procesess> ./q3 <Num_of_Toss>"
	@echo ">	./q4 <Num_of_Threads> <Num_of_Toss>"


q1: q2.c
	$(CC) $(CFLAGS) q2.c -o q2 -lm

q4: q4.c
	$(CC) $(CFLAGS) q4.c -o q4 -lpthread -lm

q3: q3.c
	$(MPICC) $(CFLAGS) q3.c -o q3 -lm

clean:
	rm -f q2 q4 q3
