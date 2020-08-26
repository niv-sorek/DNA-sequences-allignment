build:	
	mpicc -fopenmp -c main.c -o main.o
	mpicc -fopenmp -c char_cmp.c -o char_cmp.o
	mpicc -fopenmp -c sequence.c -o sequence.o
	mpicc -fopenmp -c utils.c -o utils.o
	mpicc -fopenmp -o ParallelRun main.o utils.o sequence.o char_cmp.o

run: 
	mpiexec -np 5 ./ParallelRun
