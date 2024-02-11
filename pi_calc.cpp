#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include <iostream>

int main(int argc, char** argv) {
    int rank, size;
    long long int n, local_n, points_in_circle = 0, global_points_in_circle = 0;
    double x, y, pi_estimate, distance_squared;
    double start_time, end_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        if (argc == 2) {
            n = atoll(argv[1]);
            start_time = MPI_Wtime();
        } else {
            std::cout << "Enter the number of points: ";
            std::cin >> n;
            if (std::cin.fail()) {
                std::cerr << "Error: Invalid input. Please enter a valid integer number.\n";
                MPI_Abort(MPI_COMM_WORLD, 1);
                exit(1);
            }
        }
    }

    MPI_Bcast(&n, 1, MPI_LONG_LONG_INT, 0, MPI_COMM_WORLD);
    local_n = n / size;
    if (rank < n % size) {
        local_n++;
    }

    srand(time(NULL) + rank);
    for (long long int i = 0; i < local_n; i++) {
        x = (double)rand() / RAND_MAX;
        y = (double)rand() / RAND_MAX;
        distance_squared = x*x + y*y;
        if (distance_squared <= 1) {
            points_in_circle++;
        }
    }

    MPI_Reduce(&points_in_circle, &global_points_in_circle, 1, MPI_LONG_LONG_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        pi_estimate = (4.0 * global_points_in_circle) / n;
        end_time = MPI_Wtime();
        printf("%.12f,%.12f\n", pi_estimate, end_time - start_time);
    }

    MPI_Finalize();
    return 0;
}
