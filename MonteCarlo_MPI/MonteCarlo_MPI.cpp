#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc != 2) {
        if (rank == 0)
            std::cerr << "Usage: " << argv[0] << " <number_of_points>" << std::endl;
        MPI_Finalize();
        return 1;
    }

    long long total_points = atoll(argv[1]);
    long long local_points = total_points / size;
    long long local_in_circle = 0;

    std::srand(static_cast<unsigned int>(time(NULL)) + rank); 

    double start_time = MPI_Wtime();

    for (long long i = 0; i < local_points; ++i) {
        double x = (double)std::rand() / RAND_MAX * 2.0 - 1.0;
        double y = (double)std::rand() / RAND_MAX * 2.0 - 1.0;
        if (x * x + y * y <= 1.0)
            local_in_circle++;
    }

    long long total_in_circle = 0;
    MPI_Reduce(&local_in_circle, &total_in_circle, 1, MPI_LONG_LONG_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    double end_time = MPI_Wtime();

    if (rank == 0) {
        double pi = 4.0 * total_in_circle / total_points;
        std::cout << "Approximated Pi = " << pi << std::endl;
        std::cout << "Execution time: " << (end_time - start_time) << " seconds" << std::endl;
    }

    MPI_Finalize();
    return 0;
}
