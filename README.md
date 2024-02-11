## A C++ parallel program using MPI to achieve Monte-Carlo simulation of &pi; value
### CS 6220 Course Project, co-contributors: Yunxiang Yan, Qidian Gao, Biwei Tang
### Core highlights:
1. We accept configuation input of n value (the total points used to simulate &pi;) as well as interactive commands upon terminal.
2. We calculated the run-time with different number of processors from 1~10 locally with Macbook Air M2 8+16+10, and for n = 106
   , we plot a graph of run-time of the program vs. the number of processors for a
   few chosen values of p.

### Code logic: 

First, we included the basic MPI package mpi.h for C. MPI_Init is for initializing the environment and receive configs. MPI_Comm_rank is used to retrieve the unique rank of each processors. MPI_Comm_size engages in calculating the number of active processors at each moment.

We achieved separative input using if-else statement:
```
if (rank == 0) {
if (argc == 2) {
n = atoll(argv[1]);
} else {
std::cout << "Enter the number of points: ";
std::cin >> n;
...
}
}
```
The main processor ranked 0 receives the np and broadcasts it to all processors, making sure all the processors knows the total number of dots generated for test.
```
MPI_Bcast(&n, 1, MPI_LONG_LONG_INT, 0, MPI_COMM_WORLD);
```
Next, we calculate the n/p. Notice that if n is not divisible by p, then we allocate +1 number to previous processors
```
local_n = n / size;
if (rank < n % size) {
    local_n++;
}
```
This is the core function to generate random dots using rand() and determine if or not it's inside the circle. Every processor generate local_n numbers of dots, and the separate function is the normal circle function x^2 + Y^2 <=1 with origin(0,0). 
```
srand(time(NULL) + rank);
for (long long int i = 0; i < local_n; i++) {
    x = (double)rand() / RAND_MAX;
    y = (double)rand() / RAND_MAX;
    distance_squared = x*x + y*y;
    if (distance_squared <= 1) {
        points_in_circle++;
    }
}
```
Using the MPI_reduce function, we add all dots calculated by each processor and store the output in variable 'global_points_in_circle'.
```
MPI_Reduce(&points_in_circle, &global_points_in_circle, 1, MPI_LONG_LONG_INT, MPI_SUM, 0, MPI_COMM_WORLD);
```
The calculation in main process is in alignment with the task requirement and the final run-time and &pi; value is rounded up to 12 decimal places.
```
if (rank == 0) {
pi_estimate = (4.0 * global_points_in_circle) / n;
end_time = MPI_Wtime();
printf("%.12f,%.12f\n", pi_estimate, end_time - start_time);
}
```
## Conclusion:
This is a simple but powerful parallel computing case that utilizes the basic MPI function in C++ to estimate the value of &pi;. In this task, we learned how to build makefile, test different numbers of processors using mpirun input.
