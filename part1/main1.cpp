#include "common.h"
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

bool compute_blur(int idx, double *input, double *output) {
    output[idx] = (input[idx - 1] + input[idx] + input[idx + 1]) / 3;
    return true;
}

void print_threads(int num_threads, double *output) {
    for (int i = 0; i < num_threads; i++) {
        cout << output[i] << " ";
    }
    cout << endl;
}

int main(int argc, char *argv[]) {
    int num_threads = 8;
    if (argc > 1) {
        num_threads = atoi(argv[1]);
    }

    double *input = new double[SIZE];
    double *output = new double[SIZE];
    std::thread *threads = new std::thread[num_threads];

    for (int i = 0; i < SIZE; i++) {
        double randval = fRand(-100.0, 100.0);
        input[i] = randval;
        output[i] = randval;
    }

    auto time_start = std::chrono::high_resolution_clock::now();
    int start = 1;
    int end = num_threads - 1;
    print_threads(num_threads, output);
    for (int r = 0; r < REPEATS; r++) {
        // Launch threads to compute the blur
        for (int i = start; i < end; i++) { // don't blur boundaries
            threads[i] = thread(compute_blur, i, input, output);
        }
        // Join threads
        for (int i = start; i < end; i++) {
            threads[i].join();
        }
        // Swap input and output pointers.
        double *temp = output;
        output = input;
        input = temp;
    }
    print_threads(num_threads, output);

    auto time_end = std::chrono::high_resolution_clock::now();
    auto time_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(time_end - time_start);
    double time_seconds = time_duration.count() / 1000000000.0;
    std::cout << "timings: " << time_seconds << std::endl;

}
