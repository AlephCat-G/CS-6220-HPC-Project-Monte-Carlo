#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

int main(int argc, char** argv) {
    int rank, size, i;
    int n, points_in_circle = 0, global_points_in_circle = 0;
    double x, y, pi_estimate, distance_squared;
    double start_time, end_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        if (argc < 2) {
            fprintf(stderr, "Usage: %s number_of_points\n", argv[0]);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        n = atoi(argv[1]);
        start_time = MPI_Wtime();
    }

    // Broadcast n to all processors
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Seed the random number generator differently on each processor
    srand(time(NULL) + rank);

    // Calculate the number of points for each processor
    int points_per_processor = n / size;
    int excess = n % size;
    int start = rank * points_per_processor;
    int end = start + points_per_processor + (rank < excess ? 1 : 0);

    for (i = start; i < end; i++) {
        x = (double)rand() / RAND_MAX;
        y = (double)rand() / RAND_MAX;
        distance_squared = x*x + y*y;
        if (distance_squared <= 1) {
            points_in_circle++;
        }
    }

    // Reduce all the local points_in_circle to a global sum
    MPI_Reduce(&points_in_circle, &global_points_in_circle, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        // Finalize the estimate of Pi
        pi_estimate = 4.0 * global_points_in_circle / n;
        end_time = MPI_Wtime();

        printf("Estimated Pi: %f\n", pi_estimate);
        printf("Time elapsed: %f seconds\n", end_time - start_time);
    }

    MPI_Finalize();
    return 0;
}