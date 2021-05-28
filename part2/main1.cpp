#include <atomic>
#include <iostream>
#include <thread>

using namespace std;

#define TEST_ITERATIONS (1024*256)

atomic_int x(0);
atomic_int y(0);
atomic_int var1(0);
atomic_int var2(0);

void t0_function() {
    atomic_store_explicit(&x, 1, memory_order_relaxed);
    atomic_store_explicit(&var1, atomic_load_explicit(&y, memory_order_relaxed),memory_order_relaxed);
}

void t1_function() {
    atomic_store_explicit(&y, 1, memory_order_relaxed);
    atomic_store_explicit(&var2, atomic_load_explicit(&x, memory_order_relaxed),memory_order_relaxed);
}


int main() {

    int output0 = 0;
    int output1 = 0;
    int output2 = 0;
    int output3 = 0;

    for (int i = 0; i < TEST_ITERATIONS; i++) {
        // reset
        var1.store(0, memory_order_seq_cst);
        var2.store(0, memory_order_seq_cst);
        x.store(0, memory_order_seq_cst);
        y.store(0, memory_order_seq_cst);

        // Run a test iteration
        auto a = thread(t0_function);
        auto b = thread(t1_function);
        a.join();
        b.join();

        // Record a histogram, fill in the conditions
        if (x.load() == 1 && y.load() == 0) {
            output0++;
        } else if (x.load() == 0 && y.load() == 1) {
            output1++;
        } else if (x.load() == 1 && y.load() == 1) {
            output2++;
        }
            // This should be the relaxed behavior
        else if (x.load() == 0 && y.load() == 0) {
            output3++;
        }

    }

    // Complete the print out using your output instantiations
    cout << "histogram of different observations:" << endl;
    cout << "output0: var1=1, var2=0 " << output0 << endl;
    cout << "output1: var1=0, var2=1 " << output1 << endl;
    cout << "output2: var1=1, var2=1 " << output2 << endl;
    cout << "output3: var1=0, var2=0 " << output3 << endl << endl;
    cout << "relaxed behavior frequency: " << float(output3) / float(TEST_ITERATIONS) << endl;

    return 0;

}
