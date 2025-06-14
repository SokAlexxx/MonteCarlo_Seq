#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <omp.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <number_of_points>" << std::endl;
        return 1;
    }

    long long N = atoll(argv[1]); 
    long long in_circle = 0;

    int num_threads = 6; 
    omp_set_num_threads(num_threads);

    std::srand(static_cast<unsigned int>(std::time(0)));

    auto start = std::chrono::high_resolution_clock::now();

#pragma omp parallel for reduction(+:in_circle)
    for (long long i = 0; i < N; ++i) {
        double x = (double)rand() / RAND_MAX * 2.0 - 1.0;
        double y = (double)rand() / RAND_MAX * 2.0 - 1.0;
        if (x * x + y * y <= 1.0)
            in_circle++;
    }

    auto end = std::chrono::high_resolution_clock::now();
    double pi = 4.0 * in_circle / N;
    std::chrono::duration<double> duration = end - start;

    std::cout << "Threads used: " << num_threads << std::endl;
    std::cout << "Approximated Pi = " << pi << std::endl;
    std::cout << "Execution time: " << duration.count() << " seconds" << std::endl;

    return 0;
}
