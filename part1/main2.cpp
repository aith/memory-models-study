#include "common.h"
#include <iostream>
#include <thread>
#include <chrono>
#include "iostream"

#if defined(SRBARRIER)
#include "SRBarrier.h"
#elif defined(SROBARRIER)
#include "SROBarrier.h"
#else
#error "no barrier specified!"
#endif
barrier_object B;

using namespace std;

int num_threads = 8;

bool compute_blur(int idx, double* input, double* output, barrier_object *B) {
    double *in = input;
    double *out = output;
    for (int i = 0; i < REPEATS; i++) {
        for(int ti = idx; ti < SIZE - 1; ti+=num_threads ) { // partition work based on number of threads
            if (ti == 0 || ti == SIZE-1) continue;
            out[ti] = (in[ti - 1] + in[ti] + in[ti + 1]) / 3;
        }
        B->barrier(idx);
        // flip
        auto temp = in;
        in = out;
        out = temp;
    }
    input = in;
    output = out;
    return true;
}

void print_threads(int size, double *output) {
    for (int i = 0; i < size; i++) {
        cout << output[i] << " ";
    }
    cout << endl;
}

int main(int argc, char *argv[]) {
    num_threads = 8;
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

    B.init(num_threads-2);
    auto time_start = std::chrono::high_resolution_clock::now();

    // Launch threads once
    int start = 1;
    int end = num_threads - 1;
    for (int i = 0; i < num_threads; i++) { // don't blur boundaries
        threads[i] = thread(compute_blur, i, input, output, &B);
    }
    // Join threads once
    for (int i = start; i < end; i++) {
        threads[i].join();
    }
//    print_threads(SIZE, output);
    auto time_end = std::chrono::high_resolution_clock::now();
    auto time_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(time_end - time_start);
    double time_seconds = time_duration.count() / 1000000000.0;

    std::cout << "timings: " << time_seconds << std::endl;

}
