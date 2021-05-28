#include "common.h"
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

int num_threads = 8;

bool compute_blur(int idx, double *input, double *output) {
    for (int i = idx; i < SIZE; i+=num_threads) {
        if(i == 0 || i == SIZE - 1) continue;
        output[i] = (input[i - 1] + input[i] + input[i + 1]) / 3;
    }
    return true;
}

void print_threads(int size, double *output) {
    for (int i = 0; i < size; i++) {
        cout << output[i] << " ";
    }
    cout << endl;
}

int main(int argc, char *argv[]) {
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
    for (int r = 0; r < REPEATS; r++) {
        // Launch threads to compute the blur
        for (int ti = 0; ti < num_threads; ti++) {  // partition work into threads
            threads[ti] = thread(compute_blur, ti, input, output);
        }
        // Join threads
        for (int ti = 0; ti < num_threads; ti++) {
            threads[ti].join();
        }
        // Swap input and output pointers.
        double *temp = output;
        output = input;
        input = temp;
    }
    print_threads(SIZE, output);

    auto time_end = std::chrono::high_resolution_clock::now();
    auto time_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(time_end - time_start);
    double time_seconds = time_duration.count() / 1000000000.0;
    std::cout << "timings: " << time_seconds << std::endl;

}
